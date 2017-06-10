#ifndef PROG2_BELEG_DATABASE_OBJECT_FACTORY_H
#define PROG2_BELEG_DATABASE_OBJECT_FACTORY_H

#include "DatabaseObject.h"
#include "Database.h"

#include <algorithm>
#include <memory>
#include <string>
#include <map>

#define PB2_DECLARE_LOAD_CONSTRUCTOR(T) T(std::shared_ptr<Database> database, SqlitePreparedStatement & query, const std::map<std::string, int> & columnIndexes)

namespace pb2 {
/**
     * Base factory class for DatabaseObjects
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
         * Implementing factories shall call the load constructor of the concrete
         * DatabaseObject type.
         */
        virtual std::shared_ptr<DatabaseObject> constructLoadImpl(
                SqlitePreparedStatement & query,
                const std::map<std::string, int> & columnIndexes
        ) = 0;

        /** Shall return the table name of the database object to be constructed */
        virtual const std::string & getTableName() const = 0;

        /* Called internally */
        std::map<std::string, int> buildColumnIndexes(
                SqlitePreparedStatement & query,
                const std::map<std::string, std::string> & alternativeColumnNames
        );

        std::shared_ptr<DatabaseObject> constructLoad(
                SqlitePreparedStatement & query,
                const std::map<std::string, int> & columnIndexes
        );

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
        std::shared_ptr<DatabaseObject> baseLoad(
                SqlitePreparedStatement & query,
                const std::map<std::string, std::string> & alternativeColumnNames
                = std::map<std::string, std::string>()
        );

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
        std::vector<std::shared_ptr<DatabaseObject>> baseLoadMany(
                SqlitePreparedStatement & query, int n,
                const std::map<std::string, std::string> & alternativeColumnNames
                = std::map<std::string, std::string>()
        );
    };


    /**
     * This template provides a default go-to place for DatabaseObject construction,
     * which should serve nearly all use cases without the need of creating a
     * custom factory class for each DatabaseObject.
     *
     * It can construct DatabaseObjects in a generic manner (using all available
     * constructors) as well as load them from arbitrary queries.
     */
    template<class ConcreteDatabaseObject>
    class DatabaseObjectFactory : public AbstractDatabaseObjectFactory {
        static_assert(std::is_base_of<DatabaseObject, ConcreteDatabaseObject>::value,
                      "ManyToOneLazyLoader can only handle subclasses of DatabaseObject!");
    protected:
        /**
         * Call load constructor of concrete DatabaseObject
         */
        virtual std::shared_ptr<DatabaseObject> constructLoadImpl(
                SqlitePreparedStatement & query,
                const std::map<std::string, int> & columnIndexes
        ) override {
            return std::shared_ptr<ConcreteDatabaseObject>(
                    new ConcreteDatabaseObject(database, query, columnIndexes)
            );
        }

        virtual const std::string & getTableName() const override {
            return ConcreteDatabaseObject::tableName;
        }

    public:
        DatabaseObjectFactory(std::shared_ptr<Database> database)
                : AbstractDatabaseObjectFactory(database) {}

        /**
         * Generic factory function exposing private constructors as shared_ptr-only
         * public version.
         */
        template<typename ...ConstructorArgs>
        std::shared_ptr<ConcreteDatabaseObject> construct(ConstructorArgs... args) {
            return std::shared_ptr<ConcreteDatabaseObject>(new ConcreteDatabaseObject(database, args...));
        }

        /**
         * See the documentation for AbstractDatabaseObjectFactory::baseLoad
         */
        std::shared_ptr<ConcreteDatabaseObject> load(
                SqlitePreparedStatement & query,
                const std::map<std::string, std::string> & alternativeColumnNames
                = std::map<std::string, std::string>()
        ) {
            // Call AbstractDatabaseObjectFactory implementation and cast down shared pointer
            return std::static_pointer_cast<ConcreteDatabaseObject>(baseLoad(query, alternativeColumnNames));
        }
        /**
         * See the documentation for AbstractDatabaseObjectFactory::baseLoadMany
         */
        std::vector<std::shared_ptr<ConcreteDatabaseObject>> loadMany(
                SqlitePreparedStatement & query, int n,
                const std::map<std::string, std::string> & alternativeColumnNames
                = std::map<std::string, std::string>()
        ) {
            // Do static pointer cast on each element of the result of the
            // AbstractDatabaseObjectFactory implementation
            auto baseResult = baseLoadMany(query, n, alternativeColumnNames);
            auto result = std::vector<std::shared_ptr<ConcreteDatabaseObject>>();
            std::transform(baseResult.cbegin(), baseResult.cend(), result.begin(), std::static_pointer_cast<ConcreteDatabaseObject>);
            return result;
        }
    };
}

#endif
