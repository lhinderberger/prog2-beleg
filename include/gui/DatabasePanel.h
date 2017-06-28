#ifndef PROG2_BELEG_GUI_DATABASE_PANEL_H
#define PROG2_BELEG_GUI_DATABASE_PANEL_H

#include <wx/wx.h>
#include <wx/aui/auibook.h>
#include "core/Database.h"

#include "BasketPanel.h"

namespace pb2 {
    /**
     * Panel holding one currently loaded Database and offering all possible controls
     * for modifying that Database.
     */
    class DatabasePanel : public wxPanel {
    private:
        BasketPanel * basketWindow = nullptr;
        wxAuiNotebook * notebook = nullptr;
        std::shared_ptr<Database> database;

        void evNewMediaBrowserTab(wxCommandEvent & event) { newMediaBrowserTab(); }
        void evNewUserBrowserTab(wxCommandEvent & event) { newUserBrowserTab(); }

    public:
        enum class ID { NEW_MEDIA_BROWSER_TAB, NEW_USER_BROWSER_TAB };

        DatabasePanel(wxWindow * parent, std::shared_ptr<Database> database);

        inline std::shared_ptr<Database> getDatabase() { return database; }
        void newMediaBrowserTab();
        void newUserBrowserTab();

    wxDECLARE_EVENT_TABLE();
    };
}

#endif
