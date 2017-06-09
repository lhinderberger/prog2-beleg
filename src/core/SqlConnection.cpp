#include "core/SqlConnection.h"
#include "core/SqlConnection.priv.h"
#include "core/exceptions.h"

#include <fstream>

using namespace std;
using namespace pb2;

const string inMemoryDbFilename = ":memory:";


bool fileExists(const string & filename) {
    return ifstream(filename).good();
}

void closeConnection(sqlite3 * db) {
    //TODO: Throw from destructor seems like a bad idea...
    if (db) {
        int res = sqlite3_close(db);
        if (res != SQLITE_OK)
            throw SqliteException(res);
    }
}

SqlConnection::SqlConnection(const std::string &filename, bool create) {
    priv = make_unique<SqlConnection_priv>();

    /* Check file exists constraints */
    bool exists = (filename != inMemoryDbFilename) && fileExists(filename);
    if ((exists && create) || (!exists && !create))
        throw FileExistsException(filename);

    /* Open SQLite database */
    int res = sqlite3_open_v2(
            filename.c_str(),
            &priv->connection,
            SQLITE_OPEN_READWRITE | (create ? SQLITE_OPEN_CREATE : 0),
            NULL
    );
    if (res != SQLITE_OK)
        throw SqliteException(res);


    try {
        /* Enable extended result codes */
        sqlite3_extended_result_codes(priv->connection, 1);

        /* Enable foreign keys */
        executeSQL("PRAGMA foreign_keys = ON;");

        /* Begin transaction */
        executeSQL("BEGIN;"); // bring it on!
    }
    catch(...) {
        closeConnection(priv->connection);
        throw;
    }
}

SqlConnection::~SqlConnection() {
    /* Try to close connection, throw on error */
    closeConnection(priv->connection);
}

shared_ptr<SqlConnection> SqlConnection::construct(const string & filename, bool create) {
    return shared_ptr<SqlConnection>(new SqlConnection(filename, create));
}


SqliteException SqlConnection::buildException() {
    return SqliteException(priv->connection);
}

bool SqlConnection::isTransactionActive() const {
    return sqlite3_get_autocommit(priv->connection) == 0;
}

void SqlConnection::commit() {
    executeSQL("COMMIT; BEGIN;");
}

void SqlConnection::rollback() {
    executeSQL("ROLLBACK; BEGIN;");
}

void SqlConnection::executeSQL(const string & query) {
    if (sqlite3_exec(priv->connection, query.c_str(), nullptr, nullptr, nullptr))
        throw buildException();
}
