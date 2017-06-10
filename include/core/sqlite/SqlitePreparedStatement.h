#ifndef PROG2_BELEG_SQL_PREPARED_STATEMENT_H
#define PROG2_BELEG_SQL_PREPARED_STATEMENT_H

#include <memory>
#include <map>
#include <string>

#include "SqliteConnection.h"

namespace pb2 {
    class SqlitePreparedStatement_priv;

    class SqlitePreparedStatement {
    private:
        std::unique_ptr<SqlitePreparedStatement_priv> priv;

    public:
        SqlitePreparedStatement(std::shared_ptr<SqliteConnection> connection, const std::string & sql);
        ~SqlitePreparedStatement();

        /**
         * Execute the prepared statement / retrieve another row of data.
         * Use the column* methods to retrieve column values.
         * @return True while the statement remains valid (row obtained / executed).
         * False when the last row was retrieved (Throws on error).
         */
        bool step();

        /**
         * Resets the statement so that it can be executed again.
         * Note: This does not reset the SQL command(s) and parameter bindings!
         */
        void reset();

        /**
         * Binds a blob value to a parameter.
         * The value passed in will be copied, so the value pointer only has to stay
         * valid during the execution of this method.
         */
        void bindBlob(int paramIndex, void * value, int valueBytes);

        /**
         * Binds an integer value to a parameter.
         */
        void bindInt(int paramIndex, int value);

        /**
         * Binds a Null value to a parameter.
         */
        void bindNull(int paramIndex);

        /**
         * Binds a String value to a parameter.
         * The string passed in will be copied.
         * @param paramIndex
         * @param value
         */
        void bindString(int paramIndex, const std::string & value);

        /**
         * Retrieve a Blob value from the current result row, either by its index or by
         * its full column name.
         *
         * @param bytesOut Outgoing. The number of bytes in the result.
         * @return An array of bytes, which has to be instantly evaluated, as it will
         * be freed or overwritten by subsequent calls to this API.
         */
        const void * columnBlob(int columnIndex, int * bytesOut);
        const void * columnBlob(const std::string & fullColumnName, int * bytesOut);

        /**
         * Retrieve an Integer value from the current result row, either by its index or by
         * its full column name.
         */
        int columnInt(int columnIndex);
        int columnInt(const std::string & fullColumnName);

        /**
         * Retrieve a String value from the current result row, either by its index or by
         * its full column name.
         */
        std::string columnString(int columnIndex);
        std::string columnString(const std::string & fullColumnName);

        /**
         * Returns a map of column indexes, identified by their column name (as returned
         * by SQLite).
         * This is built for the frequent use case of querying for columns by their name
         * rather than by their position in the result set.
         * For example, this is used by DatabaseObject for loading objects from the
         * database.
         */
        const std::map<std::string, int> & columnIndexes();

        /**
         * Returns the number of columns in this query.
         */
        int getColumnCount();

        /**
         * Try to retrieve the column index for a given full column name.
         * Will throw ColumnNotFoundException if the given fullColumnName could not be
         * found.
         */
        int getColumnIndex(const std::string & fullColumnName);

        /**
         * Returns the full column name of the result column identified with the given
         * index.
         *
         * The full column name consists of the table name, followed by a dot, followed
         * by the original column name or - if an expression / subquery result - the
         * column alias preceded by a dot.
         *
         * For example:
         *
         * SELECT id, value AS valueAlias, COUNT(*), COUNT(*) AS bla FROM test as testi
         *
         * Yields the following column Names:
         * 0: test.id
         * 1: test.value
         * 2: .COUNT(*)
         * 3: .bla
         */
        std::string getFullColumnName(int columnIndex);

        std::shared_ptr<SqliteConnection> getConnection() const;
    };
}

#endif
