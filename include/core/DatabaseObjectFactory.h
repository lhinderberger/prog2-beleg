#ifndef PROG2_BELEG_DATABASE_OBJECT_FACTORY_H
#define PROG2_BELEG_DATABASE_OBJECT_FACTORY_H

#include "DatabaseObject.h"
#include "Database.h"

#include <memory>
#include <string>
#include <map>

#define PB2_DECLARE_LOAD_CONSTRUCTOR(T) T(std::shared_ptr<Database> database, SqlitePreparedStatement & query, const std::map<std::string, int> & columnIndexes)

//TODO: Maybe break down in multiple files ; certainly document design decisions

namespace pb2 {
    template <typename SourcePointerType, typename TargetPointerType>
    auto castSharedPtrVector(
            const std::vector<std::shared_ptr<SourcePointerType>> & container
    ) {
        auto result = std::vector<std::shared_ptr<TargetPointerType>>();
        for (const auto & ptr : container)
            result.emplace_back(std::static_pointer_cast<TargetPointerType>(ptr));
        return result;
    };


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


    /**
     * Template allowing AbstractDatabaseObjectFactory implementations to easily and
     * automatically cast the abstract DatabaseObject pointers returned by the
     * abstractLoad() methods into a concrete DatabaseObject type.
     * This is done by adding load() methods that return casted shared_ptrs.
     */
    template <class CastDatabaseObject>
    class CastableDatabaseObjectFactory : public AbstractDatabaseObjectFactory {
    public:
        CastableDatabaseObjectFactory(std::shared_ptr<Database> database)
            : AbstractDatabaseObjectFactory(database) {}


        /* = load / loadMany implementations = */
        std::shared_ptr<CastDatabaseObject> load(
                SqlitePreparedStatement & query,
                const std::map<std::string, std::string> & alternativeColumnNames
                = std::map<std::string, std::string>()
        ) {
            return std::static_pointer_cast<CastDatabaseObject>(abstractLoad(query, alternativeColumnNames));
        }

        std::shared_ptr<CastDatabaseObject> load(
                SqlitePreparedStatement & query,
                const std::map<std::string, int> & columnIndexes
        ) {
            return std::static_pointer_cast<CastDatabaseObject>(abstractLoad(query, columnIndexes));
        }

        virtual std::vector<std::shared_ptr<CastDatabaseObject>> loadMany(
                SqlitePreparedStatement & query, int n,
                const std::map<std::string, std::string> & alternativeColumnNames
                = std::map<std::string, std::string>()
        ) {
            /* Cast from loadMany() */
            return castSharedPtrVector<DatabaseObject, CastDatabaseObject>(
                    abstractLoadMany(query, n, alternativeColumnNames)
            );
        }

        virtual std::vector<std::shared_ptr<CastDatabaseObject>> loadMany(
                SqlitePreparedStatement & query, int n,
                const std::map<std::string, int> & columnIndexes
        ) {
            /* Cast from loadMany() */
            return castSharedPtrVector<DatabaseObject, CastDatabaseObject>(
                    abstractLoadMany(query, n, columnIndexes)
            );
        }
    };


    /**
     * This template provides a default go-to place for DatabaseObject construction,
     * which should serve nearly all use cases without the need of creating a
     * custom factory class for each DatabaseObject.
     *
     * It can construct DatabaseObjects in a generic manner (using all available
     * constructors) as well as load them from arbitrary queries.
     */
    template <class ConcreteDatabaseObject>
    class DatabaseObjectFactory : public CastableDatabaseObjectFactory<ConcreteDatabaseObject> {
        // This is needed because name resolution of parametrized base class otherwise fails
        using AbstractDatabaseObjectFactory::database;
        using AbstractDatabaseObjectFactory::buildColumnIndexes;
        using AbstractDatabaseObjectFactory::setLoaded;

    public:
        DatabaseObjectFactory(std::shared_ptr<Database> database)
                : CastableDatabaseObjectFactory<ConcreteDatabaseObject>(database) {}

        /**
         * Generic factory function exposing private constructors as shared_ptr-only
         * public version.
         */
        template<typename ...ConstructorArgs>
        std::shared_ptr<ConcreteDatabaseObject> construct(ConstructorArgs... args) {
            return std::shared_ptr<ConcreteDatabaseObject>(new ConcreteDatabaseObject(database, args...));
        }


        /* = Virtual function implementations = */
        virtual std::shared_ptr<DatabaseObject> abstractLoad(
                SqlitePreparedStatement & query,
                const std::map<std::string, std::string> & alternativeColumnNames
                = std::map<std::string, std::string>()
        ) override {
            return abstractLoad(query, buildColumnIndexes(query, alternativeColumnNames, ConcreteDatabaseObject::tableName));
        }

