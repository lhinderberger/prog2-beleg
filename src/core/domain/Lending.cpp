#include "core/domain/Lending.h"
#include "core/domain/Lending.priv.h"
#include "core/exceptions.h"

#include <cstdlib>

using namespace std;
using namespace pb2;

const string Lending::tableName = "lendings";

Lending::Lending(shared_ptr<Database> database, shared_ptr<MediumCopy> mediumCopy,
                 shared_ptr<LibraryUser> libraryUser, time_t timestampLent)
        : DatabaseObject(database){
    priv = make_unique<Lending_priv>();
    priv->timesExtended = 0;
    priv->timestampReturned = 0;

    /* Copy relationship pointers */
    if (!mediumCopy)
        throw NullPointerException();
    priv->mediumCopy = mediumCopy;
    if (!libraryUser)
        throw NullPointerException();
    priv->libraryUser = libraryUser;

    /* Set starting timestamp */
    priv->timestampLent = timestampLent;

    /* Initialize due date with default value */
    priv->dueDate = *localtime(&priv->timestampLent);
    priv->dueDate.tm_mday += atoi(getDatabase()->getMeta("default_lending_runtime").c_str());
}

Lending::Lending(shared_ptr<Database> database, shared_ptr<MediumCopy> mediumCopy,
                 shared_ptr<LibraryUser> libraryUser)
        : Lending(database, mediumCopy, libraryUser, time(NULL)) {}

Lending::Lending(
        shared_ptr<Database> database, SqlitePreparedStatement & query,
        const map<string, int> & columnIndexes
) : DatabaseObject(database) {
    throw NotImplementedException();
}

Lending::~Lending() = default;


void Lending::persistImpl() {
    throw NotImplementedException();
}

shared_ptr<MediumCopy> Lending::getMediumCopy() {
    return priv->mediumCopy;
}

shared_ptr<LibraryUser> Lending::getLibraryUser() {
    return priv->libraryUser;
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

    /* Calculate new due date */
    std::tm newDueDate = *localtime(&reference);
    newDueDate.tm_mday += days;
    newDueDate.tm_hour = newDueDate.tm_min = newDueDate.tm_sec = 0;

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
    return (int)((mktime(&priv->dueDate) - reference) / 60 / 60 / 24); //TODO: Write test!
}

std::tm Lending::getDueDate() const {
    return priv->dueDate;
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
    throw NotImplementedException();
}
