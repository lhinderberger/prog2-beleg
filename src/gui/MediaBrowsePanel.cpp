#include "gui/MediaBrowsePanel.h"

using namespace pb2;
using namespace std;

BEGIN_EVENT_TABLE(pb2::MediaBrowsePanel, wxPanel)
    EVT_BUTTON((int)MediaBrowsePanel::ID::BTN_ADD_TO_BASKET, MediaBrowsePanel::evAddToBasket)
END_EVENT_TABLE()

MediaBrowsePanel::MediaBrowsePanel(
        wxWindow * parent, shared_ptr<Database> database, shared_ptr<Basket> basket
) : wxPanel(parent, wxID_ANY), basket(basket) {
    /* Create top-level box sizer */
    wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    /* Add Search table */
    searchTable = new MediaSearchTable(this, wxID_ANY, database);
    sizer->Add(searchTable, 0, wxEXPAND);
    searchTable->list();

    /* Add Media controls */
    wxSizer * mediaControlsSizer = new wxBoxSizer(wxHORIZONTAL);
    mediaControlsSizer->Add(new wxButton(this, (int)ID::BTN_ADD_TO_BASKET, _("Zum Warenkorb hinzufügen")), 0, wxALIGN_RIGHT);

    sizer->Add(mediaControlsSizer, 0, wxALIGN_RIGHT);
}

void MediaBrowsePanel::evAddToBasket(wxCommandEvent & ev) {
    /* Retrieve mediumCopy */
    auto selectedMediumCopy = searchTable->getSelectedMediumCopy();
    if (!selectedMediumCopy)
        return;

    /* Ask user for confirmation if copy is not available */
    if (selectedMediumCopy->getAvailabilityHint() != "now" || selectedMediumCopy->getDeaccessioned()) {
        if (wxMessageBox(
                _("Das gewählte Medium scheint nicht verfügbar zu sein. Trotzdem zum Warenkorb hinzufügen?"),
                _("Warenkorb - Warnung"),
                wxYES_NO | wxICON_WARNING
        ) == wxNO)
            return;
    }

    /* Add to basket */
    basket->add(selectedMediumCopy);
}