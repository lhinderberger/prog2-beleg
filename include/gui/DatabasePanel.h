#ifndef PROG2_BELEG_GUI_DATABASE_PANEL_H
#define PROG2_BELEG_GUI_DATABASE_PANEL_H

#include <wx/wx.h>
#include <wx/aui/auibook.h>
#include "core/Database.h"

#include "BasketWindow.h"

namespace pb2 {
    /**
     * Panel holding one currently loaded Database and offering all possible controls
     * for modifying that Database.
     */
    class DatabasePanel : public wxPanel {
    private:
        BasketWindow * basketWindow = nullptr;
        wxAuiNotebook * notebook = nullptr;
        std::shared_ptr<Database> database;

    public:
        DatabasePanel(wxWindow * parent, std::shared_ptr<Database> database);

        inline std::shared_ptr<Database> getDatabase() { return database; }
    };
}

#endif
