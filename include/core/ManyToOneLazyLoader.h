#ifndef PROG2_BELEG_MANY_TO_ONE_LAZY_LOADER_H
#define PROG2_BELEG_MANY_TO_ONE_LAZY_LOADER_H

#include <memory>
#include <stdexcept>

#include "exceptions.h"
#include "DatabaseObject.h"
#include "DatabaseObjectFactory.h"
#include "sqlite/SqlitePreparedStatement.h"

namespace pb2 {
    /**
     * The ManyToOneLazyLoader gives DatabaseObjects the ability to delay loading of
     * related objects until the time of their actual usage (Lazy Loading).
     *
     * Currently, this only supports referenced objects in Many-To-One relations that have
     * one primary key column. It is important that referenced classes define a public
     * static string named primaryKeyColumn that contains the name of the primary key
     * column (without table name).
     *
     * @tparam ConcreteDatabaseObject When refering a Polymorphic Type, insert the base class here.
     * @tparam FactoryClass Pass in PolymorphicDatabaseObjectFactory to be able to reference polymorphic types
     */
    template<class ConcreteDatabaseObject, typename PrimaryKeyType, class FactoryClass = DatabaseObjectFactory<ConcreteDatabaseObject>>
    class ManyToOneLazyLoader {
        static_assert(std::is_base_of<DatabaseObject, ConcreteDatabaseObject>::value,
                      "ManyToOneLazyLoader can only handle subclasses of DatabaseObject!");
    private:
        std::shared_ptr<Database> database = nullptr;
        std::shared_ptr<ConcreteDatabaseObject> ptr = nullptr;
        std::unique_ptr<PrimaryKeyType> primaryKey = nullptr;

        void load() {
            if (!primaryKey)
                return; // If there's no primary key, there's nothing we can do.

            /* Prepare statement */
            auto sql = std::string("SELECT * FROM ") + ConcreteDatabaseObject::tableName + " WHERE "
                         + ConcreteDatabaseObject::primaryKeyColumn + " = ?";
            SqlitePreparedStatement query(database->getConnection(), sql);

            /* Create new object from statement result */
            query.bind(1, *primaryKey);
            if (!query.step())
                throw DatabaseIntegrityException("Query " + sql + " returned no result!"); //TODO: Include primary key with error message somehow
            ptr = FactoryClass(database).load(query);
            primaryKey = nullptr;
        }

    public:
        ManyToOneLazyLoader(const std::shared_ptr<Database> & database) {
            if (!database)
                throw NullPointerException("No database set for ManyToOneLazyLoader");

            this->database = database;
        }

        /**
         * Returns the primary key for lazy loading, if one has been set.
         * Otherwise throws a NullPointerException.
         */
        PrimaryKeyType getPrimaryKey() const {
            if (!primaryKey)
                throw NullPointerException("Primary key not set");
            return *primaryKey;
        }

        /**
         * Loads or - if already loaded - returns a pointer to the loaded object.
         * If both the primary key and the pointer all unset, this will return a
         * null pointer.
         */
        std::shared_ptr<ConcreteDatabaseObject> get() {
            if (!ptr && primaryKey)
                load();
            return ptr;
        }

        std::shared_ptr<ConcreteDatabaseObject> operator->() {
            return get();
        }

        const std::shared_ptr<ConcreteDatabaseObject> operator->() const {
            return operator->();
        }

        void set(const std::shared_ptr<ConcreteDatabaseObject> & object) {
            primaryKey = nullptr;
            ptr = object;
        }

        void set(const PrimaryKeyType & primaryKey) {
            ptr = nullptr;
            this->primaryKey = std::make_unique<PrimaryKeyType>(primaryKey);
        }

        inline std::shared_ptr<Database> getDatabase() const { return database; }
        inline bool isPrimaryKeySet() const { return primaryKey != nullptr; }
        inline bool isLoaded() const { return ptr != nullptr; }
    };
}

#endif
