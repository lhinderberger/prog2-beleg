#include "core/sqlite/SqliteConnection.h"
#include "core/sqlite/SqliteConnection.priv.h"
#include "core/exceptions.h"

#include <fstream>
#include <iostream>

using namespace std;
using namespace pb2;

const string inMemoryDbFilename = ":memory:";


bool fileExists(const string & filename) {
    return ifstream(filename).good();
}

void closeConnection(sqlite3 * db) {
    if (db) {
        int res = sqlite3_close(db);
        if (res != SQLITE_OK)
            cerr << "Warning: Error while closing Sqlite connection!" << endl;
    }
}

SqliteConnection::SqliteConnection(const std::string &filename, bool create) {
    priv = make_unique<SqliteConnection_priv>();

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

SqliteConnection::~SqliteConnection() {
    /* Try to close connection, throw on error */
    closeConnection(priv->connection);
}

shared_ptr<SqliteConnection> SqliteConnection::construct(const string & filename, bool create) {
    return shared_ptr<SqliteConnection>(new SqliteConnection(filename, create));
}


SqliteException SqliteConnection::buildException() {
    return SqliteException(priv->connection);
}

bool SqliteConnection::isTransactionActive() const {
    return sqlite3_get_autocommit(priv->connection) == 0;
}

void SqliteConnection::commit() {
    lock_guard<mutex> guard(getWriteLock());
    executeSQL("COMMIT; BEGIN;");
}

void SqliteConnection::rollback() {
    executeSQL("ROLLBACK; BEGIN;");
}

void SqliteConnection::executeSQL(const string & query) {
    if (sqlite3_exec(priv->connection, query.c_str(), nullptr, nullptr, nullptr))
        throw buildException();
}

sqlite3_int64 SqliteConnection::lastInsertRowId() {
    lock_guard<mutex> guard(getWriteLock());
    return sqlite3_last_insert_rowid(priv->connection);
}

std::mutex& SqliteConnection::getWriteLock() const {
    return priv->writeLock;
}
