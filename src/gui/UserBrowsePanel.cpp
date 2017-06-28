#include "gui/UserBrowsePanel.h"

using namespace pb2;
using namespace std;

wxBEGIN_EVENT_TABLE(pb2::UserBrowsePanel, wxPanel)
    EVT_COMMAND((int)UserBrowsePanel::ID::USER_TABLE, PB2_EVT_ST_SELECTED, UserBrowsePanel::evUserSelected)
wxEND_EVENT_TABLE()

UserBrowsePanel::UserBrowsePanel(wxWindow * parent, shared_ptr<Database> database)
        : wxPanel(parent, wxID_ANY) {
    /* Create top-level box sizer */
    wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    /* Add User table */
    userTable = new UserSearchTable(this, (int)ID::USER_TABLE, database);
    sizer->Add(userTable, 0, wxEXPAND);
    userTable->list();

    /* Add Lendings heading */
    lendingsHeading = new wxStaticText(this, wxID_ANY, getLendingsHeading(false));
    sizer->Add(lendingsHeading, 0, wxTOP, 15);

    /* Add Lendings table */
    lendingsTable = new LendingsSearchTable(this, (int)ID::LENDINGS_TABLE, database, true);
    sizer->Add(lendingsTable, 0, wxEXPAND);
}

wxString UserBrowsePanel::getLendingsHeading(bool userSelected) const {
    static wxString baseHeading =  _("Aktive Ausleihen");
    static wxString selectUserMsg =  baseHeading + " - " + _("Bitte zuerst Nutzer auswählen!");

    /* Return only heading when a user is selected, otherwise return descriptive message */
    return userSelected ? baseHeading : selectUserMsg;
}

void UserBrowsePanel::evUserSelected(wxCommandEvent & event) {
    /* Retrieve ID of selected user */
    int userId = userTable->getSelectedId();

    /* Update lendings heading and table */
    lendingsHeading->SetLabelText(getLendingsHeading(userId != 0));
    lendingsTable->setUserId(userId);
}