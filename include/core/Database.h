#ifndef PROG2_BELEG_DATABASE_H
#define PROG2_BELEG_DATABASE_H

#include <memory>
#include <map>
#include <string>

#include "core/sqlite/SqliteConnection.h"

namespace pb2 {
    class Database_priv;

    /**
     * TODO: Description
     */
    class Database : public std::enable_shared_from_this<Database> {
    private:
        std::unique_ptr<Database_priv> priv;

        Database(std::shared_ptr<SqliteConnection> connection);

    public:
        ~Database();

        /**
         * Initializes a database
         *
         * Note: If connection points to an already initialized database, this will fail
         * with AlreadyInitializedException.
         */
        static std::shared_ptr<Database> initialize(std::shared_ptr<SqliteConnection> connection);

        /**
         * Migrate a database to the current format version, then open database
         *
         * Returns a new Database object that represents the successfully
         * migrated database.
         * @param filename
         */
        static std::shared_ptr<Database> migrate(std::shared_ptr<SqliteConnection> connection);

        /**
         * Opens an existing database
         *
         * Note: This can fail due to an outdated Database version, indicated by
         * FormatVersionException.
         */
        static std::shared_ptr<Database> open(std::shared_ptr<SqliteConnection> connection);

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
        static int getFormatVersion(std::shared_ptr<SqliteConnection> connection);


        std::shared_ptr<SqliteConnection> getConnection() const;

        /**
         * Retrieve a map of all meta/configuration values stored in the current database
         *
         * Configuration values are stored in the database in the "meta" table as string
         * values.
         * Caution: The map returned by this call will be changed with every call to
         * refreshMeta(), this includes all calls that might be issued automatically
         * on commit or rollback.
         */
        const std::map<std::string,std::string> & getMeta() const;

        /**
         * Retrieves a meta/configuration string from the database.
         */
        std::string getMeta(const std::string & name) const;

        /**
         * Sets the specified meta value by issuing an UPDATE / INSERT statement
         * in a new transaction and instantly commiting this transaction.
         * This will create a new entry if the meta key at "name" does not exist.
         * Applications are advised to use the "c_" prefix plus an application-specific
         * prefix for custom meta value names.
         *
         * Caution: This will roll back any running transactions!
         * Caution: While technically possible, applications are advised to not overwrite
         * the 'version' meta value! For version updates use migrate()
         */
        void setMeta(const std::string & name, const std::string & value);
    };
}

#endif
