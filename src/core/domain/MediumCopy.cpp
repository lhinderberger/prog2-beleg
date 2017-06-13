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
}

MediumCopy::~MediumCopy() = default;


void MediumCopy::persistImpl() {
    /* Prepare statement */
    SqlitePreparedStatement statement(getConnection(), isLoaded() ?
        buildUpdateQuery<MediumCopy>({"deaccessioned", "location"}, "WHERE medium_ean=? AND serial_number=?") :
        buildInsertQuery<MediumCopy>({"deaccessioned", "location", "medium_ean", "serial_number"}, 1)
    );

    /* Bind parameters */
    statement.bind(1, priv->deaccessioned);
    statement.bind(2, priv->location);
    statement.bind(3, priv->medium->getEAN());
    statement.bind(4, priv->serialNumber);

    /* Execute */
    statement.step();
}


shared_ptr<Lending> MediumCopy::getActiveLending() const {
    throw NotImplementedException();
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

shared_ptr<Medium> MediumCopy::getMedium() const {
    return priv->medium.get();
}

int MediumCopy::getSerialNumber() const {
    return priv->serialNumber;
}
