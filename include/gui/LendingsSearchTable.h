#ifndef PROG2_BELEG_LENDINGS_SEARCH_TABLE_H
#define PROG2_BELEG_LENDINGS_SEARCH_TABLE_H

#include "TwoQuerySearchTable.h"
#include "core/Database.h"
#include "core/sqlite/SqlitePreparedStatement.h"

namespace pb2 {
    class LendingsSearchTable : public TwoQuerySearchTable {
    private:
        int userId = 0;

    protected:
        virtual std::string getListSQL() override;
        virtual std::string getSearchSQL() override;
        virtual wxString getColumnContent(int column) override;
        virtual void prepareSearch(const wxString & query) override;
        virtual void prepareList() override;

    public:
        LendingsSearchTable(wxWindow * parent, wxWindowID id, std::shared_ptr<Database> database);
        virtual ~LendingsSearchTable() = default;

        void setUserId(int id);
        virtual void list() override;
        virtual void search(const wxString & query) override;
    };
}

#endif
