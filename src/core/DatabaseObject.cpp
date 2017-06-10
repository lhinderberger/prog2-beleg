#include "core/DatabaseObject.h"
#include "core/exceptions.h"
#include "core/DatabaseObject.priv.h"

using namespace std;
using namespace pb2;


DatabaseObject::DatabaseObject(std::shared_ptr<Database> database) {
    if (!database)
        throw NullPointerException();

    priv = make_unique<DatabaseObject_priv>();
    priv->database = database;
}

DatabaseObject::~DatabaseObject() = default;


void DatabaseObject::persist() {
    /* Make sure we only persist when a transaction is active */
    if (!priv->database->getConnection()->isTransactionActive())
        throw logic_error("DatabaseObjects cannot be persisted without an active transaction!");

    /* Call persist implementation */
    persistImpl();
}

const shared_ptr<SqliteConnection> DatabaseObject::getConnection() const {
    return priv->database->getConnection();
}

const shared_ptr<Database> DatabaseObject::getDatabase() const {
    return priv->database;
}

bool DatabaseObject::isLoaded() const {
    return priv->loaded;
}
