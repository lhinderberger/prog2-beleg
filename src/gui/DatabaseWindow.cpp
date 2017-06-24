#include "gui/DatabaseWindow.h"

using namespace pb2;
using namespace std;

DatabaseWindow::DatabaseWindow(wxWindow * parent, shared_ptr<Database> database)
    : wxWindow(parent, wxID_ANY), database(database) {
    /* Create top-level box sizer */
    wxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
    SetSizer(sizer);

    /* Create menu box sizer */
    wxSizer * menuSizer = new wxBoxSizer(wxVERTICAL);
    menuSizer->SetMinSize(200,300);
    sizer->Add(menuSizer);
    menuSizer->Add(new wxButton(this, 0, _("Medienkatalog")), 0, wxEXPAND);
    menuSizer->Add(new wxButton(this, 0, _("Benutzer & Ausleihen")), 0, wxEXPAND);

    /* Create content window */
    contentWindow = new wxWindow(this, wxID_ANY);
    contentWindow->SetWindowStyle(wxBORDER_SUNKEN);
    sizer->Add(contentWindow);

    /* Fit to parent window */
    Fit();
}