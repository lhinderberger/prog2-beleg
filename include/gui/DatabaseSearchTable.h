#ifndef PROG2_BELEG_GUI_DATABASE_SEARCH_TABLE_H
#define PROG2_BELEG_GUI_DATABASE_SEARCH_TABLE_H

#include <vector>
#include <wx/wx.h>
#include <wx/dataview.h>
#include <wx/srchctrl.h>

#include "core/sqlite/SqlitePreparedStatement.h"

wxDECLARE_EVENT(PB2_EVT_ST_SELECTED, wxCommandEvent);
wxDECLARE_EVENT(PB2_EVT_ST_NEW_ITEM, wxCommandEvent);

namespace pb2 {
    class DatabaseSearchTable : public wxPanel {
    private:
        void buildColumns(const std::vector<wxString> & headings);
        void buildRows();
        void evSearch(wxCommandEvent & ev);
        void evNewItem(wxCommandEvent & ev);
        void evRowSelected(wxCommandEvent & ev);

    protected:
        wxSearchCtrl * searchCtrl = nullptr;
        wxDataViewListCtrl * dataView = nullptr;

        /* Implementing class shall prepare a search query and set it ready to be used
         * in subsequent calls to step() / getColumnContent().
         */
        virtual void prepareSearch(const wxString & query) = 0;

        /**
         * Like prepareSearch, but for list queries without search term (e.g. "catch all")
         * @param sortColumn
         */
        virtual void prepareList() = 0;

        /** Retrieve the next row (usually direct call to SqlitePreparedStatement::step()) */
        virtual bool step() = 0;

        /** Retrieve the text content of the given column
         * Column order as in columnHeadings constructor argument
         */
        virtual wxString getColumnContent(int column) = 0;

    public:
        enum class ID { SEARCH_CTRL = 1, NEW_ITEM_BUTTON, DATA_VIEW };

        DatabaseSearchTable(
            wxWindow * parent, const wxString & newItemButtonLabel,
            const std::vector<wxString> & columnHeadings
        );
        virtual ~DatabaseSearchTable() = default;

        virtual void list();
        virtual void search(const wxString & query);

    wxDECLARE_EVENT_TABLE();
    };
}

#endif
