#ifndef PROG2_BELEG_ABSTRACT_DATABASE_OBJECT_FACTORY_H
#define PROG2_BELEG_ABSTRACT_DATABASE_OBJECT_FACTORY_H

#include <map>
#include <memory>
#include <string>

#include "DatabaseObject.h"
#include "Database.h"

namespace pb2 {
    /**
     * Abstract factory class for DatabaseObjects
     *
     * ORM and querying can be complicated, so constructing DatabaseObjects must be
     * flexible.
     * This abstract factory class and its concrete implementations provide flexibility in
     * creating / loading DatabaseObjects.
     */
    class AbstractDatabaseObjectFactory {
    protected:
        std::shared_ptr<Database> database;

        /**
         * Helper function that can build column indexes for use in load constructors from
         * a query and provided alternative column names.
         */
        std::map<std::string, int> buildColumnIndexes(
                SqlitePreparedStatement & query,
                const std::map<std::string, std::string> & alternativeColumnNames,
                const std::string & tableName
        );

        void setLoaded(std::shared_ptr<DatabaseObject> dbObject, bool loaded);

    public:
        AbstractDatabaseObjectFactory(std::shared_ptr<Database> & database);
        virtual ~AbstractDatabaseObjectFactory();

        /**
         * Loads one instance of the object from the current row retrieved by the given
         * SqlitePreparedStatement.
         * Columns will be queried by their full column name (see SqlitePreparedStatement
         * documentation), unless overridden by alternativeColumnNames.
         *
         * @param alternativeColumnNames Maps full column names of object columns to
         * alias names for the query. For example, if an object has the column test.name
         * and the query might have a column .combined_name, the map would contain the
         * pair ("test.name", ".combined_name").
         */
        virtual std::shared_ptr<DatabaseObject> abstractLoad(
                SqlitePreparedStatement & query,
                const std::map<std::string, std::string> & alternativeColumnNames
                = std::map<std::string, std::string>()
        ) = 0;

        virtual std::shared_ptr<DatabaseObject> abstractLoad(
                SqlitePreparedStatement & query,
                const std::map<std::string, int> & columnIndexes
        ) = 0;

        /**
         * Loads multiple instances of the object as in load().
         *
         * Note: This function will fail gracefully when less than n items can be retrieved
         * due to a lack of rows in the query result. In this case, this function will
         * simply return less than n objects of the query result. So be sure to check the
         * size of the returned vector to be sure exactly n objects really have been loaded.
         *
         * @param n Pass in a negative value to load all remaining rows. Otherwise pass in
         * a number of rows to be read.
         */
        virtual std::vector<std::shared_ptr<DatabaseObject>> abstractLoadMany(
                SqlitePreparedStatement & query, int n,
                const std::map<std::string, std::string> & alternativeColumnNames
                = std::map<std::string, std::string>()
        ) = 0;

        virtual std::vector<std::shared_ptr<DatabaseObject>> abstractLoadMany(
                SqlitePreparedStatement & query, int n,
                const std::map<std::string, int> & columnIndexes
        ) = 0;
    };
}
#endif