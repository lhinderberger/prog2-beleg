#include "gui/UserBrowsePanel.h"

using namespace pb2;
using namespace std;

wxBEGIN_EVENT_TABLE(pb2::UserBrowsePanel, wxPanel)
    EVT_COMMAND((int)UserBrowsePanel::ID::USER_TABLE, PB2_EVT_ST_SELECTED, UserBrowsePanel::evUserSelected)
    EVT_BUTTON((int)UserBrowsePanel::ID::LENDINGS_EXTEND, UserBrowsePanel::evLendingExtend)
    EVT_BUTTON((int)UserBrowsePanel::ID::LENDINGS_RETURN, UserBrowsePanel::evLendingReturn)
wxEND_EVENT_TABLE()

UserBrowsePanel::UserBrowsePanel(
        wxWindow * parent, shared_ptr<Database> database, shared_ptr<Basket> basket
) : wxPanel(parent, wxID_ANY), basket(basket) {
    /* Create top-level box sizer */
    wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    /* Add User table */
    userTable = new UserSearchTable(this, (int)ID::USER_TABLE, database);
    sizer->Add(userTable, 2, wxEXPAND);
    userTable->list();

    /* Add Lendings heading */
    lendingsHeading = new wxStaticText(this, wxID_ANY, getLendingsHeading(false));
    sizer->Add(lendingsHeading, 0, wxTOP, 15);

    /* Add Lendings table */
    lendingsTable = new LendingsSearchTable(this, (int)ID::LENDINGS_TABLE, database, true);
    sizer->Add(lendingsTable, 1, wxEXPAND);

    /* Add Lendings controls */
    wxSizer * lendingsControlsSizer = new wxBoxSizer(wxHORIZONTAL);
    lendingsControlsSizer->Add(new wxButton(this, (int)ID::LENDINGS_EXTEND, _("Verlängerung...")));
    lendingsControlsSizer->Add(new wxButton(this, (int)ID::LENDINGS_RETURN, _("Rückgabe...")));
    sizer->Add(lendingsControlsSizer, 0, wxALIGN_RIGHT);
}

wxString UserBrowsePanel::getLendingsHeading(bool userSelected) const {
    static wxString baseHeading =  _("Aktive Ausleihen");
    static wxString selectUserMsg =  baseHeading + " - " + _("Bitte zuerst Nutzer auswählen!");

    /* Return only heading when a user is selected, otherwise return descriptive message */
    return userSelected ? baseHeading : selectUserMsg;
}

void UserBrowsePanel::evLendingExtend(wxCommandEvent & event) {
    /* Retrieve selected lending */
    auto lending = lendingsTable->getSelectedLending();
    if (!lending)
        return;

    /* Pass on control and reload on success */
    if (lendingsController.extend(lending))
        lendingsTable->reload();
}

void UserBrowsePanel::evLendingReturn(wxCommandEvent & event) {
    /* Retrieve selected lending */
    auto lending = lendingsTable->getSelectedLending();
    if (!lending)
        return;

    /* Pass on control and reload on success */
    if (lendingsController.returnL(lending))
        lendingsTable->reload();
}

void UserBrowsePanel::evUserSelected(wxCommandEvent & event) {
    /* Retrieve ID of selected user */
    int userId = userTable->getSelectedId();

    /* Update lendings heading and table */
    lendingsHeading->SetLabelText(getLendingsHeading(userId != 0));
    lendingsTable->setUserId(userId);
}