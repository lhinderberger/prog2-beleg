#include "core/DatabaseObjectFactory.h"
#include "core/DatabaseObject.priv.h"

using namespace pb2;
using namespace std;

AbstractDatabaseObjectFactory::AbstractDatabaseObjectFactory(
        std::shared_ptr<Database> & database) {
    this->database = database;
}

AbstractDatabaseObjectFactory::~AbstractDatabaseObjectFactory() {}


map<string, int> AbstractDatabaseObjectFactory::buildColumnIndexes(
        SqlitePreparedStatement & query,
        const map<string, string> & alternativeColumnNames,
        const std::string & tableName
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
        if (tableName != columnName.substr(0, dotIndex))
            continue;

        /* Add index to clean map */
        cleanColumnIndexes[columnName.substr(dotIndex + 1)] = indexPair.second;
    }

    return cleanColumnIndexes;
}

void AbstractDatabaseObjectFactory::setLoaded(shared_ptr<DatabaseObject> dbObject, bool loaded) {
    dbObject->setLoaded(loaded);
}
