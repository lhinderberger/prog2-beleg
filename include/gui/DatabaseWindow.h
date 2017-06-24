#ifndef PROG2_BELEG_GUI_DATABASE_WINDOW_H
#define PROG2_BELEG_GUI_DATABASE_WINDOW_H

#include <wx/wx.h>
#include "core/Database.h"

namespace pb2 {
    /**
     * Window holding one currently loaded Database and offering all possible controls
     * for modifying that Database.
     */
    class DatabaseWindow : public wxWindow {
    private:
        wxWindow * contentWindow = nullptr;
        std::shared_ptr<Database> database;

    public:
        DatabaseWindow(wxWindow * parent, std::shared_ptr<Database> database);

        inline std::shared_ptr<Database> getDatabase() { return database; }
    };
}

#endif
