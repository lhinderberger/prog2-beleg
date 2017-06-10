#ifndef PROG2_BELEG_DATABASE_OBJECT_H
#define PROG2_BELEG_DATABASE_OBJECT_H

#include <memory>
#include <map>
#include <string>
#include <vector>

#include "core/sqlite/SqlitePreparedStatement.h"

namespace pb2 {
    class Database;
    class DatabaseObject_priv;

    /**
     * Base class for all objects that are part of the Database and thus
     * can be loaded from and persisted to the database.
     *
     * Implementing concrete classes are advised to contain:
     *
     * - A load constructor, having the same parameters as
     * AbstractDatabaseObjectFactory::constructLoadImpl in order to ensure functioning with
     * the default DatabaseObjectFactory. That load constructor may be (and should be)
     * private. This in turn means that implementing classes should friend-declare
     * DatabaseObjectFactory.
     *
     * - A const static string containing the SQL table name
     */
    class DatabaseObject : public std::enable_shared_from_this<DatabaseObject> {
        friend class AbstractDatabaseObjectFactory;

    private:
        std::unique_ptr<DatabaseObject_priv> priv;

    protected:
        DatabaseObject(std::shared_ptr<Database> database);

        virtual void persistImpl() = 0;

    public:
        virtual ~DatabaseObject();

        /**
         * Persists the DatabaseObject to the database.
         *
         * Warning: This operation will throw logic_error if no transaction is running
         * instead of silently accepting and committing the database object, potentially
         * ruining referential integrity.
         *
         * Warning: This operation is not thread-safe. Be sure no other thread accesses
         * the database connection while this operation is running!
         *
         * TODO: Make this thread-safe by acquiring a lock on the database connection.
         */
        void persist();


        // Getters
        const std::shared_ptr<Database> getDatabase() const;
        const std::shared_ptr<SqliteConnection> getConnection() const;

        /**
         * Returns true if the object was loaded from database instead of constructed
         * at runtime.
         */
        bool isLoaded() const;
    };
}

#include "Database.h"

#endif
