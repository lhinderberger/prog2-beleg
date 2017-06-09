#include "core/exceptions.h"
#include "core/LibraryUser.h"
#include "core/LibraryUser.priv.h"

using namespace std;
using namespace pb2;

LibraryUser::LibraryUser(shared_ptr<Database> database, int id)
        : DatabaseObject(database) {
    priv = make_unique<LibraryUser_priv>();

    priv->id = id;
}

shared_ptr<LibraryUser> LibraryUser::construct(shared_ptr<Database> database, int id) {
    return shared_ptr<LibraryUser>(new LibraryUser(database, id));
}

const string & LibraryUser::getTableName() const {
    static string t("library_users");
    return t;
}

const string & LibraryUser::getType() const {
    static string t("libraryuser");
    return t;
}


void LibraryUser::loadImpl(SqlPreparedStatement & query, const map<string, int> & columnIndexes) {
    throw NotImplementedException();
}

void LibraryUser::persistImpl() {
    throw NotImplementedException();
}


int LibraryUser::getId() const {
    return priv->id;
}

string LibraryUser::getFirstName() const {
    return priv->firstName;
}

void LibraryUser::setFirstName(const string & firstName) {
    //TODO: Validate
    throw NotImplementedException();

    priv->firstName = firstName;
}

string LibraryUser::getLastName() const {
    return priv->lastName;
}

void LibraryUser::setLastName(const string & lastName) {
    //TODO: Validate
    throw NotImplementedException();

    priv->lastName = lastName;
}

TelephoneNumber LibraryUser::getTelephone() const {
    return priv->telephone;
}

void LibraryUser::setTelephone(const TelephoneNumber & telephone) {
    //TODO: Validate?
    throw NotImplementedException();

    priv->telephone = telephone;
}

shared_ptr<PostalAddress> LibraryUser::getPostalAddress() const {
    //TODO: Lazy loading?
    return priv->postalAddress;
}

void LibraryUser::setPostalAddress(shared_ptr<PostalAddress> postalAddress) {
    priv->postalAddress = postalAddress;
}
