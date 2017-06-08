#include "core/DatabaseObject.h"
#include "core/exceptions.h"
#include "core/DatabaseObject.priv.h"

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


DatabaseObject::DatabaseObject(std::shared_ptr<Database> database) {
    if (!database)
        throw NullPointerException();

    priv = make_unique<DatabaseObject_priv>();
    priv->database = database;
}

DatabaseObject::~DatabaseObject() {}


void DatabaseObject::load(SqlPreparedStatement & query,
                          const map<string, string> & alternativeColumnNames) {
    /* Build column indexes map, incorporate alternatives */
    map<string,int> columnIndexes = query.columnIndexes();
    for (const auto & alternative : alternativeColumnNames)
        columnIndexes[alternative.first] = query.getColumnIndex(alternative.second);

    /* Call load implementation */
    loadImpl(query, columnIndexes);
    priv->loaded = true;
}

void DatabaseObject::persist() {
    persistImpl();
}

string DatabaseObject::buildInsertQuery(vector<string> columnNames, int nObjects) const {
    if (nObjects < 0)
        throw logic_error("nObjects must not be negative!");

    /* Begin query */
    string query = "INSERT INTO " + getTableName();

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

string DatabaseObject::buildUpdateQuery(vector<string> columnNames,
                                        const string & whereClause) const {
    /* Begin query */
    string query = "UPDATE " + getTableName();

    /* Glue together column update statements */
    query += " SET ";
    glue(columnNames, " = ?", ", ", query);

    /* Return including where clause */
    return query + ' ' + whereClause;
}

string DatabaseObject::getFullColumnName(const string & columnName) const {
    return string(getTableName()) + '.' + columnName;
}

string DatabaseObject::getFullColumnName(
        const string & columnName,
        const map<string, string> & alternativeNames
) const {
    /* Retrieve regular full column name first */
    auto fullColumnName = getFullColumnName(columnName);

    /* Search alternative names to see if there is an alternative for this name */
    auto i = alternativeNames.find(fullColumnName);
    if (i == alternativeNames.end())
        return fullColumnName;

    /* Return depending on search result */
    return (i == alternativeNames.end()) ? fullColumnName : i->second;
}

const shared_ptr<SqlConnection> DatabaseObject::getConnection() const {
    return priv->database->getConnection();
}

const shared_ptr<Database> DatabaseObject::getDatabase() const {
    return priv->database;
}

bool DatabaseObject::isLoaded() const {
    return priv->loaded;
}
