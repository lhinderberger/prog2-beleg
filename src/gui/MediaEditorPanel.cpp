#include "gui/MediaEditorPanel.h"
#include "gui/translations.h"

#include "core/domain/Book.h"
#include "core/domain/Software.h"
#include "core/domain/Audio.h"
#include "core/domain/Video.h"

const char * types[4] = {"book", "software", "audio", "video"};

using namespace pb2;
using namespace std;

MediaEditorPanel::MediaEditorPanel(wxAuiNotebook * parent, shared_ptr<MediumCopy> mediumCopy)
    : EditorPanel(parent), editMediumCopy(mediumCopy)
{
    buildFormControls();
}

MediaEditorPanel::MediaEditorPanel(wxAuiNotebook * parent, shared_ptr<Database> database)
    : EditorPanel(parent), createDatabase(database)
{
    buildFormControls();
}

void MediaEditorPanel::buildFormControls() {
    /* Initialize form flex grid sizer */
    wxFlexGridSizer * sizer = new wxFlexGridSizer(12, 2, 10, 5);
    formControlsSizer->Add(sizer, 1, wxEXPAND);

    /* Prepare building helpers */
    auto medium = creating() ? nullptr : editMediumCopy->getMedium();
    auto author = creating() ? nullptr : medium->getAuthor();

    auto buildLabel = [this, sizer](const wxString & caption) {
        sizer->Add(new wxStaticText(this, wxID_ANY, caption));
    };

    auto buildTextBox = [this, sizer, buildLabel](wxTextCtrl ** out, int id, const wxString & caption, const wxString & value) {
        buildLabel(caption);
        *out = new wxTextCtrl(this, wxID_ANY, value);
        sizer->Add(*out, 1, wxEXPAND);
        (*out)->SetMinSize(wxSize(300, 30));
    };

    /* Build Type field */
    buildLabel(_("Typ:"));
    if (creating()) {
        typeComboBox = new wxComboBox(this, wxID_ANY);
        sizer->Add(typeComboBox, 1, wxEXPAND);

        /* Initialize combo box */
        for (auto& type : types)
            typeComboBox->Append(mediaTypeTranslation(type), (void*)type);
    } else
        sizer->Add(new wxStaticText(this, wxID_ANY, mediaTypeTranslation(medium->getType())));

    /* Build EAN field */
    buildLabel(_("EAN / ISBN*:"));
    if (creating()) {
        eanTextBox = new wxTextCtrl(this, wxID_ANY);
        sizer->Add(eanTextBox, 1, wxEXPAND);
    } else
        sizer->Add(new wxStaticText(this, wxID_ANY, medium->getEAN()));

    /* Build title fields */
    buildTextBox(&titleTextBox, wxID_ANY, _("Titel*:"), creating() ? "" : medium->getTitle());
    buildTextBox(&subtitleTextBox, wxID_ANY, _("Untertitel*:"), creating() ? "" : medium->getSubtitle());

    /* Build format field */
    buildLabel(_("Format:"));
    formatComboBox = new wxComboBox(this, wxID_ANY);
    sizer->Add(formatComboBox, 1, wxEXPAND);

    /* Initialize combo box */
    const auto & allowedFormats = Medium::allowedFormats();
    for (const string & format : allowedFormats) {
        int index = formatComboBox->Append(mediaFormatTranslation(format), (void *) &format);
        if (!creating() && format == medium->getFormat())
            formatComboBox->Select(index);
    }

    /* Section author fields */
    buildLabel(_("Autor:"));
    buildLabel("");

    buildTextBox(&authorFirstNameTextBox, wxID_ANY, _("Vorname*:"), creating() ? "" : author->getFirstName());
    buildTextBox(&authorLastNameTextBox, wxID_ANY, _("Nachname*:"), creating() ? "" : author->getLastName());

    /* Section copy fields */
    buildLabel(_("Exemplar:"));
    buildLabel("");

    /* Build serial number field */
    wxString serialNumberStr;
    serialNumberStr.Printf("%d", creating() ? 1 : editMediumCopy->getSerialNumber());
    buildLabel(_("Serien-Nr.:"));
    buildLabel(serialNumberStr);

    /* Build location field */
    buildTextBox(&locationTextBox, wxID_ANY, _("Standort*:"), creating() ? "" : editMediumCopy->getLocation());

    /* Build deaccessioned field */
    buildLabel("");
    accessionedCheckBox = new wxCheckBox(this, wxID_ANY, _("Exemplar ist im Bestand"));
    accessionedCheckBox->SetValue(creating() || !editMediumCopy->getDeaccessioned());
    sizer->Add(accessionedCheckBox);
}

