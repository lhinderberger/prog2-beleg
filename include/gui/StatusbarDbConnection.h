#ifndef PROG2_BELEG_GUI_STATUSBAR_DB_CONNECTION_H
#define PROG2_BELEG_GUI_STATUSBAR_DB_CONNECTION_H

#include <wx/wx.h>

#include "core/sqlite/SqliteConnection.h"

namespace pb2 {
    /**
     * SqliteConnection specialization that updates the GUI status bar when database
     * commits / rollbacks occured.
     */
    class StatusbarDbConnection : public SqliteConnection {
    private:
        wxStatusBar * statusBar = nullptr;

    protected:
        StatusbarDbConnection(wxStatusBar * statusBar, const std::string & filename, bool create);

    public:
        /**
         * Opens or creates a SQL database (see SqliteConnection for details)
         */
        static std::shared_ptr<SqliteConnection> construct(wxStatusBar * statusBar, const std::string & filename, bool create);

        void commit() override;
        void rollback() override;
    };
}

#endif