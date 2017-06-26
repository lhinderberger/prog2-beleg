#include "gui/DatabasePanel.h"
#include "gui/MediaBrowsePanel.h"
#include <wx/statline.h>

using namespace pb2;
using namespace std;

wxBEGIN_EVENT_TABLE(pb2::DatabasePanel, wxWindow)
    EVT_BUTTON((int)DatabasePanel::ID::NEW_MEDIA_BROWSER_TAB, DatabasePanel::evNewMediaBrowserTab)
wxEND_EVENT_TABLE()

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
    sidebarSizer->Add(new wxButton(this, (int)ID::NEW_MEDIA_BROWSER_TAB, _("Medienkatalog")), 0, wxEXPAND);
    sidebarSizer->Add(new wxButton(this, (int)ID::NEW_USER_BROWSER_TAB, _("Benutzer && Ausleihen")), 0, wxEXPAND);

    /* Divider and BasketPanel */
    sidebarSizer->Add(new wxStaticLine(this));
    basketWindow = new BasketPanel(this);
    sidebarSizer->Add(basketWindow, 1, wxEXPAND);

    /* Create content wxNotebook */
    notebook = new wxNotebook(this, wxID_ANY);
    sizer->Add(notebook);

    /* Open new media browser tab by default */
    newMediaBrowserTab();

    /* Fit to parent window */
    Fit();
}

void DatabasePanel::newMediaBrowserTab() {
    notebook->AddPage(new MediaBrowsePanel(notebook), _("Medienkatalog"));
}

void DatabasePanel::newUserBrowserTab() {
    //TODO Implement
    throw NotImplementedException();
}
