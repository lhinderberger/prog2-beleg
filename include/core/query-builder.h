#ifndef PROG2_BELEG_QUERY_BUILDER_H
#define PROG2_BELEG_QUERY_BUILDER_H

#include <string>
#include <vector>

namespace pb2 {
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
    std::string buildInsertQuery(
        std::vector<std::string> columnNames, int nObjects, const std::string & tableName
    );

    template<class ConcreteDatabaseObject>
    std::string buildInsertQuery(std::vector<std::string> columnNames, int nObjects) {
        return buildInsertQuery(columnNames, nObjects, ConcreteDatabaseObject::tableName);
    }

    /**
     * Builds a SQL query string for updating a table.
     *
     * This will generate a query like
     *
     * UPDATE test SET col_a=?, col_b=? WHERE id=?;
     *
     * given the parameters columnNames{"col_a", "col_b"} and whereClause("WHERE id=?").
     */
    std::string buildUpdateQuery(
            std::vector<std::string> columnNames, const std::string & whereClause,
            const std::string & tableName
    );

    template<class ConcreteDatabaseObject>
    std::string buildUpdateQuery(
            std::vector<std::string> columnNames, const std::string & whereClause
    ) {
        return buildUpdateQuery(columnNames, whereClause, ConcreteDatabaseObject::tableName);
    }
}

#endif
