#include "gui/MediaBrowsePanel.h"

using namespace pb2;
using namespace std;

MediaBrowsePanel::MediaBrowsePanel(wxWindow * parent, shared_ptr<Database> database)
    : wxPanel(parent, wxID_ANY) {
    /* Create top-level box sizer */
    wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    /* Add Search table */
    searchTable = new MediaSearchTable(this, wxID_ANY, database);
    sizer->Add(searchTable, 0, wxEXPAND);
    searchTable->list();
}