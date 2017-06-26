#include "gui/MediaSearchTable.h"
#include "gui/translations.h"

#define LIKEQUERY "'%%' || :query || '%%'"

using namespace pb2;
using namespace std;

const char * basicSQL =
        "SELECT * FROM media_copies "
        "JOIN media ON media_copies.medium_ean = media.ean "
        "JOIN authors ON authors.id = media.author_id ";

MediaSearchTable::MediaSearchTable(wxWindow * parent, shared_ptr<Database> database)
    : DatabaseSearchTable(
        parent, _("Neues Medium..."),
        {
            _("EAN"), _("Serien-Nr."), _("Autor"), _("Titel"), _("Format"),
            _("Standort"), _("Verfügbarkeit")
        }
    ), database(database) {

}


void MediaSearchTable::prepareSearch(const wxString & query) {
    /* Prepare query, if necessary */
    if (searchQuery == nullptr) {
        searchQuery = make_shared<SqlitePreparedStatement>(
                database->getConnection(),
                string(basicSQL) +
                " WHERE media.title LIKE " LIKEQUERY " OR media.subtitle LIKE " LIKEQUERY " "
                "OR authors.first_name LIKE " LIKEQUERY " OR authors.last_name LIKE " LIKEQUERY " "
                "OR media_copies.location LIKE " LIKEQUERY " OR media.ean LIKE " LIKEQUERY ""
        );
    }
    currentQuery = QueryType::SEARCH;

    /* Reset query */
    searchQuery->reset();

    /* Bind parameter */
    searchQuery->bind(1, query.ToStdString());
}

void MediaSearchTable::prepareList() {
    /* Prepare query, if necessary */
    if (listQuery == nullptr)
        listQuery = make_shared<SqlitePreparedStatement>(database->getConnection(), string(basicSQL));
    currentQuery = QueryType::LIST;

    /* Reset query */
    listQuery->reset();
}

bool MediaSearchTable::step() {
    if (currentQuery == QueryType::LIST)
        return listQuery->step();
    else if (currentQuery == QueryType::SEARCH)
        return searchQuery->step();
    else
        return false;
}

wxString MediaSearchTable::getColumnContent(int column) {
    //TODO: Move all list/search query wrangling into own base class
    /* Get current query */
    shared_ptr<SqlitePreparedStatement> query = nullptr;
    if (currentQuery == QueryType::LIST)
        query = listQuery;
    else if (currentQuery == QueryType::SEARCH)
        query = searchQuery;
    else
        throw logic_error("Trying to retrieve column content without active query!");

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
                return _("Unbekannt (siehe Details)");

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
