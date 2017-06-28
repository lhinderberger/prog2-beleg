#ifndef PROG2_BELEG_USER_BROWSE_PANEL_H
#define PROG2_BELEG_USER_BROWSE_PANEL_H

#include <wx/wx.h>

#include "core/Database.h"
#include "LendingsSearchTable.h"
#include "UserSearchTable.h"

namespace pb2 {
    class UserBrowsePanel : public wxPanel {
    private:
        LendingsSearchTable * lendingsTable = nullptr;
        UserSearchTable * userTable = nullptr;
        wxStaticText * lendingsHeading = nullptr;

        wxString getLendingsHeading(bool userSelected) const;

        void evUserSelected(wxCommandEvent & event);

    public:
        enum class ID { USER_TABLE = 1, LENDINGS_TABLE };

        UserBrowsePanel(wxWindow * parent, std::shared_ptr<Database> database);

    wxDECLARE_EVENT_TABLE();
    };
}

#endif
