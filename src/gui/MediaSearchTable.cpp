#include "gui/MediaSearchTable.h"
#include "gui/translations.h"

#define LIKEQUERY "'%%' || :query || '%%'"

using namespace pb2;
using namespace std;

MediaSearchTable::MediaSearchTable(wxWindow * parent, wxWindowID id, shared_ptr<Database> database)
    : TwoQuerySearchTable(
        parent, id, _("Neues Medium..."),
        {
            _("EAN"), _("Serien-Nr."), _("Autor"), _("Titel"), _("Format"),
            _("Standort"), _("Verfügbarkeit")
        },
        database
    ) {

}


string MediaSearchTable::getSearchSQL() {
    return getListSQL() + " WHERE media.title LIKE " LIKEQUERY " OR media.subtitle LIKE " LIKEQUERY " "
           "OR authors.first_name LIKE " LIKEQUERY " OR authors.last_name LIKE " LIKEQUERY " "
           "OR media_copies.location LIKE " LIKEQUERY " OR media.ean LIKE " LIKEQUERY
           "OR (media.ean || media_copies.serial_number) LIKE " LIKEQUERY;
}

string MediaSearchTable::getListSQL() {
    return  "SELECT * FROM media_copies "
            "JOIN media ON media_copies.medium_ean = media.ean "
            "JOIN authors ON authors.id = media.author_id ";
}

wxString MediaSearchTable::getColumnContent(int column) {
    /* Get current query */
    shared_ptr<SqlitePreparedStatement> query = currentQuery();

    /* Retrieve media object */
    string directQueryColumn;
    if (column == 0)
        directQueryColumn = "media.ean";
    else if (column == 1)
        directQueryColumn = "media_copies.serial_number";
    else if (column == 2)
        return query->columnString("authors.last_name") + ", " + query->columnString("authors.first_name");
    else if (column == 3)
        directQueryColumn = "media.title";
    else if (column == 4) {
        /* Build format label, translate values if possible */
        wxString type = mediaTypeTranslation(query->columnString("media.type"));
        wxString format = mediaFormatTranslation(query->columnString("media.format"));
        return type + " (" + format + ")";
    }
    else if (column == 5)
        directQueryColumn = "media_copies.location";
    else if (column == 6) {
        /* Deaccessioned media are no longer available */
        bool deaccessioned = query->columnInt("media_copies.deaccessioned") != 0;
        if (deaccessioned)
            return _("Nicht mehr im Bestand");

        /* Get and interpret availability hint */
        string availHint = query->columnString("media_copies.availability_hint");
        if (availHint == "now")
            return _("Verfügbar");
        else {
            /* Build date object from availability string */
            wxDateTime dateTime;
            if (!dateTime.ParseISODate(availHint))
                return _("Unbekannt)");

            // Medium might be not available until short before the library closes on
            // return date, so better safe than sorry - add a day
            dateTime.Add(wxDateSpan(0, 0, 0, 1));

            /* Build availability string */
            return dateTime.Format(_("Verfügbar ab %e. %B"));
        }
    }
    else throw logic_error("Invalid column index!");

    /* Direct query */
    return query->columnString(directQueryColumn);
}

pair<string,int> MediaSearchTable::getSelectedEanSerial() const {
    static pair<string,int> invalid = {"", -1};
    /* Is there any row selected? */
    int selectedRow = dataView->GetSelectedRow();
    if (selectedRow == wxNOT_FOUND || selectedRow < 0)
        return invalid;

    /* Retrieve medium EAN */
    wxVariant eanVal;
    wxString ean;
    dataView->GetValue(eanVal, (unsigned int)selectedRow, 0);
    if (!eanVal.Convert(&ean))
        return invalid;

    /* Retrieve medium Serial Number */
    wxVariant serialVal;
    long serial;
    dataView->GetValue(serialVal, (unsigned int)selectedRow, 1);
    if (!serialVal.Convert(&serial) || serial > INT_MAX)
        return invalid;

    return pair<string,int>{ean.ToStdString(), serial};
}

shared_ptr<MediumCopy> MediaSearchTable::getSelectedMediumCopy() const {
    /* Get primary key */
    auto eanSerial = getSelectedEanSerial();
    if (eanSerial.second < 0)
        return nullptr;

    /* Query and load MediumCopy */
    SqlitePreparedStatement query(getDatabase()->getConnection(), "SELECT * FROM media_copies WHERE medium_ean = ? AND serial_number = ?");
    query.bind(1, eanSerial.first);
    query.bind(2, eanSerial.second);
    if (!query.step())
        return nullptr;

    return DatabaseObjectFactory<MediumCopy>(getDatabase()).load(query);
}
