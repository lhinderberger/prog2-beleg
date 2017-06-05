#ifndef PROG2_BELEG_SQL_PREPARED_STATEMENT_H
#define PROG2_BELEG_SQL_PREPARED_STATEMENT_H

#include <memory>

#include "SqlConnection.h"

namespace pb2 {
    class SqlPreparedStatement_priv;

    class SqlPreparedStatement {
    private:
        std::unique_ptr<SqlPreparedStatement_priv> priv;

    public:
        SqlPreparedStatement(std::shared_ptr<SqlConnection> connection, const std::string & sql);
        ~SqlPreparedStatement();

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
         * Retrieve a Blob value from the current result row.
         * @param bytesOut Outgoing. The number of bytes in the result.
         * @return An array of bytes, which has to be instantly evaluated, as it will
         * be freed or overwritten by subsequent calls to this API.
         */
        const void * columnBlob(int columnIndex, int * bytesOut);

        /**
         * Retrieve an Integer value from the current result row
         */
        int columnInt(int columnIndex);

        /**
         * Retrieve a String value from the current result row.
         */
        std::string columnString(int columnIndex);

        std::shared_ptr<SqlConnection> getConnection() const;
    };
}

#endif
