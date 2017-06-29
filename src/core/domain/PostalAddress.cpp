#include "core/domain/PostalAddress.h"
#include "core/domain/PostalAddress.priv.h"
#include "core/query-builder.h"

using namespace std;
using namespace pb2;

const string PostalAddress::tableName = "postal_addresses";

PostalAddress::PostalAddress(shared_ptr<Database> database, int id)
    : DatabaseObject(database) {
    priv = make_unique<PostalAddress_priv>();

    priv->id = id;
}

PostalAddress::PostalAddress(
        shared_ptr<Database> database, SqlitePreparedStatement & query,
        const map<string, int> & columnIndexes
) : DatabaseObject(database) {
    priv = make_unique<PostalAddress_priv>();

    priv->id = query.columnInt(columnIndexes.at("id"));
    priv->street = query.columnString(columnIndexes.at("street"));
    priv->houseNumber= query.columnString(columnIndexes.at("house_number"));
    priv->addition = query.columnString(columnIndexes.at("addition"));
    priv->zip = query.columnString(columnIndexes.at("zip"));
    priv->city = query.columnString(columnIndexes.at("city"));
}

PostalAddress::~PostalAddress() = default;


void PostalAddress::del() {
    SqlitePreparedStatement query(getConnection(), "DELETE FROM " + tableName + " WHERE id = ?");
    query.bind(1, priv->id);
    query.step();
}

void PostalAddress::persistImpl() {
    /* Prepare statement */
    vector<string> columns = {"street", "house_number", "addition", "zip", "city"};
    if (!isLoaded() && priv->id > 0)
        columns.push_back("id");

    SqlitePreparedStatement statement(getConnection(), isLoaded() ?
        buildUpdateQuery<PostalAddress>(columns, "WHERE id=?") :
        buildInsertQuery<PostalAddress>(columns, 1)
    );

    /* Bind parameters */
    statement.bind(1, priv->street);
    statement.bind(2, priv->houseNumber);
    statement.bind(3, priv->addition);
    statement.bind(4, priv->zip);
    statement.bind(5, priv->city);
    if (priv->id > 0) // Only when explicitly setting ID
        statement.bind(6, priv->id);

    /* Execute */
    statement.step();
    if (priv->id <= 0) // Retrieve automatically generated ID
        priv->id = (int)getConnection()->lastInsertRowId();
}

int PostalAddress::getId() const {
    return priv->id;
}

string PostalAddress::getStreet() const {
    return priv->street;
}

void PostalAddress::setStreet(const string & street) {
    priv->street = street;
}

string PostalAddress::getHouseNumber() const {
    return priv->houseNumber;
}

void PostalAddress::setHouseNumber(const string & houseNumber) {
    priv->houseNumber = houseNumber;
}

string PostalAddress::getAddition() const {
    return priv->addition;
}

void PostalAddress::setAddition(const string & addition) {
    priv->addition = addition;
}

string PostalAddress::getZip() const {
    return priv->zip;
}

void PostalAddress::setZip(const string & zip) {
    priv->zip = zip;
}

string PostalAddress::getCity() const {
    return priv->city;
}

void PostalAddress::setCity(const string & city) {
    priv->city = city;
}
