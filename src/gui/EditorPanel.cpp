#include "gui/EditorPanel.h"

using namespace std;
using namespace pb2;

BEGIN_EVENT_TABLE(pb2::EditorPanel, wxPanel)
    EVT_BUTTON((int)EditorPanel::ID::BTN_ABORT, EditorPanel::evAbortClicked)
    EVT_BUTTON((int)EditorPanel::ID::BTN_SAVE, EditorPanel::evSaveClicked)
END_EVENT_TABLE()

EditorPanel::EditorPanel(wxAuiNotebook * parent)
    : wxPanel(parent, wxID_ANY) {

    wxSizer * mainSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(mainSizer);

    /* Add form controls sizer */
    formControlsSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(formControlsSizer, 1);

    /* Add editor buttons sizer */
    wxSizer * editorButtonsSizer = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(editorButtonsSizer, 0, wxTOP, 15);

    /* Add editor buttons */
    editorButtonsSizer->Add(new wxButton(this, (int)ID::BTN_ABORT, _("Abbrechen")), 0, wxALIGN_RIGHT);
    editorButtonsSizer->Add(new wxButton(this, (int)ID::BTN_SAVE, _("Speichern")), 0, wxALIGN_RIGHT);
}

void EditorPanel::evAbortClicked(wxCommandEvent & event) {
    closeTab();
}

void EditorPanel::evSaveClicked(wxCommandEvent & event) {
    save();
    closeTab();
}

void EditorPanel::closeTab() {
    auto* parent = (wxAuiNotebook*)GetParent();
    parent->DeletePage((size_t)parent->GetPageIndex(this));
}
