#ifndef PROG2_BELEG_USER_BROWSE_PANEL_H
#define PROG2_BELEG_USER_BROWSE_PANEL_H

#include <wx/wx.h>

#include "core/Database.h"
#include "UserSearchTable.h"

namespace pb2 {
    class UserBrowsePanel : public wxPanel {
    private:
        UserSearchTable * searchTable = nullptr;

    public:
        UserBrowsePanel(wxWindow * parent, std::shared_ptr<Database> database);
    };
}

#endif
