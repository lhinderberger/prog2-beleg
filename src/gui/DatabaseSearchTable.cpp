#include "gui/DatabaseSearchTable.h"

using namespace pb2;
using namespace std;

wxDEFINE_EVENT(PB2_EVT_ST_SELECTED, wxCommandEvent);
wxDEFINE_EVENT(PB2_EVT_ST_NEW_ITEM, wxCommandEvent);

wxBEGIN_EVENT_TABLE(pb2::DatabaseSearchTable, wxPanel)
    EVT_SEARCHCTRL_SEARCH_BTN((int)DatabaseSearchTable::ID::SEARCH_CTRL, DatabaseSearchTable::evSearch)
    EVT_TEXT_ENTER((int)DatabaseSearchTable::ID::SEARCH_CTRL, DatabaseSearchTable::evSearch)
    EVT_BUTTON((int)DatabaseSearchTable::ID::NEW_ITEM_BUTTON, DatabaseSearchTable::evNewItem)
    EVT_DATAVIEW_SELECTION_CHANGED((int)DatabaseSearchTable::ID::DATA_VIEW, DatabaseSearchTable::evRowSelected)
wxEND_EVENT_TABLE()

DatabaseSearchTable::DatabaseSearchTable(
    wxWindow * parent, wxWindowID id, const wxString & newItemButtonLabel, const vector<wxString> & columnHeadings
) : wxPanel(parent, id) {
    /* Create top-level box sizer */
    wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    /* Create search box + new item button */
    wxSizer * searchBoxSizer = new wxBoxSizer(wxHORIZONTAL);

    searchCtrl = new wxSearchCtrl(this, (int)ID::SEARCH_CTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    searchBoxSizer->Add(searchCtrl, 1, wxEXPAND);
    searchBoxSizer->Add(new wxButton(this, (int)ID::NEW_ITEM_BUTTON, newItemButtonLabel));

    sizer->Add(searchBoxSizer, 0, wxEXPAND);

    /* Create table */
    dataView = new wxDataViewListCtrl(this, (int)ID::DATA_VIEW);
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

void DatabaseSearchTable::evNewItem(wxCommandEvent & ev) {
    /* Generate and send an ST_NEW_ITEM event */
    wxCommandEvent event(PB2_EVT_ST_NEW_ITEM, GetId());
    wxPostEvent(GetParent(), event);
}

void DatabaseSearchTable::evRowSelected(wxDataViewEvent & ev) {
    /* Generate and send an ST_SELECTED event */
    wxCommandEvent event(PB2_EVT_ST_SELECTED, GetId());
    wxPostEvent(GetParent(), event);
}

void DatabaseSearchTable::list() {
    prepareList();
    buildRows();
}

void DatabaseSearchTable::search(const wxString & query) {
    prepareSearch(query);
    buildRows();
}