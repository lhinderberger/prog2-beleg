#include "gui/DatabasePanel.h"
#include <wx/statline.h>

using namespace pb2;
using namespace std;

DatabasePanel::DatabasePanel(wxWindow * parent, shared_ptr<Database> database)
    : wxPanel(parent, wxID_ANY), database(database) {
    /* Create top-level box sizer */
    wxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
    SetSizer(sizer);

    /* Create sidebar box sizer */
    wxSizer * sidebarSizer = new wxBoxSizer(wxVERTICAL);
    sidebarSizer->SetMinSize(200,600);
    sizer->Add(sidebarSizer, 0, wxALL, 10);

    /* Create "New tab" buttons */
    sidebarSizer->Add(new wxStaticText(this, wxID_ANY, _("Neuer Tab:")));
    sidebarSizer->Add(new wxButton(this, wxID_ANY, _("Medienkatalog")), 0, wxEXPAND);
    sidebarSizer->Add(new wxButton(this, wxID_ANY, _("Benutzer && Ausleihen")), 0, wxEXPAND);

    /* Divider and BasketWindow */
    sidebarSizer->Add(new wxStaticLine(this));
    basketWindow = new BasketWindow(this);
    sidebarSizer->Add(basketWindow, 1, wxEXPAND);

    /* Create content wxNotebook */
    notebook = new wxNotebook(this, wxID_ANY);
    sizer->Add(notebook);

    /* Open new media browser tab by default */
    //TODO

    /* Fit to parent window */
    Fit();
}
