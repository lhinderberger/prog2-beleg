#ifndef PROG2_BELEG_GUI_MAIN_FRAME_H
#define PROG2_BELEG_GUI_MAIN_FRAME_H

#include "core/Database.h"
#include "gui/DatabasePanel.h"
#include <wx/wx.h>

namespace pb2 {
    /**
     * Contains the main window of prog2-beleg and is responsible for managing
     * DatabasePanels. It provides the wiring for database creation / opening / closing.
     */
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
        DatabasePanel * databasePanel = nullptr;

        void onClose(wxCloseEvent & event);
        void evCloseDatabase(wxCommandEvent & event) { closeDatabase(); }
        void evNewDatabase(wxCommandEvent & event) { newDatabase(); }
        void evOpenDatabase(wxCommandEvent & event) { openDatabase(); }
        void setDatabaseAndOpenPanel(std::shared_ptr<Database> database);

        wxDECLARE_EVENT_TABLE();
    };
}

#endif
