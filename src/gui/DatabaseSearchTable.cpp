#include "gui/DatabaseSearchTable.h"

using namespace pb2;

DatabaseSearchTable::DatabaseSearchTable(wxWindow * parent, const wxString & newItemButtonLabel
) : wxPanel(parent, wxID_ANY) {
    /* Create top-level box sizer */
    wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    /* Create search box + new item button */
    wxSizer * searchBoxSizer = new wxBoxSizer(wxHORIZONTAL);

    searchCtrl = new wxSearchCtrl(this, wxID_ANY);
    searchBoxSizer->Add(searchCtrl, 1, wxEXPAND);
    searchBoxSizer->Add(new wxButton(this, 12345 /* TODO */, newItemButtonLabel));

    sizer->Add(searchBoxSizer, 0, wxEXPAND);

    /* Create table */
    dataView = new wxDataViewListCtrl(this, wxID_ANY);
    dataView->SetMinSize(wxSize(0, 300));
    sizer->Add(dataView, 1, wxEXPAND);
}