bool MediaEditorPanel::checkMandatoryFields() {
    vector<wxTextCtrl*> mandatory = {
            titleTextBox, subtitleTextBox, authorFirstNameTextBox, authorLastNameTextBox,
            locationTextBox
    };

    if (eanTextBox)
        mandatory.push_back(eanTextBox);

    /* Check default mandatory fields */
    for (wxTextCtrl * m : mandatory) {
        if (m->IsEmpty()) {
            defaultMandatoryError();
            return false;
        }
    }

    return true;
}

bool MediaEditorPanel::save() {
    auto database = creating() ? createDatabase : editMediumCopy->getDatabase();

    auto selectedClientData = [](wxComboBox * comboBox) {
        void * result = nullptr;
        int index = comboBox->GetSelection();
        if (index >= 0)
            result = comboBox->GetClientData((unsigned int)index);
        return result;
    };

    try {
        auto mediumCopyFactory = DatabaseObjectFactory<MediumCopy>(database);
        auto authorFactory = DatabaseObjectFactory<Author>(database);

        shared_ptr<Medium> medium = nullptr;
        shared_ptr<Author> author = nullptr;
        shared_ptr<MediumCopy> mediumCopy = nullptr;

        if (creating()) {
            /* Determine medium type + construct medium */
            const char * clientData = (const char*)selectedClientData(typeComboBox);
            if (!clientData)
                throw ValidationException("Es muss ein Typ angegeben werden!");
            else {
                string type = clientData;
                string ean = eanTextBox->GetValue().ToStdString();
                if (type == "book")
                    medium = DatabaseObjectFactory<Book>(createDatabase).construct(ean);
                else if (type == "software")
                    medium = DatabaseObjectFactory<Software>(createDatabase).construct(ean);
                else if (type == "audio")
                    medium = DatabaseObjectFactory<Audio>(createDatabase).construct(ean);
                else if (type == "video")
                    medium = DatabaseObjectFactory<Video>(createDatabase).construct(ean);
                else
                    throw LogicError("Invalid type: " + type);
            }

            /* Construct new mediumCopy / author objects */
            mediumCopy = mediumCopyFactory.construct(medium, 1);
            author = authorFactory.construct(-1);
        }
        else {
            mediumCopy = editMediumCopy;
            medium = mediumCopy->getMedium();
            author = medium->getAuthor();
        }

        /* Set values (validation using ValidationException) */
        medium->setTitle(titleTextBox->GetValue().ToStdString());
        medium->setSubtitle(subtitleTextBox->GetValue().ToStdString());

        string * fmtClientData = (string*)selectedClientData(formatComboBox);
        if (!fmtClientData)
            throw ValidationException("Es muss ein Format angegeben werden!");
        medium->setFormat(*fmtClientData);

        string authorFirstName = authorFirstNameTextBox->GetValue().ToStdString();
        string authorLastName = authorLastNameTextBox->GetValue().ToStdString();
        if (authorFirstName != author->getFirstName() || authorLastName != author->getLastName()) {
            // Replace author, do not change author - otherwise this would cause undesirable side effects
            author = authorFactory.construct(-1);
            author->setFirstName(authorFirstName);
            author->setLastName(authorLastName);
            medium->setAuthor(author);
        }

        mediumCopy->setLocation(locationTextBox->GetValue().ToStdString());
        mediumCopy->setDeaccessioned(!accessionedCheckBox->GetValue());

        /* Try to persist all to database */
        author->persist();
        medium->persist();
        mediumCopy->persist();

        medium->getDatabase()->getConnection()->commit();
        return true;

    }
    catch (pb2::CoreException & e) {
        database->getConnection()->rollback();

        wxMessageBox(_("Ein Fehler ist aufgetreten:\n\n") + e.what(), _("Fehler"), wxICON_ERROR);
        return false;
    }
}
