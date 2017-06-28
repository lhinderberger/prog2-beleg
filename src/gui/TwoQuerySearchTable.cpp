#include "gui/TwoQuerySearchTable.h"

using namespace pb2;
using namespace std;

TwoQuerySearchTable::TwoQuerySearchTable(
        wxWindow * parent, wxWindowID id, const wxString & newItemButtonLabel,
        const vector<wxString> & columnHeadings, shared_ptr<Database> database
) : DatabaseSearchTable(parent, id, newItemButtonLabel, columnHeadings), database(database) {

}


shared_ptr<SqlitePreparedStatement> TwoQuerySearchTable::currentQuery() {
    if (currentQueryType == QueryType::LIST)
        return listQuery;
    else if (currentQueryType == QueryType::SEARCH)
        return searchQuery;
    else
        throw logic_error("currentQuery: No active query!");
}

void TwoQuerySearchTable::generalPrepare(shared_ptr<SqlitePreparedStatement> & query,
                                         const string & sql, QueryType type) {
    /* Prepare query, if necessary */
    if (query == nullptr)
        query = make_shared<SqlitePreparedStatement>(database->getConnection(), sql);

    /* Reset and make current */
    query->reset();
    currentQueryType = type;
}

void TwoQuerySearchTable::prepareSearch(const wxString & query) {
    /* Prepare query, if necessary */
    generalPrepare(searchQuery, getSearchSQL(), QueryType::SEARCH);

    /* Bind parameter */
    if (!noBind)
        searchQuery->bind(1, query.ToStdString());
}

void TwoQuerySearchTable::prepareList() {
    /* Prepare query, if necessary */
    generalPrepare(listQuery, getListSQL(), QueryType::LIST);
}

bool TwoQuerySearchTable::step() {
    return currentQuery()->step();
}
