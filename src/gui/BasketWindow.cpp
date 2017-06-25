#include "gui/BasketWindow.h"

using namespace pb2;

BasketWindow::BasketWindow(wxWindow * parent)
    : wxWindow(parent, wxID_ANY) {
    /* Create top-level box sizer */
    wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    /* Heading */
    sizer->Add(new wxStaticText(this, wxID_ANY, _("Warenkorb:")));

    /* List box */
    listBox = new wxListBox(this, wxID_ANY);
    sizer->Add(listBox, 1 , wxEXPAND | wxALL);

    /* "Remove" button */
    wxButton * removeButton = new wxButton(this, 12345 /* TODO */, _("Aus Warenkorb entfernen"));
    sizer->Add(removeButton);

    Fit();
}