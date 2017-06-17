#include "core/domain/Lending.h"
#include "core/domain/Lending.priv.h"
#include "core/exceptions.h"

#include <cassert>

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

    /* Initialize runtime with default value */
    //TODO: Where to configure??
    throw NotImplementedException();
    priv->runtime = 14;
}

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

void Lending::extend(int days) {
    throw NotImplementedException();
    //TODO: Extension API seems not ready yet...
}

int Lending::getDaysLeft() const {
    time_t now = time(NULL);
    return (int)((priv->timestampLent + priv->runtime - now) / 60 / 60 / 24); //TODO: Change if extension API changes
}

unsigned int Lending::getRuntime() const {
    return priv->runtime;
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
