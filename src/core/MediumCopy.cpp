#include "core/MediumCopy.h"
#include "core/Medium.priv.h"
#include "core/MediumCopy.priv.h"
#include "core/exceptions.h"

#include <cassert>

using namespace std;
using namespace pb2;

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

shared_ptr<MediumCopy> MediumCopy::construct(shared_ptr<Database> database,
                                             shared_ptr<Medium> medium, int serialNumber) {
    return shared_ptr<MediumCopy>(new MediumCopy(database, medium, serialNumber));
}

const string & MediumCopy::getTableName() const {
    static string t("medium_copy");
    return t;
}

const string & MediumCopy::getType() const {
    static string t("mediumcopy");
    return t;
}

void MediumCopy::load(SqlPreparedStatement & query,
                      const map<string, string> & alternativeColumnNames) {
    throw NotImplementedException();
}

void MediumCopy::persist() {
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
