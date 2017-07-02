#ifndef PROG2_BELEG_POLYMORPHIC_DATABASE_OBJECT_FACTORY_H
#define PROG2_BELEG_POLYMORPHIC_DATABASE_OBJECT_FACTORY_H

#include "CastableDatabaseObjectFactory.h"

namespace pb2 {
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
