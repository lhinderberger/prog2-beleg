#ifndef PROG2_BELEG_SQL_CONNECTION_H
#define PROG2_BELEG_SQL_CONNECTION_H

#include <memory>
#include <string>

#include "core/exceptions.h"

namespace pb2 {
    class SqliteConnection_priv;
    class SqlitePreparedStatement;

    /**
     * SqliteConnection is a thin wrapper around SQLite for easier usage with C++
     * Parts of its concepts are being directly adapted from prog1-beleg.
     */
    class SqliteConnection : public std::enable_shared_from_this<SqliteConnection> {
        friend class SqlitePreparedStatement;

    private:
        std::unique_ptr<SqliteConnection_priv> priv;

    protected:
        SqliteConnection(const std::string & filename, bool create);

    public:
        /**
         * Opens or creates a SQL database
         *
         * Note: A transaction will be started immediately after opening the database.
         * By default, all statements are always part of a transaction. This can be
         * manually overridden by not using commit() and issuing transaction controlling
         * SQL via executeSQL().
         *
         * Note: Foreign key constraints will be enforced by default, unlike the SQLite
         * default settings.
         *
         * @param filename The unambiguous filename of the database, i.e. which database
         * shall be opened or under which name it shall be created
         * @param create If this is false, the file specified by filename MUST exist and
         * be a valid database file. Otherwise the target file MUST not exist and a new
         * database will be created.
         */
        static std::shared_ptr<SqliteConnection> construct(const std::string & filename, bool create);

        ~SqliteConnection();

        /**
         * Checks whether a transaction is running. Unless you manually mess around with
         * transaction control statements using executeSQL() this should always return
         * true.
         */
        bool isTransactionActive() const;

        /**
         * Commits the current transaction and begins a new one.
         */
        virtual void commit();

        /**
         * Rolls back the current transaction, basically taking back all SQL commands
         * that have not been commited yet.
         */
        virtual void rollback();

        /**
         * Executes the given SQL command.
         * Useful as a short-cut for issuing DDL commands, not so much for
         * DQL commands - use prepared statements instead.
         *
         * CAUTION: Do not use this for executing SQL based on user input - use prepared
         * statements instead. Otherwise you risk being vulnerable to SQL injection!
         */
        virtual void executeSQL(const std::string & query);

        /**
         * Constructs a SqliteException from the current error state of the underlying
         * Sqlite connection.
         * @return
         */
        SqliteException buildException();

        /**
         * Returns the row ID of the last insert (i.e. for auto-generation of primary keys).
         */
        sqlite3_int64 lastInsertRowId();
    };
}

#include "SqlitePreparedStatement.h"

#endif
