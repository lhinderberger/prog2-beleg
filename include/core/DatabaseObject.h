#ifndef PROG2_BELEG_DATABASE_OBJECT_H
#define PROG2_BELEG_DATABASE_OBJECT_H

#include <memory>
#include <map>
#include <string>
#include <vector>

#include "SqlPreparedStatement.h"

namespace pb2 {
    class Database;
    class DatabaseObject_priv;

    class DatabaseObject : public std::enable_shared_from_this<DatabaseObject> {
    private:
        std::unique_ptr<DatabaseObject_priv> priv;

    protected:
        DatabaseObject(std::shared_ptr<Database> database);

        virtual void loadImpl(SqlPreparedStatement & query,
                  const std::map<std::string, int> & columnIndexes) = 0;

        virtual void persistImpl() = 0;

    public:
        virtual ~DatabaseObject();

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
        void load(SqlPreparedStatement & query,
            const std::map<std::string, std::string> & alternativeColumnNames
            = std::map<std::string, std::string>());

        /**
         * Persists the DatabaseObject to the database.
         */
        void persist();

        /**
         * Builds a SQL query string for inserting a set number of objects into the
         * database.
         *
         * This will generate a query like
         *
         * INSERT INTO test(col_a, col_b) VALUES (?,?), (?,?);
         *
         * given the parameters columnNames{"col_a", "col_b"} and nObjects(2).
         */
        std::string buildInsertQuery(std::vector<std::string> columnNames, int nObjects) const;

        /**
         * Builds a SQL query string for updating a table.
         *
         * This will generate a query like
         *
         * UPDATE test SET col_a=?, col_b=? WHERE id=?;
         *
         * given the parameters columnNames{"col_a", "col_b"} and whereClause("WHERE id=?").
         */
        std::string buildUpdateQuery(std::vector<std::string> columnNames,
                                     const std::string & whereClause) const;

        /**
         * Tries to determine the full column name of a given partial column name and
         * optional alternative names given.
         *
         * @param columnName A partial column name, without the table name prefixed.
         * @param alternativeNames If the full column name is a key in alternativeNames,
         * the value of the relative map entry will be returned instead of the full column
         * name.
         */
        std::string getFullColumnName(const std::string & columnName) const;
        std::string getFullColumnName(
                const std::string & columnName,
                const std::map<std::string, std::string> & alternativeNames
        ) const;


        // Getters
        const std::shared_ptr<Database> getDatabase() const;
        const std::shared_ptr<SqlConnection> getConnection() const;

        /**
         * Returns true if the object was loaded from database instead of constructed
         * at runtime.
         */
        bool isLoaded() const;
    };
}

#include "Database.h"

#endif