        virtual std::shared_ptr<DatabaseObject> abstractLoad(
                SqlitePreparedStatement & query,
                const std::map<std::string, int> & columnIndexes
        ) override {
            /* Call load constructor */
            auto result = std::shared_ptr<DatabaseObject>(
                    new ConcreteDatabaseObject(database, query, columnIndexes)
            );

            /* Set loaded flag on freshly loaded object */
            setLoaded(result, true);

            return result;
        }


        virtual std::vector<std::shared_ptr<DatabaseObject>> abstractLoadMany(
                SqlitePreparedStatement & query, int n,
                const std::map<std::string, std::string> & alternativeColumnNames
                = std::map<std::string, std::string>()
        ) override {
            return abstractLoadMany(query, n, buildColumnIndexes(query, alternativeColumnNames, ConcreteDatabaseObject::tableName));
        }

        virtual std::vector<std::shared_ptr<DatabaseObject>> abstractLoadMany(
                SqlitePreparedStatement & query, int n,
                const std::map<std::string, int> & columnIndexes
        ) override {
            /* Prepare column indexes and result memory */
            auto result = std::vector<std::shared_ptr<DatabaseObject>>();

            /* Query multiple objects */
            if (n < 0) {
                // Query until there are no more rows left
                do {
                    result.push_back(abstractLoad(query, columnIndexes));
                } while(query.step());
            } else {
                // Query until there are either no more rows left or n rows have been read
                for (int i = 0; i < n; i++) {
                    result.push_back(abstractLoad(query, columnIndexes));
                    if (!query.step())
                        break;
                }
            }

            /* Finalize */
            return result;
        }
    };


    /**
     * This is a DatabaseObjectFactory for polymorphic DatabaseObjects.
     * Polymorphic Base DatabaseObjects have to define a public static member function
     * that determines which factory type to actually use for each string type parameter.
     * Its signature is:
     *
     * static std::unique_ptr<AbstractDatabaseObjectFactory> polymorphicFactory(std::shared_ptr<Database> database, const string & type);
     *
     * Note: The loading methods of this factory will return a null pointer on invalid
     * types for single-item load functions and ignore rows with invalid types for multi-item
     * load functions.
     */
    template <class BaseDatabaseObject>
    class PolymorphicDatabaseObjectFactory : public CastableDatabaseObjectFactory<BaseDatabaseObject> {
        // This is needed because name resolution of parametrized base class otherwise fails
        using AbstractDatabaseObjectFactory::database;
        using AbstractDatabaseObjectFactory::buildColumnIndexes;
        using AbstractDatabaseObjectFactory::setLoaded;

    protected:
        std::unique_ptr<AbstractDatabaseObjectFactory> retrieveFactory(const std::string & type) {
            return std::move(BaseDatabaseObject::polymorphicFactory(database, type));
        }

    public:
        PolymorphicDatabaseObjectFactory(std::shared_ptr<Database> database)
                : CastableDatabaseObjectFactory<BaseDatabaseObject>(database) {}

        /* = Virtual function implementations = */
        virtual std::shared_ptr<DatabaseObject> abstractLoad(
                SqlitePreparedStatement & query,
                const std::map<std::string, std::string> & alternativeColumnNames
                = std::map<std::string, std::string>()
        ) override {
            return abstractLoad(query, buildColumnIndexes(query, alternativeColumnNames, BaseDatabaseObject::tableName));
        }

        virtual std::shared_ptr<DatabaseObject> abstractLoad(
                SqlitePreparedStatement & query,
                const std::map<std::string, int> & columnIndexes
        ) override {
            auto type = query.columnString(columnIndexes.at("type"));
            auto factory = retrieveFactory(type);
            return factory ? factory->abstractLoad(query, columnIndexes) : nullptr;
        }

        virtual std::vector<std::shared_ptr<DatabaseObject>> abstractLoadMany(
                SqlitePreparedStatement & query, int n,
                const std::map<std::string, std::string> & alternativeColumnNames
                = std::map<std::string, std::string>()
        ) override {
            return abstractLoadMany(query, n, buildColumnIndexes(query, alternativeColumnNames, BaseDatabaseObject::tableName));
        }

        virtual std::vector<std::shared_ptr<DatabaseObject>> abstractLoadMany(
                SqlitePreparedStatement & query, int n,
                const std::map<std::string, int> & columnIndexes
        ) override {
            /* Prepare result container and determine type column index */
            auto typeIndex = columnIndexes.at("type");
            auto result = std::vector<std::shared_ptr<DatabaseObject>>();

            /* Query multiple objects */
            if (n != 0) {
                int nLoaded = 0;
                do {
                    auto factory = retrieveFactory(query.columnString(typeIndex));
                    if (!factory)
                        continue;
                    result.push_back(factory->abstractLoad(query, columnIndexes));
                    nLoaded++;
                } while (query.step() && nLoaded != n);
            }

            /* Finalize */
            return result;
        }
    };
}

#endif
