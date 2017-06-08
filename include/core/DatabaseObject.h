#ifndef PROG2_BELEG_DATABASE_OBJECT_H
#define PROG2_BELEG_DATABASE_OBJECT_H

#include <memory>
#include <map>
#include <string>

#include "SqlPreparedStatement.h"

namespace pb2 {
    class Database;

    class DatabaseObject : public std::enable_shared_from_this<DatabaseObject> {
    private:
        std::shared_ptr<Database> database;

    protected:
        DatabaseObject(std::shared_ptr<Database> database);

    public:
        virtual ~DatabaseObject();

        inline const std::shared_ptr<Database> getDatabase() const { return database; }

        /**
         * Returns the SQL table name of this DatabaseObject.
         */
        virtual const std::string & getTableName() const = 0;

        /**
         * Lower-case class name of the concrete database object
         */
        virtual const std::string & getType() const = 0;

        /**
         * Loads an instance of the object from the current row retrieved by the given
         * SqlPreparedStatement.
         * Columns will be queried by their full column name (see SqlPreparedStatement
         * documentation), unless overridden by alternativeColumnNames.
         *
         * @param query
         * @param alternativeColumnNames
         */
        virtual void load(SqlPreparedStatement & query,
            const std::map<std::string, std::string> & alternativeColumnNames
            = std::map<std::string, std::string>()) = 0;

        /**
         * Persists the DatabaseObject to the database.
         */
        virtual void persist() = 0;
    };
}

#include "Database.h"

#endif
