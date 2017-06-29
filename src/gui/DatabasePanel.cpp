#include "gui/DatabasePanel.h"
#include "gui/MediaBrowsePanel.h"
#include "gui/UserBrowsePanel.h"
#include <wx/statline.h>

using namespace pb2;
using namespace std;

wxBEGIN_EVENT_TABLE(pb2::DatabasePanel, wxPanel)
    EVT_BUTTON((int)DatabasePanel::ID::NEW_MEDIA_BROWSER_TAB, DatabasePanel::evNewMediaBrowserTab)
    EVT_BUTTON((int)DatabasePanel::ID::NEW_USER_BROWSER_TAB, DatabasePanel::evNewUserBrowserTab)
wxEND_EVENT_TABLE()

DatabasePanel::DatabasePanel(wxWindow * parent, shared_ptr<Database> database)
    : wxPanel(parent, wxID_ANY), database(database) {
    /* Initialize Basket */
    basket = make_shared<Basket>();

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
    basketWindow = new BasketPanel(this, basket);
    sidebarSizer->Add(basketWindow, 1, wxEXPAND);

    /* Create content wxNotebook */
    notebook = new wxAuiNotebook(this, wxID_ANY);
    sizer->Add(notebook, 1, wxEXPAND | wxBOTTOM | wxRIGHT, 10);

    /* Open new media browser tab by default */
    newMediaBrowserTab();

    /* Fit to parent window */
    Fit();
}

void DatabasePanel::newMediaBrowserTab() {
    notebook->AddPage(new MediaBrowsePanel(notebook, database, basket), _("Medienkatalog"));
    notebook->ChangeSelection(notebook->GetPageCount() - 1);
}

void DatabasePanel::newUserBrowserTab() {
    notebook->AddPage(new UserBrowsePanel(notebook, database), _("Benutzer & Ausleihen"));
    notebook->ChangeSelection(notebook->GetPageCount() - 1);
}
