#include "core/domain/MediumCopy.h"
#include "core/domain/Medium.priv.h"
#include "core/domain/MediumCopy.priv.h"
#include "core/exceptions.h"
#include "core/query-builder.h"

#include <cassert>

using namespace std;
using namespace pb2;

const string MediumCopy::tableName = "media_copies";


MediumCopy::MediumCopy(shared_ptr<Database> database, shared_ptr<Medium> medium,
                       int serialNumber) : DatabaseObject(database) {
    priv = make_unique<MediumCopy_priv>(database);

    // Verify and copy identifying fields
    assert(medium);
    priv->medium.set(medium);
    priv->serialNumber = serialNumber;
    priv->availabilityHint = "now";

    // Initialize remaining fields
    priv->deaccessioned = false;
}

MediumCopy::MediumCopy(
        shared_ptr<Database> database, SqlitePreparedStatement & query,
        const map<string, int> & columnIndexes
) : DatabaseObject(database) {
    priv = make_unique<MediumCopy_priv>(database);

    priv->deaccessioned = query.columnInt(columnIndexes.at("deaccessioned")) != 0;
    priv->location = query.columnString(columnIndexes.at("location"));
    priv->serialNumber = query.columnInt(columnIndexes.at("serial_number"));
    priv->medium.set(query.columnString(columnIndexes.at("medium_ean")));
    priv->availabilityHint = query.columnString(columnIndexes.at("availability_hint"));
}

MediumCopy::~MediumCopy() = default;


void MediumCopy::del() {
    SqlitePreparedStatement query(getConnection(), "DELETE FROM " + tableName + " WHERE medium_ean = ? AND serial_number = ?");
    query.bind(1, getMediumEAN());
    query.bind(2, getSerialNumber());
    query.step();
}

void MediumCopy::persistImpl() {
    /* Determine serial number, if required */
    int serialNumber = priv->serialNumber;
    if (serialNumber <= 0) {
        SqlitePreparedStatement snQuery(
                getConnection(), string("SELECT MAX(serial_number) FROM ") + tableName
        );
        serialNumber = snQuery.step() && !snQuery.columnIsNull(0) ? snQuery.columnInt(0) + 1 : 1;
    }

    /* Prepare statement */
    SqlitePreparedStatement statement(getConnection(), isLoaded() ?
        buildUpdateQuery<MediumCopy>({"deaccessioned", "location", "availability_hint"}, "WHERE medium_ean=? AND serial_number=?") :
        buildInsertQuery<MediumCopy>({"deaccessioned", "location", "availability_hint", "medium_ean", "serial_number"}, 1)
    );

    /* Bind parameters */
    statement.bind(1, priv->deaccessioned);
    statement.bind(2, priv->location);
    statement.bind(3, priv->availabilityHint);
    statement.bind(4, priv->medium->getEAN());
    statement.bind(5, serialNumber);

    /* Execute */
    statement.step();
    priv->serialNumber = serialNumber;
}


shared_ptr<Lending> MediumCopy::getActiveLending() const {
    SqlitePreparedStatement query(
            getConnection(),
            "SELECT * FROM " + Lending::tableName +
            " WHERE medium_ean = ? AND medium_copy_serial_number = ? "
            "AND (timestamp_returned = 0 OR timestamp_returned IS NULL)"
    );
    query.bind(1, getMediumEAN());
    query.bind(2, getSerialNumber());
    if (!query.step())
        return nullptr;
    else
        return DatabaseObjectFactory<Lending>(getDatabase()).load(query);
}

bool MediumCopy::getDeaccessioned() const {
    return priv->deaccessioned;
}

void MediumCopy::setDeaccessioned(bool deaccessioned) {
    priv->deaccessioned = deaccessioned;
}

string MediumCopy::getLocation() const {
    return priv->location;
}

void MediumCopy::setLocation(const string &location) {
    priv->location = location;
}

string MediumCopy::getAvailabilityHint() const {
    return priv->availabilityHint;
}

void MediumCopy::setAvailabilityHint(const string &availabilityHint) {
    priv->availabilityHint = availabilityHint;
}

shared_ptr<Medium> MediumCopy::getMedium() const {
    return priv->medium.get();
}

string MediumCopy::getMediumEAN() const {
    if (priv->medium.isPrimaryKeySet())
        return priv->medium.getPrimaryKey();
    else
        return priv->medium.get()->getEAN();
}

int MediumCopy::getSerialNumber() const {
    return priv->serialNumber;
}
