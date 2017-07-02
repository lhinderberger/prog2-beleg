#ifndef PROG2_BELEG_GUI_MEDIA_EDITOR_PANEL_H
#define PROG2_BELEG_GUI_MEDIA_EDITOR_PANEL_H

#include "core/domain/MediumCopy.h"
#include "EditorPanel.h"

namespace pb2 {
    /*
     * Editor panel for MediumCopy entities - can also create Medium and Author entities
     */
    class MediaEditorPanel : public EditorPanel {
    private:
        std::shared_ptr<MediumCopy> editMediumCopy;
        std::shared_ptr<Database> createDatabase;

        wxTextCtrl * eanTextBox = nullptr;
        wxTextCtrl * titleTextBox = nullptr;
        wxTextCtrl * subtitleTextBox = nullptr;
        wxTextCtrl * authorFirstNameTextBox = nullptr;
        wxTextCtrl * authorLastNameTextBox = nullptr;
        wxTextCtrl * locationTextBox = nullptr;
        wxComboBox * typeComboBox = nullptr;
        wxComboBox * formatComboBox = nullptr;
        wxCheckBox * accessionedCheckBox = nullptr;

        void buildFormControls(); // Called by constructors

        /* Determines whether a new item is being created or an existing one edited */
        inline bool creating() const { return createDatabase != nullptr; }

    protected:
        virtual bool save() override;


    public:
        /**
         * Creates a MediaEditorPanel for an existing MediumCopy
         */
        MediaEditorPanel(wxAuiNotebook * parent, std::shared_ptr<MediumCopy> mediumCopy);

        /**
         * Creates a MediaEditorPanel, creating a new Medium + MediumCopy
         */
        MediaEditorPanel(wxAuiNotebook * parent, std::shared_ptr<Database> database);
    };
}

#endif
