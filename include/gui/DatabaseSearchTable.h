#ifndef PROG2_BELEG_GUI_DATABASE_SEARCH_TABLE_H
#define PROG2_BELEG_GUI_DATABASE_SEARCH_TABLE_H

#include <wx/wx.h>
#include <wx/dataview.h>
#include <wx/srchctrl.h>

#include "core/sqlite/SqlitePreparedStatement.h"

namespace pb2 {
    class DatabaseSearchTable : public wxPanel {
    private:
        wxSearchCtrl * searchCtrl = nullptr;
        wxDataViewListCtrl * dataView = nullptr;

    public:
        DatabaseSearchTable(wxWindow * parent, const wxString & newItemButtonLabel);
        virtual ~DatabaseSearchTable() = default;
    };
}

#endif
