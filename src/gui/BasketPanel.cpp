#include <core/exceptions.h>
#include "gui/BasketPanel.h"

using namespace pb2;
using namespace std;

BEGIN_EVENT_TABLE(pb2::BasketPanel, wxPanel)
    EVT_BUTTON((int)BasketPanel::ID::REMOVE_BTN, BasketPanel::evRemove)
END_EVENT_TABLE()

BasketPanel::BasketPanel(wxWindow * parent, shared_ptr<Basket> basket)
    : wxPanel(parent, wxID_ANY), basket(basket) {
    /* Create top-level box sizer */
    wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    /* Heading */
    sizer->Add(new wxStaticText(this, wxID_ANY, _("Warenkorb:")));

    /* List box */
    listBox = new wxListBox(this, wxID_ANY);
    sizer->Add(listBox, 1 , wxEXPAND | wxALL);

    /* "Remove" button */
    wxButton * removeButton = new wxButton(this, (int)BasketPanel::ID::REMOVE_BTN, _("Aus Warenkorb entfernen"));
    sizer->Add(removeButton);

    /* Add Basket listeners */
    basket->addListener([this](BasketEventType type, shared_ptr<MediumCopy> mediumCopy){
        rebuildListBox();
    });

    Fit();
}

void BasketPanel::evRemove(wxCommandEvent & ev) {
    /* Get current list item */
    int index = listBox->GetSelection();
    if (index == wxNOT_FOUND || index < 0)
        return;

    // Raw pointer is only for identification of MediumCopy! Never dereference this!
    auto * rawPtr = (MediumCopy*)listBox->GetClientData((unsigned int)index);
    if (!rawPtr)
        return;

    /* Remove item from basket */
    basket->remove(rawPtr);

    // No need to rebuild list box - the listener defined in the constructor will take care of this
}

void BasketPanel::rebuildListBox() {
    /* Clear first */
    listBox->Clear();

    /* Rebuild list box from basket */
    for (const auto & mediumCopy : basket->getList()) {
        if (!mediumCopy)
            continue;

        auto medium = mediumCopy->getMedium();
        wxString label;
        label.Printf(_("%s, %s (S/N %d)"), medium->getAuthor()->getLastName(), medium->getTitle(), mediumCopy->getSerialNumber());
        listBox->Append(label, mediumCopy.get());
    }
}
