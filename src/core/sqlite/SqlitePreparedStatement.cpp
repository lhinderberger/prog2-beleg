#include "core/sqlite/SqlitePreparedStatement.h"
#include "core/sqlite/SqlitePreparedStatement.priv.h"
#include "core/sqlite/SqliteConnection.priv.h"
#include "core/exceptions.h"

#include <climits>
#include <iostream>

using namespace std;
using namespace pb2;

SqlitePreparedStatement::SqlitePreparedStatement(
        shared_ptr<SqliteConnection> connection, const string & sql
) {
    if (!connection)
        throw NullPointerException();

    priv = make_unique<SqlitePreparedStatement_priv>();
    priv->connection = connection;

    /* Get statement size */
    auto statementSize = sql.size();
    if (statementSize > INT_MAX)
        throw StringTooLongException();

    /* Prepare statement */
    if (sqlite3_prepare_v2(
            connection->priv->connection,
            sql.c_str(),
            (int)sql.size(),
            &priv->statement,
            NULL
    ))
        throw connection->buildException();
}

SqlitePreparedStatement::~SqlitePreparedStatement() {
    if (priv->statement) {
        int res = sqlite3_finalize(priv->statement);
        if (res != SQLITE_OK)
            cerr << "Warning: Could not finalize prepared statement!" << endl;
    }
}

bool SqlitePreparedStatement::step() {
    int res = sqlite3_step(priv->statement);
    if (res == SQLITE_OK || res == SQLITE_ROW) // Row obtained, statement still valid
        return true;
    else if (res == SQLITE_DONE)
        return false;
    else
        throw priv->connection->buildException();
}

void SqlitePreparedStatement::reset() {
    if (sqlite3_reset(priv->statement) != SQLITE_OK)
        throw priv->connection->buildException();
}

void SqlitePreparedStatement::bind(int paramIndex, int value) {
    if (sqlite3_bind_int(priv->statement, paramIndex, value) != SQLITE_OK)
        throw priv->connection->buildException();
}

void SqlitePreparedStatement::bind(int paramIndex) {
    if (sqlite3_bind_null(priv->statement, paramIndex) != SQLITE_OK)
        throw priv->connection->buildException();
}

void SqlitePreparedStatement::bind(int paramIndex, const string & value) {
    auto valSize = value.size();
    if (valSize > INT_MAX)
        throw StringTooLongException();

    if (sqlite3_bind_text(priv->statement, paramIndex, value.c_str(), (int)value.size(), NULL) != SQLITE_OK)
        throw priv->connection->buildException();
}

bool SqlitePreparedStatement::columnIsNull(int columnIndex) {
    return sqlite3_column_type(priv->statement, columnIndex) == SQLITE_NULL;
}

bool SqlitePreparedStatement::columnIsNull(const std::string & fullColumnName) {
    return columnIsNull(getColumnIndex(fullColumnName));
}

int SqlitePreparedStatement::columnInt(int columnIndex) {
    int result = sqlite3_column_int(priv->statement, columnIndex);

    int errcode = sqlite3_errcode(priv->connection->priv->connection);
    if (!result && errcode != SQLITE_OK && errcode != SQLITE_ROW)
        throw priv->connection->buildException();
    else if (!result && columnIsNull(columnIndex))
        throw DatabaseIntegrityException("Column is NULL!");

    return result;
}

int SqlitePreparedStatement::columnInt(const string & fullColumnName) {
    return columnInt(getColumnIndex(fullColumnName));
}

string SqlitePreparedStatement::columnString(int columnIndex) {
    auto column = (const char *)sqlite3_column_text(priv->statement, columnIndex);
    
    int errcode = sqlite3_errcode(priv->connection->priv->connection);
    if (!column && errcode != SQLITE_OK && errcode != SQLITE_ROW)
        throw priv->connection->buildException();
    else if (!column && columnIsNull(columnIndex))
        throw DatabaseIntegrityException("Column is NULL!");

    return string(column);
}

string SqlitePreparedStatement::columnString(const string & fullColumnName) {
    return columnString(getColumnIndex(fullColumnName));
}

const map<string, int> & SqlitePreparedStatement::columnIndexes() {
    /* Build map on first call (lazy loading) */
    if (!priv->columnIndexes) {
        /* Allocate map */
        priv->columnIndexes = make_unique<map<string,int>>();

        /* Iterate over columns, build map */
        auto columnCount = getColumnCount();
        for (int i=0; i < columnCount; i++)
            priv->columnIndexes->insert({getFullColumnName(i), i});
    }

    /* Return reference to map */
    return *(priv->columnIndexes);
}

int SqlitePreparedStatement::getColumnCount() {
    int columnCount = sqlite3_column_count(priv->statement);
    if (columnCount < 0)
        throw logic_error("Column count shouldn't drop below zero!");

    return columnCount;
}

int SqlitePreparedStatement::getColumnIndex(const string & fullColumnName) {
    auto indexes = columnIndexes();
    auto it = indexes.find(fullColumnName);
    if (it == indexes.end())
        throw ColumnNotFoundException(fullColumnName);
    return it->second;
}

string SqlitePreparedStatement::getFullColumnName(int columnIndex) {
    auto db = priv->connection->priv->connection;

    /* Try to retrieve column origin name (original table name + original column name) */
    const char * tableName = sqlite3_column_table_name(priv->statement, columnIndex);
    if (!tableName && sqlite3_errcode(db) != SQLITE_OK)
        throw priv->connection->buildException();
    const char * columnName = sqlite3_column_origin_name(priv->statement, columnIndex);
    if (!columnName && sqlite3_errcode(db) != SQLITE_OK)
        throw priv->connection->buildException();

    /* If the column is an expression / subquery result it won't have an origin name. Use
     * alias name instead.
     */
    if (!columnName) {
        tableName = "";
        columnName = sqlite3_column_name(priv->statement, columnIndex);
        if (!columnName)
            throw priv->connection->buildException();
    }

    /* Build full name and return */
    return string(tableName) + '.' + columnName;
}

shared_ptr<SqliteConnection> SqlitePreparedStatement::getConnection() const {
    return priv->connection;
}
