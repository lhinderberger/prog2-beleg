#include "gui/MediaBrowsePanel.h"

using namespace pb2;

MediaBrowsePanel::MediaBrowsePanel(wxWindow * parent)
    : wxPanel(parent, wxID_ANY) {
    /* Create top-level box sizer */
    wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    /* Add Search table */
    searchTable = new MediaSearchTable(this);
    sizer->Add(searchTable, 0, wxEXPAND);
}