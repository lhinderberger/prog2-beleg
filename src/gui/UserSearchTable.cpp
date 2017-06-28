#include "gui/UserSearchTable.h"
#include "gui/translations.h"

#define LIKEQUERY "'%%' || :query || '%%'"

using namespace pb2;
using namespace std;

UserSearchTable::UserSearchTable(wxWindow * parent, shared_ptr<Database> database)
    : TwoQuerySearchTable(
        parent, _("Neuer Benutzer..."),
        {
            _("Benutzer-Nr."), _("Nachname"), _("Vorname"), _("PLZ"), _("Ort"), _("Anschrift")
        },
        database
    ) {

}


string UserSearchTable::getSearchSQL() {
    return getListSQL() + " WHERE library_users.id LIKE " LIKEQUERY " OR last_name LIKE " LIKEQUERY " "
           "OR first_name LIKE " LIKEQUERY " OR zip LIKE " LIKEQUERY " "
           "OR city LIKE " LIKEQUERY " OR street LIKE " LIKEQUERY " "
           "OR (street || ' ' || house_number) LIKE " LIKEQUERY;
}

string UserSearchTable::getListSQL() {
    return  "SELECT * FROM library_users "
            "JOIN postal_addresses ON postal_addresses.id = library_users.postal_address_id ";
}

wxString UserSearchTable::getColumnContent(int column) {
    /* Get current query */
    shared_ptr<SqlitePreparedStatement> query = currentQuery();

    /* Retrieve media object */
    string directQueryColumn;
    if (column == 0)
        directQueryColumn = "library_users.id";
    else if (column == 1)
        directQueryColumn = "library_users.last_name";
    else if (column == 2)
        directQueryColumn = "library_users.first_name";
    else if (column == 3)
        directQueryColumn = "postal_addresses.zip";
    else if (column == 4)
        directQueryColumn = "postal_addresses.city";
    else if (column == 5) {
        /* Build street + houseNumber label */
        wxString street = query->columnString("postal_addresses.street");
        wxString houseNumber = query->columnString("postal_addresses.house_number");
        return street + ' ' + houseNumber;
    }
    else throw logic_error("Invalid column index!");

    /* Direct query */
    return query->columnString(directQueryColumn);
}
