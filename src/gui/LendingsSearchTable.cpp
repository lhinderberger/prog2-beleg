#include "gui/LendingsSearchTable.h"
#include "gui/translations.h"

#define LIKEQUERY "'%%' || :query || '%%'"

using namespace pb2;
using namespace std;

LendingsSearchTable::LendingsSearchTable(wxWindow * parent, wxWindowID id, shared_ptr<Database> database)
    : TwoQuerySearchTable(
        parent, id, _("Neue Ausleihe..."),
        {
            _("EAN"), _("Serien-Nr."), _("Autor"), _("Name"), _("Ausgeliehen am"), _("Fälligkeit")
        },
        database
    ) {
    /* Configure TwoQuerySearchTable */
    setNoBind(true); // There is more than one search/list parameter, so TwoQuerySearchTable shall not auto-bind
}


string LendingsSearchTable::getSearchSQL() {
    return getListSQL() + " WHERE media.ean LIKE " LIKEQUERY " OR media.title LIKE " LIKEQUERY " "
           "OR authors.last_name LIKE " LIKEQUERY " OR authors.first_name LIKE " LIKEQUERY " "
           "OR (media.ean || media_copies.serial_number) LIKE " LIKEQUERY;
}

string LendingsSearchTable::getListSQL() {
    return  "SELECT * FROM lendings "
            "JOIN media_copies ON lendings.medium_ean = media_copies.medium_ean, lendings.medium_copy_serial_number = media_copies.serial_number "
            "JOIN media ON media.ean = lendings.medium_ean "
            "JOIN authors ON author.id = media.author_id "
            "WHERE lendings.library_user_id = :user_id ";
}

wxString LendingsSearchTable::getColumnContent(int column) {
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
        /* Build label from timestamp_lent */
        wxDateTime dateTime((time_t)query->columnInt("lendings.timestamp_lent"));
        if (!dateTime.IsValid())
            return _("Unbekannt (siehe Details)");
        return dateTime.Format(_("%d.%m.%C"));
    }
    else if (column == 5) {
        /* Build label from due_date */
        wxDateTime dateTime;
        if (!dateTime.ParseISODate(query->columnString("lendings.due_date")))
            return _("Unbekannt (siehe Details)");
        return dateTime.Format(_("%e. %B"));
    }
    else throw logic_error("Invalid column index!");

    /* Direct query */
    return query->columnString(directQueryColumn);
}

void LendingsSearchTable::setUserId(int id) {
    /* Set user id */
    userId = id;

    /* Refresh view */
    if (getCurrentQueryType() == QueryType::SEARCH)
        searchCtrl->Clear();
    else
        list();
}

void LendingsSearchTable::list() {
    /* Show nothing when no user is selected */
    if (!userId)
        dataView->DeleteAllItems();
    else
        TwoQuerySearchTable::list();
}

void LendingsSearchTable::search(const wxString & query) {
    /* Show nothing when no user is selected */
    if (!userId)
        dataView->DeleteAllItems();
    else
        TwoQuerySearchTable::search(query);
}

void LendingsSearchTable::prepareList() {
    /* Prepare */
    TwoQuerySearchTable::prepareList();

    /* Bind parameter */
    currentQuery()->bind(1, userId);
}

void LendingsSearchTable::prepareSearch(const wxString & query) {
    /* Prepare */
    TwoQuerySearchTable::prepareList();

    /* Bind parameters */
    auto statement = currentQuery();
    statement->bind(1, userId);
    statement->bind(2, query.ToStdString());
}
