#include "core/domain/Lending.h"
#include "core/domain/Lending.priv.h"
#include "core/exceptions.h"
#include "core/query-builder.h"

#include <cstdlib>
#include <cstring>

using namespace std;
using namespace pb2;

const string Lending::tableName = "lendings";

void parseDate(std::tm & tm, const string & isoDateString) {
    memset(&tm, 0, sizeof(std::tm));
    tm.tm_hour = 23;
    tm.tm_min = tm.tm_sec = 59;
    tm.tm_wday = tm.tm_yday = 0;
    if (sscanf(isoDateString.c_str(), "%d-%d-%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday) != 3)
        throw DatabaseIntegrityException("Invalid due date format in Database!");
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    if (mktime(&tm) == -1)
        throw logic_error("mktime failed - what a surprise...");
}

Lending::Lending(shared_ptr<Database> database, shared_ptr<MediumCopy> mediumCopy,
                 shared_ptr<LibraryUser> libraryUser, time_t timestampLent, int id)
        : DatabaseObject(database){
    priv = make_unique<Lending_priv>(database);
    priv->timesExtended = 0;
    priv->timestampReturned = 0;

    /* Copy relationship pointers */
    if (!mediumCopy)
        throw NullPointerException();
    priv->mediumCopy = mediumCopy;
    priv->mediumEan = mediumCopy->getMedium()->getEAN();
    priv->mediumCopySerialNumber = mediumCopy->getSerialNumber();

    if (!libraryUser)
        throw NullPointerException();
    priv->libraryUser.set(libraryUser);

    /* Set starting timestamp */
    priv->timestampLent = timestampLent;

    /* Initialize due date with default value */
    priv->dueDate = *gmtime(&priv->timestampLent);
    priv->dueDate.tm_hour = 23;
    priv->dueDate.tm_min = priv->dueDate.tm_sec = 59;
    priv->dueDate.tm_mday += atoi(getDatabase()->getMeta("default_lending_runtime").c_str());
}

Lending::Lending(shared_ptr<Database> database, shared_ptr<MediumCopy> mediumCopy,
                 shared_ptr<LibraryUser> libraryUser, int id)
        : Lending(database, mediumCopy, libraryUser, time(NULL), id) {}

Lending::Lending(
        shared_ptr<Database> database, SqlitePreparedStatement & query,
        const map<string, int> & columnIndexes
) : DatabaseObject(database) {
    priv = make_unique<Lending_priv>(database);

    priv->id = query.columnInt(columnIndexes.at("id"));
    priv->mediumCopySerialNumber = query.columnInt(columnIndexes.at("medium_copy_serial_number"));
    priv->mediumEan = query.columnString(columnIndexes.at("medium_ean"));
    priv->libraryUser.set(query.columnInt(columnIndexes.at("library_user_id")));
    priv->timestampLent = query.columnInt(columnIndexes.at("timestamp_lent"));
    parseDate(priv->dueDate, query.columnString(columnIndexes.at("due_date")));
    priv->timestampReturned = query.columnInt(columnIndexes.at("timestamp_returned"));
    int te = query.columnInt(columnIndexes.at("times_extended"));
    if (te < 0)
        throw DatabaseIntegrityException("Times extended is negative");
    else
        priv->timesExtended = (unsigned int)te;
}

Lending::~Lending() = default;


void Lending::persistImpl() {
    /* Prepare statement */
    vector<string> columns = {"timestamp_returned", "times_extended", "due_date", "library_user_id", "medium_ean", "medium_copy_serial_number", "timestamp_lent"};
    if (!isLoaded() && priv->id > 0)
        columns.push_back("id");

    SqlitePreparedStatement statement(getConnection(), isLoaded() ?
        buildUpdateQuery<Lending>(columns, "WHERE id = ?") :
        buildInsertQuery<Lending>(columns, 1)
    );

    /* Bind parameters */
    statement.bind(1, (int)priv->timestampReturned);
    statement.bind(2, (int)priv->timesExtended);
    // 3: see below
    // 4: see below
    statement.bind(5, priv->mediumEan);
    statement.bind(6, priv->mediumCopySerialNumber);
    statement.bind(7, (int)priv->timestampLent);
    if (priv->id > 0)
        statement.bind(8, priv->id);

    /* Get due date */
    statement.bind(3, getDueDateISOString());

    /* Get library user primary key */
    int primaryKey = 0;
    if (priv->libraryUser.isPrimaryKeySet())
        primaryKey = priv->libraryUser.getPrimaryKey();
    else if (priv->libraryUser.isLoaded())
        primaryKey = priv->libraryUser.get()->getId();
    if (primaryKey == 0)
        throw logic_error("Primary key for library user cannot be 0");
    statement.bind(4, primaryKey);

    /* Execute */
    statement.step();

    /* Retrieve ID */
    if (priv->id <= 0)
        priv->id = (int)getConnection()->lastInsertRowId();

    /* Set availability hint on MediumCopy */
    getMediumCopy()->setAvailabilityHint(isReturned() ? "now" : getDueDateISOString());
    getMediumCopy()->persist();
}

shared_ptr<MediumCopy> Lending::getMediumCopy() {
    /* On-the-fly read-only lazy loader */
    if (!priv->mediumCopy) {
        SqlitePreparedStatement query(
                getConnection(),
                string("SELECT * FROM ") + MediumCopy::tableName +
                " WHERE medium_ean = ? AND serial_number = ?"
        );
        query.bind(1, priv->mediumEan);
        query.bind(2, priv->mediumCopySerialNumber);
        query.step();
        priv->mediumCopy = DatabaseObjectFactory<MediumCopy>(getDatabase()).load(query);
    }

    return priv->mediumCopy;
}

shared_ptr<LibraryUser> Lending::getLibraryUser() {
    return priv->libraryUser.get();
}

time_t Lending::getTimestampLent() const {
    return priv->timestampLent;
}

void Lending::extend() {
    extend(atoi(getDatabase()->getMeta("default_extend_days").c_str()));
}

void Lending::extend(int days) {
    extend(time(NULL), days);
}

void Lending::extend(time_t reference, int days) {
    if (days <= 0)
        throw logic_error("Extend Days cannot be zero or negative!");

    if (isReturned())
        throw logic_error("Cannot extend returned Lending!");

    /* Calculate new due date */
    std::tm newDueDate = *gmtime(&reference);
    newDueDate.tm_mday += days;
    newDueDate.tm_hour = 23;
    newDueDate.tm_min = newDueDate.tm_sec = 59;

    /* Compare due dates */
    time_t tsNew = mktime(&newDueDate);
    time_t tsOld = mktime(&priv->dueDate);
    if (tsNew <= tsOld)
        throw NotExtensibleException();

    /* Set new due date; increment counter */
    priv->dueDate = newDueDate;
    priv->timesExtended++;
}

int Lending::getDaysLeft() const {
    return getDaysLeft(time(NULL));
}

int Lending::getDaysLeft(time_t reference) const {
    if (isReturned() && priv->timestampReturned < reference)
        reference = priv->timestampReturned;
    return (int)((mktime(&priv->dueDate) + priv->dueDate.tm_gmtoff - reference) / 60 / 60 / 24);
}

tm Lending::getDueDate() const {
    return priv->dueDate;
}

string Lending::getDueDateISOString() const {
    char due[12];
    due[11] = 0;
    mktime(&priv->dueDate);
    strftime(due, 11, "%F", &priv->dueDate);
    return string(due);
}

time_t Lending::getTimestampReturned() const {
    if (!priv->timestampReturned)
        throw LendingNotReturnedException();
    return priv->timestampReturned;
}

unsigned int Lending::getTimesExtended() const {
    return priv->timesExtended;
}

bool Lending::isReturned() const {
    return priv->timestampReturned > priv->timestampLent;
}

void Lending::returnL(time_t timestampReturned) {
    if (isReturned())
        throw logic_error("Already returned!");
    priv->timestampReturned = timestampReturned;
}

void Lending::returnL() {
    returnL(time(NULL));
}
