#ifndef PROG2_BELEG_GUI_EDITOR_PANEL_H
#define PROG2_BELEG_GUI_EDITOR_PANEL_H

#include <wx/wx.h>
#include <wx/aui/auibook.h>

namespace pb2 {
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
