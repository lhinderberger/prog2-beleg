#ifndef PROG2_BELEG_GUI_USER_EDITOR_PANEL_H
#define PROG2_BELEG_GUI_USER_EDITOR_PANEL_H

#include "core/domain/LibraryUser.h"
#include "EditorPanel.h"

namespace pb2 {
    /**
     * EditorPanel for LibraryUser entities - also creates PostalAddress entities.
     */
    class UserEditorPanel : public EditorPanel {
    private:
        std::shared_ptr<LibraryUser> editUser;
        std::shared_ptr<Database> createDatabase;

        wxTextCtrl * firstNameTextBox = nullptr;
        wxTextCtrl * lastNameTextBox = nullptr;
        wxTextCtrl * streetTextBox = nullptr;
        wxTextCtrl * houseNumberTextBox = nullptr;
        wxTextCtrl * zipTextBox = nullptr;
        wxTextCtrl * cityTextBox = nullptr;
        wxTextCtrl * phoneTextBox = nullptr;

        void buildFormControls(); // Called by constructors

        /* Determines whether a new item is being created or an existing one edited */
        inline bool creating() const { return createDatabase != nullptr; }

    protected:
        virtual bool save() override;


    public:
        /**
         * Creates a UserEditorPanel for an existing LibraryUser
         */
        UserEditorPanel(wxAuiNotebook * parent, std::shared_ptr<LibraryUser> libraryUser);

        /**
         * Creates a UserEditorPanel, creating a new LibraryUser
         */
        UserEditorPanel(wxAuiNotebook * parent, std::shared_ptr<Database> database);
    };
}

#endif
