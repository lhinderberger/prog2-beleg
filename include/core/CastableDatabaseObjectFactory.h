#ifndef PROG2_BELEG_CASTABLE_DATABASE_OBJECT_FACTORY_H
#define PROG2_BELEG_CASTABLE_DATABASE_OBJECT_FACTORY_H

#include "AbstractDatabaseObjectFactory.h"

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
}

#endif
