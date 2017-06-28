#include "gui/UserBrowsePanel.h"

using namespace pb2;
using namespace std;

UserBrowsePanel::UserBrowsePanel(wxWindow * parent, shared_ptr<Database> database)
    : wxPanel(parent, wxID_ANY) {
    /* Create top-level box sizer */
    wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    /* Add User table */
    searchTable = new UserSearchTable(this, wxID_ANY, database);
    sizer->Add(searchTable, 0, wxEXPAND);
    searchTable->list();

    /* Add Lendings table */
    //TODO
}