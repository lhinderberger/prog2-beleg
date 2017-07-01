#ifndef PROG2_BELEG_USER_BROWSE_PANEL_H
#define PROG2_BELEG_USER_BROWSE_PANEL_H

#include <wx/wx.h>

#include "core/Database.h"
#include "Basket.h"
#include "BrowsePanel.h"
#include "LendingsController.h"
#include "LendingsSearchTable.h"
#include "UserSearchTable.h"

namespace pb2 {
    class UserBrowsePanel : public BrowsePanel {
    private:
        LendingsController lendingsController;
        LendingsSearchTable * lendingsTable = nullptr;
        UserSearchTable * userTable = nullptr;
        wxStaticText * lendingsHeading = nullptr;

        std::shared_ptr<Basket> basket;

        wxString getLendingsHeading(bool userSelected) const;

        void evLendingExtend(wxCommandEvent & event);
        void evLendingReturn(wxCommandEvent & event);
        void evNewLending(wxCommandEvent & event);
        void evUserSelected(wxCommandEvent & event);
        void evDeleteUser(wxCommandEvent & event);
        void evEditUser(wxCommandEvent & event);

    public:
        enum class ID {
            USER_TABLE = 1, LENDINGS_TABLE, LENDINGS_EXTEND, LENDINGS_RETURN,
            BTN_DELETE_USER, BTN_EDIT_USER
        };

        UserBrowsePanel(
                wxWindow * parent, std::shared_ptr<Database> database,
                std::shared_ptr<Basket> basket
        );

        virtual void refresh() override;

    wxDECLARE_EVENT_TABLE();
    };
}

#endif
