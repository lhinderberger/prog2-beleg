#ifndef PROG2_BELEG_DATABASE_H
#define PROG2_BELEG_DATABASE_H

#include <memory>
#include <string>

#include "SqlConnection.h"

namespace pb2 {
    class Database_priv;

    /**
     * TODO: Description
     */
    class Database : public std::enable_shared_from_this<Database> {
    private:
        std::unique_ptr<Database_priv> priv;

        Database(std::shared_ptr<SqlConnection> connection);

    public:
        /**
         * Initializes a database
         *
         * Note: If connection points to an already initialized database, this will fail
         * with AlreadyInitializedException.
         */
        static std::shared_ptr<Database> initialize(std::shared_ptr<SqlConnection> connection);

        /**
         * Migrate a database to the current format version, then open database
         *
         * Returns a new Database object that represents the successfully
         * migrated database.
         * @param filename
         */
        static std::shared_ptr<Database> migrate(std::shared_ptr<SqlConnection> connection);

        /**
         * Opens an existing database
         *
         * Note: This can fail due to an outdated Database version, indicated by
         * FormatVersionException.
         */
        static std::shared_ptr<Database> open(std::shared_ptr<SqlConnection> connection);

        /**
         * Returns the most recent database format version the currently loaded library
         * can handle
         * @return A positive format version
         */
        static int getCurrentFormatVersion();

        /**
         * Returns the database format version of the database file specified by
         * filename.
         * @return A positive format version
         */
        static int getFormatVersion(std::shared_ptr<SqlConnection> connection);

        std::shared_ptr<SqlConnection> getConnection() const;
    };
}

#endif
