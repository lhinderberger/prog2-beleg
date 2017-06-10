#include "core/DatabaseObjectFactory.h"
#include "core/DatabaseObject.priv.h"

using namespace pb2;
using namespace std;

AbstractDatabaseObjectFactory::AbstractDatabaseObjectFactory(
        std::shared_ptr<Database> & database) {
    this->database = database;
}

AbstractDatabaseObjectFactory::~AbstractDatabaseObjectFactory() {}

shared_ptr<DatabaseObject> AbstractDatabaseObjectFactory::baseLoad(
        SqlPreparedStatement & query,
        const map<string, string> & alternativeColumnNames
) {
    return constructLoad(query, buildColumnIndexes(query, alternativeColumnNames));
}

vector<std::shared_ptr<DatabaseObject>> AbstractDatabaseObjectFactory::baseLoadMany(
        SqlPreparedStatement & query, int n,
        const map<string, string> & alternativeColumnNames
) {
    /* Prepare column indexes and result memory */
    auto cleanColumnIndexes = buildColumnIndexes(query, alternativeColumnNames);
    auto result = vector<shared_ptr<DatabaseObject>>();

    /* Query multiple objects */
    if (n < 0) {
        // Query until there are no more rows left
        do {
            result.push_back(constructLoad(query, cleanColumnIndexes));
        } while(query.step());
    } else {
        // Query until there are either no more rows left or n rows have been read
        for (int i = 0; i < n; i++) {
            result.push_back(constructLoad(query, cleanColumnIndexes));
            if (!query.step())
                break;
        }
    }

    /* Finalize */
    return result;
}

map<string, int> AbstractDatabaseObjectFactory::buildColumnIndexes(
        SqlPreparedStatement & query,
        const map<string, string> & alternativeColumnNames
) {
    /* Build column indexes map, incorporate alternatives */
    map<string,int> columnIndexes = query.columnIndexes();
    for (const auto & alternative : alternativeColumnNames)
        columnIndexes[alternative.first] = query.getColumnIndex(alternative.second);

    /* Strip full column names of table name */
    auto cleanColumnIndexes = map<string,int>();
    for (const auto & indexPair : columnIndexes) {
        const auto & columnName = indexPair.first;

        /* Find separating dot */
        auto dotIndex = columnName.find('.');
        if (dotIndex == columnName.npos)
            continue;

        /* Drop columns from other tables */
        if (getTableName() != columnName.substr(0, dotIndex))
            continue;

        /* Add index to clean map */
        cleanColumnIndexes[columnName.substr(dotIndex + 1)] = indexPair.second;
    }

    return cleanColumnIndexes;
}

shared_ptr<DatabaseObject> AbstractDatabaseObjectFactory::constructLoad(
        SqlPreparedStatement & query, const map<string, int> & columnIndexes
) {
    /* Call load implementation */
    auto result = constructLoadImpl(query, columnIndexes);

    /* Set loaded flag on freshly loaded object */
    result->priv->loaded = true;

    return result;
}
