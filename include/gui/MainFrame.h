#ifndef PROG2_BELEG_GUI_MAIN_FRAME_H
#define PROG2_BELEG_GUI_MAIN_FRAME_H

#include "core/Database.h"
#include "gui/DatabaseWindow.h"
#include <wx/wx.h>

namespace pb2 {
    class MainFrame : public wxFrame {
    public:
        enum class ID { NEW_DATABASE, OPEN_DATABASE, CLOSE_DATABASE };
        MainFrame(const wxString & title, const wxPoint & pos, const wxSize & size);

        /**
         * Close database with asking for user confirmation first
         * @return true, if the database was closed, false if closing was aborted
         */
        bool closeDatabase();

        void newDatabase();
        void openDatabase();

    private:
        std::shared_ptr<Database> database;
        DatabaseWindow * databaseWindow = nullptr;

        void onClose(wxCloseEvent & event);
        void tbCloseDatabase(wxCommandEvent & event) { closeDatabase(); }
        void tbNewDatabase(wxCommandEvent & event) { newDatabase(); }
        void tbOpenDatabase(wxCommandEvent & event) { openDatabase(); }

        wxDECLARE_EVENT_TABLE();
    };
}

#endif
