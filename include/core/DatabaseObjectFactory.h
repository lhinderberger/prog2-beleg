#ifndef PROG2_BELEG_DATABASE_OBJECT_FACTORY_H
#define PROG2_BELEG_DATABASE_OBJECT_FACTORY_H

#include "CastableDatabaseObjectFactory.h"

#define PB2_DECLARE_LOAD_CONSTRUCTOR(T) T(std::shared_ptr<Database> database, SqlitePreparedStatement & query, const std::map<std::string, int> & columnIndexes)

namespace pb2 {
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
         * This will call the constructors (even if private) of the ConcreteDatabaseObject.
         * It will forward the arguments given to it to that constructor, prepended with
         * a shared pointer to the database (which should be the first argument for every
         * DatabaseObject constructor).
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
}

#endif
