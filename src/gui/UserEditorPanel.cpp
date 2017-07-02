#include "gui/UserEditorPanel.h"

using namespace pb2;
using namespace std;

UserEditorPanel::UserEditorPanel(wxAuiNotebook * parent, shared_ptr<LibraryUser> libraryUser)
    : EditorPanel(parent), editUser(libraryUser)
{
    buildFormControls();
}

UserEditorPanel::UserEditorPanel(wxAuiNotebook * parent, shared_ptr<Database> database)
    : EditorPanel(parent), createDatabase(database)
{
    buildFormControls();
}

void UserEditorPanel::buildFormControls() {
    /* Initialize form flex grid sizer */
    wxFlexGridSizer * sizer = new wxFlexGridSizer(7, 2, 10, 5);
    formControlsSizer->Add(sizer, 1, wxEXPAND);

    /* Prepare building helpers */
    auto address = creating() ? nullptr : editUser->getPostalAddress();

    auto buildLabel = [this, sizer](const wxString & caption) {
        sizer->Add(new wxStaticText(this, wxID_ANY, caption));
    };

    auto buildTextBox = [this, sizer, buildLabel](wxTextCtrl ** out, int id, const wxString & caption, const wxString & value) {
        buildLabel(caption);
        *out = new wxTextCtrl(this, wxID_ANY, value);
        sizer->Add(*out, 1, wxEXPAND);
        (*out)->SetMinSize(wxSize(300, 30));
    };

    /* Build user ID field */
    wxString idLabel = _("Wird automatisch generiert");
    if (!creating())
        idLabel.Printf("%d", editUser->getId());
    buildLabel(_("Benutzer-Nr.:"));
    buildLabel(idLabel);

    /* Build name text boxes */
    buildTextBox(&firstNameTextBox, wxID_ANY, _("Vorname*:"), creating() ? "" : editUser->getFirstName());
    buildTextBox(&lastNameTextBox, wxID_ANY, _("Nachname*:"), creating() ? "" : editUser->getLastName());
    buildTextBox(&phoneTextBox, wxID_ANY, _("Telefon*:"), creating() ? "" : editUser->getTelephone());

    /* Build Postal address controls */
    buildLabel(_("Anschrift"));
    buildLabel("");

    streetTextBox = new wxTextCtrl(this, wxID_ANY, creating() ? "" : address->getStreet());
    houseNumberTextBox = new wxTextCtrl(this, wxID_ANY, creating() ? "" : address->getHouseNumber());
    zipTextBox = new wxTextCtrl(this, wxID_ANY, creating() ? "" : address->getZip());
    cityTextBox = new wxTextCtrl(this, wxID_ANY, creating() ? "" : address->getCity());

    wxBoxSizer * streetSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer * citySizer = new wxBoxSizer(wxHORIZONTAL);

    streetSizer->Add(streetTextBox, 3);
    streetSizer->Add(houseNumberTextBox, 1);
    citySizer->Add(zipTextBox, 1);
    citySizer->Add(cityTextBox, 3);

    buildLabel(_("Straße / Nr.*:"));
    sizer->Add(streetSizer);
    buildLabel(_("PLZ / Ort.*:"));
    sizer->Add(citySizer);
}

bool UserEditorPanel::checkMandatoryFields() {
    vector<wxTextCtrl*> mandatory = {
            firstNameTextBox, lastNameTextBox, streetTextBox, houseNumberTextBox,
            zipTextBox, cityTextBox, phoneTextBox
    };

    /* Check default mandatory fields */
    for (wxTextCtrl * m : mandatory) {
        if (m->IsEmpty()) {
            defaultMandatoryError();
            return false;
        }
    }

    return true;
}

bool UserEditorPanel::save() {
    auto database = creating() ? createDatabase : editUser->getDatabase();

    try {
        auto addressFactory = DatabaseObjectFactory<PostalAddress>(database);
        auto userFactory = DatabaseObjectFactory<LibraryUser>(database);

        shared_ptr<LibraryUser> user = nullptr;
        shared_ptr<PostalAddress> address = nullptr;

        if (creating()) {
            user = userFactory.construct(-1);
            address = addressFactory.construct(-1);
        }
        else {
            user = editUser;
            address = user->getPostalAddress();
        }

        /* Set values (validation using ValidationException) */
        user->setFirstName(firstNameTextBox->GetValue().ToStdString());
        user->setLastName(lastNameTextBox->GetValue().ToStdString());
        user->setTelephone(phoneTextBox->GetValue().ToStdString());

        string street = streetTextBox->GetValue().ToStdString();
        string houseNumber = houseNumberTextBox->GetValue().ToStdString();
        string zip = zipTextBox->GetValue().ToStdString();
        string city = cityTextBox->GetValue().ToStdString();

        if (
            creating() ||
            street != address->getStreet() || houseNumber != address->getHouseNumber()
            || zip != address->getZip() || city != address->getCity()
        ) {
            // Replace address, do not change address - otherwise this would cause undesirable side effects
            /* Try to find matching address first */
            SqlitePreparedStatement query(
                    database->getConnection(),
                    "SELECT * FROM postal_addresses WHERE street = ? AND house_number = ? AND zip = ? AND city = ?"
            );
            query.bind(1, street);
            query.bind(2, houseNumber);
            query.bind(3, zip);
            query.bind(4, city);
            if (query.step())
                address = addressFactory.load(query);
            else // Otherwise construct new address
                address = addressFactory.construct(-1);

            /* Fill in data */
            address->setStreet(street);
            address->setHouseNumber(houseNumber);
            address->setZip(zip);
            address->setCity(city);
            user->setPostalAddress(address);
        }

        /* Try to persist all to database */
        address->persist();
        user->persist();

        user->getDatabase()->getConnection()->commit();
        return true;

    }
    catch (pb2::Exception & e) {
        database->getConnection()->rollback();

        wxMessageBox(_("Ein Fehler ist aufgetreten:\n\n") + e.what(), _("Fehler"), wxICON_ERROR);
        return false;
    }
}
