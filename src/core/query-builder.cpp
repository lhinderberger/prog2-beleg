#include "core/query-builder.h"

#include <stdexcept>

using namespace std;
using namespace pb2;


// Helper functions
void glue(vector<string> columnNames, const string & suffix, const string & separator, string & out) {
    bool first = true;
    for (const auto & columnName : columnNames) {
        if (!first)
            out += separator;
        out += columnName + suffix;
        first = false;
    }
}

void glue(const string & repeatString, size_t nRepeat, const string & separator, string & out) {
    bool first = true;
    for (size_t i = 0; i < nRepeat; i++) {
        if (!first)
            out += separator;
        out += repeatString;
        first = false;
    }
}

string pb2::buildInsertQuery(
        vector<string> columnNames, int nObjects, const string & tableName
) {
    if (nObjects < 0)
        throw logic_error("nObjects must not be negative!");

    /* Begin query */
    string query = "INSERT INTO " + tableName;

    /* Glue together column names */
    query += " (";
    glue(columnNames, "", ", ", query);
    query += ')';

    /* Add value bracket pairs */
    query += " VALUES ";

    string valueBracketPair = "(";
    glue("?", columnNames.size(), ", ", valueBracketPair);
    valueBracketPair += ')';

    glue(valueBracketPair, (size_t)nObjects, ", ", query);

    /* Finalize and return */
    return query + ';';
}

string pb2::buildUpdateQuery(
        vector<string> columnNames, const string & whereClause, const string & tableName
) {
    /* Begin query */
    string query = "UPDATE " + tableName;

    /* Glue together column update statements */
    query += " SET ";
    glue(columnNames, " = ?", ", ", query);

    /* Return including where clause */
    return query + ' ' + whereClause;
}