#include "core/Lending.h"
#include "core/Lending.priv.h"
#include "core/exceptions.h"

#include <cassert>

using namespace std;
using namespace pb2;

Lending::Lending(shared_ptr<MediumCopy> mediumCopy,
                 shared_ptr<LibraryUser> libraryUser, time_t timestampLent) {
    priv = make_unique<Lending_priv>();
    priv->timesExtended = 0;
    priv->timestampReturned = 0;

    /* Copy relationship pointers */
    assert(mediumCopy);
    priv->mediumCopy = mediumCopy;
    assert(libraryUser);
    priv->libraryUser = libraryUser;

    /* Set starting timestamp */
    //TODO: Validate!
    throw NotImplementedException();
    priv->timestampLent = timestampLent;

    /* Initialize runtime with default value */
    //TODO: Where to configure??
    priv->runtime = 14;
}

std::shared_ptr<Lending> Lending::construct(std::shared_ptr<MediumCopy> mediumCopy,
                                            std::shared_ptr<LibraryUser> libraryUser,
                                            time_t timestampLent) {
    return shared_ptr<Lending>(new Lending(mediumCopy, libraryUser, timestampLent));
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

void Lending::extend(int days) {
    throw NotImplementedException();
}

int Lending::getDaysLeft() const {
    throw NotImplementedException();
}

unsigned int Lending::getRuntime() const {
    return priv->runtime;
}

time_t Lending::getTimestampReturned() const {
    throw NotImplementedException();
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
