#ifndef PROG2_BELEG_LENDINGS_SEARCH_TABLE_H
#define PROG2_BELEG_LENDINGS_SEARCH_TABLE_H

#include "TwoQuerySearchTable.h"
#include "core/Database.h"
#include "core/domain/Lending.h"
#include "core/sqlite/SqlitePreparedStatement.h"

namespace pb2 {
    /**
     * DatabaseSearchTable for Lending entities.
     */
    class LendingsSearchTable : public TwoQuerySearchTable {
    private:
        bool activeOnly;
        int userId = 0;

    protected:
        virtual std::string getListSQL() override;
        virtual std::string getSearchSQL() override;
        virtual wxString getColumnContent(int column) override;
        virtual void prepareSearch(const wxString & query) override;
        virtual void prepareList() override;

    public:
        /**
         * Constructs a new LendingsSearchTable
         * @param activeOnly Set this to true to only show lendings that are not returned
         * yet.
         */
        LendingsSearchTable(wxWindow * parent, wxWindowID id, std::shared_ptr<Database> database, bool activeOnly);
        virtual ~LendingsSearchTable() = default;

        void setUserId(int id);
        virtual void list() override;
        virtual void search(const wxString & query) override;

        inline bool getActiveOnly() const { return activeOnly; }

        /**
         * Returns a shared pointer to the currently selected lending, or nullptr if no
         * lending is selected / could be determined.
         */
        std::shared_ptr<Lending> getSelectedLending() const;

        /**
         * Returns the ID of the currently selected Lending, or 0 if no user is selected or
         * could be determined.
         */
        int getSelectedId() const;
    };
}

#endif
