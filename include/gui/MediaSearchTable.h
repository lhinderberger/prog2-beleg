#ifndef PROG2_BELEG_MEDIA_SEARCH_TABLE_H
#define PROG2_BELEG_MEDIA_SEARCH_TABLE_H

#include "DatabaseSearchTable.h"
#include "core/Database.h"
#include "core/sqlite/SqlitePreparedStatement.h"

namespace pb2 {
    class MediaSearchTable : public DatabaseSearchTable {
    private:
        enum class QueryType { NONE, LIST, SEARCH };

        QueryType currentQuery = QueryType::NONE;

        std::shared_ptr<Database> database;
        std::shared_ptr<SqlitePreparedStatement> listQuery;
        std::shared_ptr<SqlitePreparedStatement> searchQuery;



    protected:
        virtual void prepareSearch(const wxString & query) override;
        virtual void prepareList() override;
        virtual bool step() override;
        virtual wxString getColumnContent(int column) override;

    public:
        MediaSearchTable(wxWindow * parent, std::shared_ptr<Database> database);
        virtual ~MediaSearchTable() = default;
    };
}

#endif
