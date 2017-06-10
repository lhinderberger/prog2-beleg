#ifndef PROG2_BELEG_MANY_TO_ONE_LAZY_LOADER_H
#define PROG2_BELEG_MANY_TO_ONE_LAZY_LOADER_H

#include <memory>
#include <stdexcept>

#include "exceptions.h"
#include "DatabaseObject.h"
#include "core/sqlite/SqlitePreparedStatement.h"

namespace pb2 {
    template<class ConcreteDatabaseObject, typename PrimaryKeyType>
    class ManyToOneLazyLoader {
        static_assert(std::is_base_of<DatabaseObject, ConcreteDatabaseObject>::value,
                      "ManyToOneLazyLoader can only handle subclasses of DatabaseObject!");
    private:
        std::shared_ptr<Database> database = nullptr;
        std::shared_ptr<ConcreteDatabaseObject> ptr = nullptr;
        std::unique_ptr<PrimaryKeyType> primaryKey = nullptr;
        std::string tableName, primaryKeyName;

        void load() {
            if (!primaryKey)
                throw std::logic_error("Cannot load without primary key!");

            /* Prepare statement */
            auto query = std::string("SELECT * FROM ") + tableName + " WHERE "
                         + primaryKeyName + " = ?";
            auto statement = SqlitePreparedStatement(database->getConnection(), query);

            /* Create new object from statement result */
            statement.step();
            ptr = ConcreteDatabaseObject::construct(statement);
            primaryKey = nullptr;
        }

    public:
        ManyToOneLazyLoader(const std::shared_ptr<Database> & database,
                            const std::string & tableName,
                            const std::string & primaryKeyName) {
            if (!database)
                throw NullPointerException();

            this->database = database;
            this->tableName = tableName;
            this->primaryKeyName = primaryKeyName;
        }

        /**
         * Returns the primary key for lazy loading, if one has been set.
         * Otherwise throws a NullPointerException.
         */
        PrimaryKeyType getPrimaryKey() const {
            if (!primaryKey)
                throw NullPointerException();
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

        void set(const std::shared_ptr<ConcreteDatabaseObject> & object) {
            primaryKey = nullptr;
            ptr = object;
        }

        void set(const PrimaryKeyType & primaryKey) {
            ptr = nullptr;
            this->primaryKey = primaryKey;
        }

        inline std::shared_ptr<Database> getDatabase() const { return database; }
        inline bool isPrimaryKeySet() const { return primaryKey != nullptr; }
        inline bool isLoaded() const { return ptr != nullptr; }
    };
}

#endif
