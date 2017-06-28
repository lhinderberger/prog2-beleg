#ifndef PROG2_BELEG_TWO_QUERY_SEARCH_TABLE_H
#define PROG2_BELEG_TWO_QUERY_SEARCH_TABLE_H

#include "DatabaseSearchTable.h"
#include "core/Database.h"
#include "core/sqlite/SqlitePreparedStatement.h"

namespace pb2 {
    /**
     * Serves as base class for common DatabaseSearchTables that have one query for
     * listing and another one for searching.
     * Note: prepareSearch() will bind the query string parameter as parameter 1 of your
     * search query, unless you explicitly call "setNoBind" in the constructor.
     */
    class TwoQuerySearchTable : public DatabaseSearchTable {
    protected:
        enum class QueryType { NONE, LIST, SEARCH };


        virtual std::string getListSQL() = 0;
        virtual std::string getSearchSQL() = 0;

        virtual void prepareSearch(const wxString & query) override;
        virtual void prepareList() override;
        virtual bool step() override;

        /**
         * Retrieve the currently active query or throw logic_error if there
         * is no active query (i.e. currentQueryType is NONE)
         */
        std::shared_ptr<SqlitePreparedStatement> currentQuery();

        /**
         * Call setNoBind in the constructor to keep TwoQuerySearchTable from binding
         * the query string parameter in prepareSearch().
         */
        inline void setNoBind(bool noBind) { this->noBind = noBind; }

        inline QueryType getCurrentQueryType() const { return currentQueryType; }

    public:
        TwoQuerySearchTable(
                wxWindow * parent, wxWindowID id, const wxString & newItemButtonLabel,
                const std::vector<wxString> & columnHeadings,
                std::shared_ptr<Database> database
        );
        virtual ~TwoQuerySearchTable() = default;

        inline std::shared_ptr<Database> getDatabase() const { return database; }

    private:
        bool noBind = false;
        QueryType currentQueryType = QueryType::NONE;

        std::shared_ptr<Database> database;
        std::shared_ptr<SqlitePreparedStatement> listQuery;
        std::shared_ptr<SqlitePreparedStatement> searchQuery;

        void generalPrepare(std::shared_ptr<SqlitePreparedStatement> & query, const std::string & sql, QueryType type);
    };
}

#endif
