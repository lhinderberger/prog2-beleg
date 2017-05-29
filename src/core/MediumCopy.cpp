#include "core/MediumCopy.h"
#include "core/Medium.priv.h"
#include "core/MediumCopy.priv.h"
#include "core/exceptions.h"

#include <cassert>

using namespace std;
using namespace pb2;

MediumCopy::MediumCopy(std::shared_ptr<Medium> medium, int serialNumber) {
    priv = make_unique<MediumCopy_priv>();

    // Verify and copy identifying fields
    assert(medium);
    priv->medium = medium;
    priv->serialNumber = serialNumber;

    // Initialize remaining fields
    priv->deaccessioned = false;
}

shared_ptr<MediumCopy> MediumCopy::construct(shared_ptr<Medium> medium, int serialNumber) {
    return shared_ptr<MediumCopy>(new MediumCopy(medium, serialNumber));
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
