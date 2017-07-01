#include "gui/EditorPanel.h"

using namespace std;
using namespace pb2;

BEGIN_EVENT_TABLE(pb2::EditorPanel, wxPanel)
    EVT_BUTTON((int)EditorPanel::ID::BTN_ABORT, EditorPanel::evAbortClicked)
    EVT_BUTTON((int)EditorPanel::ID::BTN_SAVE, EditorPanel::evSaveClicked)
    EVT_CLOSE(EditorPanel::evPanelClosing)
END_EVENT_TABLE()

EditorPanel::EditorPanel(wxWindow * parent)
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
    Close(true); // Force Close - Don't call event handler
}

void EditorPanel::evSaveClicked(wxCommandEvent & event) {
    save();
    Close(true); // Force Close - Don't call event handler
}

void EditorPanel::evPanelClosing(wxCloseEvent & event) {
    if (!event.CanVeto()) {
        event.Skip();
        return;
    }

    /* Ask user whether they want to save their changes */
    int confirmation = wxMessageBox(
            _("Wollen Sie Ihre Änderungen speichern?"),
            _("Editor schließen"),
            wxYES_NO | wxCANCEL
    );

    if (confirmation == wxCANCEL) {
        event.Veto();
        return;
    } else if (confirmation == wxYES)
        save();

    event.Skip();
}
