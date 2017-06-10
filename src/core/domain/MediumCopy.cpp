#include "core/domain/MediumCopy.h"
#include "core/domain/Medium.priv.h"
#include "core/domain/MediumCopy.priv.h"
#include "core/exceptions.h"

#include <cassert>

using namespace std;
using namespace pb2;

const string MediumCopy::tableName = "medium_copies";


MediumCopy::MediumCopy(shared_ptr<Database> database, shared_ptr<Medium> medium,
                       int serialNumber) : DatabaseObject(database) {
    priv = make_unique<MediumCopy_priv>();

    // Verify and copy identifying fields
    assert(medium);
    priv->medium = medium;
    priv->serialNumber = serialNumber;

    // Initialize remaining fields
    priv->deaccessioned = false;
}

MediumCopy::MediumCopy(
        shared_ptr<Database> database, SqlPreparedStatement & query,
        const map<string, int> & columnIndexes
) : DatabaseObject(database) {
    throw NotImplementedException();
}

MediumCopy::~MediumCopy() = default;


void MediumCopy::persistImpl() {
    throw NotImplementedException();
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
    //TODO: Validate
    throw NotImplementedException();

    priv->location = location;
}

shared_ptr<Medium> MediumCopy::getMedium() const {
    return priv->medium;
}

int MediumCopy::getSerialNumber() const {
    return priv->serialNumber;
}
