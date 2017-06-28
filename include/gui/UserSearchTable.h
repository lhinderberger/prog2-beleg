#ifndef PROG2_BELEG_USER_SEARCH_TABLE_H
#define PROG2_BELEG_USER_SEARCH_TABLE_H

#include "TwoQuerySearchTable.h"
#include "core/Database.h"
#include "core/sqlite/SqlitePreparedStatement.h"

namespace pb2 {
    class UserSearchTable : public TwoQuerySearchTable {
    protected:
        virtual std::string getListSQL() override;
        virtual std::string getSearchSQL() override;
        virtual wxString getColumnContent(int column) override;

    public:
        UserSearchTable(wxWindow * parent, std::shared_ptr<Database> database);
        virtual ~UserSearchTable() = default;
    };
}

#endif
