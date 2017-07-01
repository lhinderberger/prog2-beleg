#include "gui/MediaBrowsePanel.h"

using namespace pb2;
using namespace std;

BEGIN_EVENT_TABLE(pb2::MediaBrowsePanel, wxPanel)
    EVT_BUTTON((int)MediaBrowsePanel::ID::BTN_ADD_TO_BASKET, MediaBrowsePanel::evAddToBasket)
    EVT_BUTTON((int)MediaBrowsePanel::ID::BTN_DELETE_MEDIUM, MediaBrowsePanel::evDeleteMedium)
    EVT_BUTTON((int)MediaBrowsePanel::ID::BTN_EDIT_MEDIUM, MediaBrowsePanel::evEditMedium)
    EVT_BUTTON((int)MediaBrowsePanel::ID::BTN_NEW_COPY, MediaBrowsePanel::evNewCopy)
END_EVENT_TABLE()

MediaBrowsePanel::MediaBrowsePanel(
        wxWindow * parent, shared_ptr<Database> database, shared_ptr<Basket> basket
) : BrowsePanel(parent), basket(basket) {
    /* Create top-level box sizer */
    wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    /* Add Search table */
    searchTable = new MediaSearchTable(this, wxID_ANY, database);
    sizer->Add(searchTable, 7, wxEXPAND);
    searchTable->list();

    /* Add Media controls */
    wxSizer * mediaControlsSizer = new wxBoxSizer(wxHORIZONTAL);
    mediaControlsSizer->Add(new wxButton(this, (int)ID::BTN_NEW_COPY, _("Exemplar duplizieren")), 0, wxALIGN_RIGHT);
    mediaControlsSizer->Add(new wxButton(this, (int)ID::BTN_DELETE_MEDIUM, _("Exemplar löschen...")), 0, wxALIGN_RIGHT);
    mediaControlsSizer->Add(new wxButton(this, (int)ID::BTN_EDIT_MEDIUM, _("Details / Bearbeiten...")), 0, wxALIGN_RIGHT);
    mediaControlsSizer->Add(new wxButton(this, (int)ID::BTN_ADD_TO_BASKET, _("Zum Warenkorb hinzufügen")), 0, wxALIGN_RIGHT);

    sizer->Add(mediaControlsSizer, 3, wxALIGN_RIGHT);
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

void MediaBrowsePanel::evDeleteMedium(wxCommandEvent & ev) {
    /* Retrieve mediumCopy */
    auto mediumCopy = searchTable->getSelectedMediumCopy();
    if (!mediumCopy)
        return;

    /* Ask user for confirmation */
    auto medium = mediumCopy->getMedium();
    wxString message;
    message.Printf(
            _(
                    "Wollen Sie das Medium %s/%d (%s: %s) wirklich löschen?\n"
                    "Dadurch werden jegliche Daten gelöscht, die mit dem Medium in Beziehung stehen.\n"
                    "Dies kann nicht mehr rückgängig gemacht werden!\n\n"
                    "Hinweis: Sie können auch ein Medium nur vorübergehend aus dem Bestand nehmen. Klicken Sie hierzu auf Details/Bearbeiten..."
            ),
            medium->getEAN(), mediumCopy->getSerialNumber(), medium->getAuthor()->getLastName(), medium->getTitle()
    );
    if (wxMessageBox(message, _("Medium löschen"), wxICON_WARNING | wxYES_NO) != wxYES)
        return;

    /* Delete MediumCopy */
    mediumCopy->del();

    /* Delete Medium as well, if it is the last copy */
    if (medium->queryCopies().size() == 0)
        medium->del();

    /* Commit and refresh */
    medium->getConnection()->commit();
    refreshCascade();
}

void MediaBrowsePanel::evEditMedium(wxCommandEvent & ev) {
    throw NotImplementedException();
}

void MediaBrowsePanel::evNewCopy(wxCommandEvent & ev) {

}

void MediaBrowsePanel::refresh() {
    searchTable->reload();
}