#include "gui/DatabaseSearchTable.h"

using namespace pb2;
using namespace std;

wxBEGIN_EVENT_TABLE(pb2::DatabaseSearchTable, wxWindow)
    EVT_SEARCHCTRL_SEARCH_BTN((int)DatabaseSearchTable::ID::SEARCH_CTRL, DatabaseSearchTable::evSearch)
    EVT_TEXT_ENTER((int)DatabaseSearchTable::ID::SEARCH_CTRL, DatabaseSearchTable::evSearch)
wxEND_EVENT_TABLE()

DatabaseSearchTable::DatabaseSearchTable(
    wxWindow * parent, const wxString & newItemButtonLabel, const vector<wxString> & columnHeadings
) : wxPanel(parent, wxID_ANY) {
    /* Create top-level box sizer */
    wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    /* Create search box + new item button */
    wxSizer * searchBoxSizer = new wxBoxSizer(wxHORIZONTAL);

    searchCtrl = new wxSearchCtrl(this, (int)ID::SEARCH_CTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    searchBoxSizer->Add(searchCtrl, 1, wxEXPAND);
    searchBoxSizer->Add(new wxButton(this, 12345 /* TODO */, newItemButtonLabel));

    sizer->Add(searchBoxSizer, 0, wxEXPAND);

    /* Create table */
    dataView = new wxDataViewListCtrl(this, wxID_ANY);
    dataView->SetMinSize(wxSize(0, 300));
    buildColumns(columnHeadings);
    sizer->Add(dataView, 1, wxEXPAND);
}

void DatabaseSearchTable::buildColumns(const vector<wxString> & headings) {
    for (const auto & heading : headings)
        dataView->AppendTextColumn(heading);
}

void DatabaseSearchTable::buildRows() {
    dataView->DeleteAllItems();

    unsigned columnCount = dataView->GetColumnCount();
    while (step()) {
        /* Build values vector */
        wxVector<wxVariant> values;
        for (unsigned i = 0; i < columnCount; i++)
            values.push_back(getColumnContent(i));

        /* Append to dataView */
        dataView->AppendItem(values);
    }
}

void DatabaseSearchTable::evSearch(wxCommandEvent & ev) {
    wxString query = ev.GetString().Trim();
    if (query.IsEmpty())
        list();
    else
        search(query);
}

void DatabaseSearchTable::list() {
    prepareList();
    buildRows();
}

void DatabaseSearchTable::search(const wxString & query) {
    prepareSearch(query);
    buildRows();
}