#include "core/Author.h"
#include "core/Author.priv.h"
#include "core/exceptions.h"

using namespace std;
using namespace pb2;

Author::Author(shared_ptr<Database> database, int id) : DatabaseObject(database) {
    priv = make_unique<Author_priv>();
    priv->id = id;
}

shared_ptr<Author> Author::construct(shared_ptr<Database> database, int id) {
    return shared_ptr<Author>(new Author(database, id));
}

const string & Author::getTableName() const {
    static string t("author");
    return t;
}

const std::string & Author::getType() const {
    static string t("author");
    return t;
}


void Author::loadImpl(SqlPreparedStatement & query, const map<string, int> & columnIndexes) {
    //TODO: Use template or macro
    priv->id = query.columnInt(columnIndexes.at("id"));
    priv->firstName = query.columnString(columnIndexes.at("firstName"));
    priv->lastName = query.columnString(columnIndexes.at("lastName"));
}

void Author::persistImpl() {
    /* Prepare statement */
    SqlPreparedStatement statement(getConnection(), isLoaded() ?
        buildUpdateQuery({"firstName", "lastName"}, "WHERE id=?") :
        buildInsertQuery({"firstName", "lastName", "id"}, 1)
    );

    /* Bind parameters */
    statement.bindString(0, priv->firstName);
    statement.bindString(1, priv->lastName);
    statement.bindInt(2, priv->id);

    /* Execute */
    statement.step();
}

int Author::getId() const {
    return priv->id;
}

string Author::getFirstName() const {
    return priv->firstName;
}

void Author::setFirstName(const string & firstName) {
    priv->firstName = firstName;
}

string Author::getLastName() const {
    return priv->lastName;
}

void Author::setLastName(const string & lastName) {
    priv->lastName = lastName;
}
