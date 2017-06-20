#include "core/exceptions.h"
#include "core/domain/LibraryUser.h"
#include "core/domain/LibraryUser.priv.h"

#include "core/query-builder.h"

using namespace std;
using namespace pb2;

const string LibraryUser::tableName = "library_users";

LibraryUser::LibraryUser(shared_ptr<Database> database, int id)
        : DatabaseObject(database) {
    priv = make_unique<LibraryUser_priv>(database);

    priv->id = id;
}

LibraryUser::LibraryUser(
        shared_ptr<Database> database, SqlitePreparedStatement & query,
        const map<string, int> & columnIndexes
) : DatabaseObject(database) {
    priv = make_unique<LibraryUser_priv>(database);

    priv->id = query.columnInt(columnIndexes.at("id"));
    priv->firstName = query.columnString(columnIndexes.at("first_name"));
    priv->lastName = query.columnString(columnIndexes.at("last_name"));
    priv->telephone = query.columnString(columnIndexes.at("telephone"));
    priv->postalAddress.set(query.columnInt(columnIndexes.at("postal_address_id")));
}

LibraryUser::~LibraryUser() = default;


void LibraryUser::persistImpl() {
    /* Prepare statement */
    vector<string> columns = {"first_name", "last_name", "telephone", "postal_address_id"};
    if (!isLoaded() && priv->id > 0)
        columns.push_back("id");
    SqlitePreparedStatement statement(getConnection(), isLoaded() ?
        buildUpdateQuery<LibraryUser>(columns, "WHERE id=?") :
        buildInsertQuery<LibraryUser>(columns, 1)
    );

    /* Bind parameters */
    statement.bind(1, priv->firstName);
    statement.bind(2, priv->lastName);
    statement.bind(3, priv->telephone);
    //Param #4: See below
    if (priv->id > 0) // Only when explicitly setting ID
        statement.bind(5, priv->id);

    /* Bind postal address primary key */
    int primaryKey = 0;
    if (priv->postalAddress.isPrimaryKeySet())
        primaryKey = priv->postalAddress.getPrimaryKey();
    else if(priv->postalAddress.isLoaded())
        primaryKey = priv->postalAddress->getId();
    if (primaryKey == 0)
        throw NullPointerException();
    statement.bind(4, primaryKey);

    /* Execute */
    statement.step();
    if (priv->id <= 0) // Retrieve automatically generated ID
        priv->id = (int)getConnection()->lastInsertRowId();
}


int LibraryUser::getId() const {
    return priv->id;
}

string LibraryUser::getFirstName() const {
    return priv->firstName;
}

void LibraryUser::setFirstName(const string & firstName) {
    priv->firstName = firstName;
}

string LibraryUser::getLastName() const {
    return priv->lastName;
}

void LibraryUser::setLastName(const string & lastName) {
    priv->lastName = lastName;
}

string LibraryUser::getTelephone() const {
    return priv->telephone;
}

void LibraryUser::setTelephone(const string & telephone) {
    priv->telephone = telephone;
}

shared_ptr<PostalAddress> LibraryUser::getPostalAddress() const {
    return priv->postalAddress.get();
}

void LibraryUser::setPostalAddress(shared_ptr<PostalAddress> postalAddress) {
    priv->postalAddress.set(postalAddress);
}
