#ifndef PROG2_BELEG_GUI_EDITOR_PANEL_H
#define PROG2_BELEG_GUI_EDITOR_PANEL_H

#include <wx/wx.h>
#include <wx/aui/auibook.h>

namespace pb2 {
    /**
     * Base class for entity editors in the prog2-beleg GUI.
     *
     * This base class only provides the most bare-bones controls typically used for
     * editors, which is a "save" and a "close" button plus some boilerplate event
     * handling logic.
     */
    class EditorPanel : public wxPanel {
    private:
        void evAbortClicked(wxCommandEvent & event);
        void evSaveClicked(wxCommandEvent & event);

        void closeTab();

    protected:
        wxSizer * formControlsSizer = nullptr;
        virtual bool save() = 0;

    public:
        enum class ID { BTN_ABORT = 1, BTN_SAVE = 2 };

        EditorPanel(wxAuiNotebook * parent);

        DECLARE_EVENT_TABLE();
    };
}

#endif
