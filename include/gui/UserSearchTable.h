#ifndef PROG2_BELEG_USER_SEARCH_TABLE_H
#define PROG2_BELEG_USER_SEARCH_TABLE_H

#include "TwoQuerySearchTable.h"
#include "core/Database.h"
#include "core/sqlite/SqlitePreparedStatement.h"
#include "core/domain/LibraryUser.h"

namespace pb2 {
    class UserSearchTable : public TwoQuerySearchTable {
    protected:
        virtual std::string getListSQL() override;
        virtual std::string getSearchSQL() override;
        virtual wxString getColumnContent(int column) override;

    public:
        UserSearchTable(wxWindow * parent, std::shared_ptr<Database> database);
        virtual ~UserSearchTable() = default;

        /**
         * Returns a shared pointer to the currently selected user, or nullptr if no
         * user is selected / could be determined.
         */
        std::shared_ptr<LibraryUser> getSelectedUser() const;

        /**
         * Returns the ID of the currently selected user, or 0 if no user is selected or
         * could be determined.
         */
        int getSelectedId() const;
    };
}

#endif
