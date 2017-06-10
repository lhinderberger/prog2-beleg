#include "core/Author.h"
#include "core/Author.priv.h"
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
        shared_ptr<Database> database, SqlPreparedStatement & query,
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


void Author::persistImpl() {
    /* Prepare statement */
    SqlPreparedStatement statement(getConnection(), isLoaded() ?
        buildUpdateQuery<Author>({"first_name", "last_name"}, "WHERE id=?") :
        buildInsertQuery<Author>({"first_name", "last_name", "id"}, 1)
    );

    /* Bind parameters */
    statement.bindString(1, priv->firstName);
    statement.bindString(2, priv->lastName);
    statement.bindInt(3, priv->id);

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
