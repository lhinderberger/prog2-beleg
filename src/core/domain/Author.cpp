#include "core/domain/Author.h"
#include "core/domain/Author.priv.h"
#include "core/exceptions.h"
#include "core/query-builder.h"

using namespace std;
using namespace pb2;

const string Author::tableName = "authors";

Author::Author(shared_ptr<Database> database, int id) : DatabaseObject(database) {
    priv = make_unique<Author_priv>();
    priv->id = id;
}

Author::Author(
        shared_ptr<Database> database, SqlitePreparedStatement & query,
        const map<string, int> & columnIndexes
) : DatabaseObject(database) {
    priv = make_unique<Author_priv>();

    //TODO: Is there a way to overload this constructor?
    //TODO: Use template or macro?
    priv->id = query.columnInt(columnIndexes.at("id"));
    priv->firstName = query.columnString(columnIndexes.at("first_name"));
    priv->lastName = query.columnString(columnIndexes.at("last_name"));
}

Author::~Author() = default;


void Author::del() {
    SqlitePreparedStatement query(getConnection(), "DELETE FROM " + tableName + " WHERE id = ?");
    query.bind(1, priv->id);
    query.step();
}

void Author::persistImpl() {
    /* Prepare statement */
    string query = "";
    if (isLoaded())
        query = buildUpdateQuery<Author>({"first_name", "last_name"}, "WHERE id=?");
    else {
        /* Auto-generate ID? */
        if (priv->id <= 0)
            query = buildInsertQuery<Author>({"first_name", "last_name"}, 1);
        else
            query = buildInsertQuery<Author>({"first_name", "last_name", "id"}, 1);
    }
    SqlitePreparedStatement statement(getConnection(), query);

    /* Bind parameters */
    statement.bind(1, priv->firstName);
    statement.bind(2, priv->lastName);
    if (priv->id > 0)
        statement.bind(3, priv->id);

    /* Execute */
    statement.step();

    /* Retrieve ID */
    if (priv->id <= 0)
        priv->id = (int)getConnection()->lastInsertRowId();
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
