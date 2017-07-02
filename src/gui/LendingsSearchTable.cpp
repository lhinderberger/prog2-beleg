#include "gui/LendingsSearchTable.h"
#include "gui/translations.h"

#define LIKEQUERY "'%%' || :query || '%%'"

using namespace pb2;
using namespace std;

LendingsSearchTable::LendingsSearchTable(wxWindow * parent, wxWindowID id, shared_ptr<Database> database, bool activeOnly)
    : TwoQuerySearchTable(
        parent, id, _("Neue Ausleihe..."),
        {
            _("Verleih-Nr."), _("EAN"), _("Serien-Nr."), _("Autor"), _("Name"), _("Ausgeliehen am"), _("Verlängert"), _("Fälligkeit")
        },
        database
    ) {
    /* Configure TwoQuerySearchTable */
    setNoBind(true); // There is more than one search/list parameter, so TwoQuerySearchTable shall not auto-bind

    /* Copy configuration values */
    this->activeOnly = activeOnly;
}


string LendingsSearchTable::getSearchSQL() {
    return getListSQL() + " AND (media.ean LIKE " LIKEQUERY " OR media.title LIKE " LIKEQUERY " "
           "OR authors.last_name LIKE " LIKEQUERY " OR authors.first_name LIKE " LIKEQUERY " "
           "OR (media.ean || media_copies.serial_number) LIKE " LIKEQUERY ")";
}

string LendingsSearchTable::getListSQL() {
    string activeFilter = activeOnly ? "(lendings.timestamp_returned = 0 OR lendings.timestamp_returned IS NULL) AND" : "";
    return  "SELECT * FROM lendings "
            "JOIN media_copies ON lendings.medium_ean = media_copies.medium_ean AND lendings.medium_copy_serial_number = media_copies.serial_number "
            "JOIN media ON media.ean = lendings.medium_ean "
            "JOIN authors ON authors.id = media.author_id "
            "WHERE " + activeFilter + " lendings.library_user_id = :user_id ";
}

wxString LendingsSearchTable::getColumnContent(int column) {
    static wxString unknownString = _("Unbekannt");

    /* Get current query */
    shared_ptr<SqlitePreparedStatement> query = currentQuery();

    /* Retrieve media object */
    string directQueryColumn;
    if (column == 0)
        directQueryColumn = "lendings.id";
    else if (column == 1)
        directQueryColumn = "media.ean";
    else if (column == 2)
        directQueryColumn = "media_copies.serial_number";
    else if (column == 3)
        return query->columnString("authors.last_name") + ", " + query->columnString("authors.first_name");
    else if (column == 4)
        directQueryColumn = "media.title";
    else if (column == 5) {
        /* Build label from timestamp_lent */
        wxDateTime dateTime((time_t)query->columnInt("lendings.timestamp_lent"));
        if (!dateTime.IsValid())
            return unknownString;
        return dateTime.Format(_("%d.%m.%y"));
    }
    else if (column == 6) {
        /* Build label for times_extended */
        wxString out;
        if (out.Printf(_("%dx"), query->columnInt("lendings.times_extended")) < 0)
            return unknownString;
        return out;
    }
    else if (column == 7) {
        /* Build label from due_date */
        wxDateTime dateTime;
        if (!dateTime.ParseISODate(query->columnString("lendings.due_date")))
            return unknownString;
        return dateTime.Format(_("%e. %B"));
    }
    else throw LogicError("Invalid column index!");

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
    TwoQuerySearchTable::prepareSearch(query);

    /* Bind parameters */
    auto statement = currentQuery();
    statement->bind(1, userId);
    statement->bind(2, query.ToStdString());
}

shared_ptr<Lending> LendingsSearchTable::getSelectedLending() const {
    /* Is there a Lending selected? */
    int lendingId = getSelectedId();
    if (!lendingId)
        return nullptr;

    /* Load the user with the given ID */
    SqlitePreparedStatement query(getDatabase()->getConnection(), "SELECT * FROM lendings WHERE id = :id");
    query.bind(1, lendingId);
    query.step();
    return DatabaseObjectFactory<Lending>(getDatabase()).load(query);
}

int LendingsSearchTable::getSelectedId() const {
    /* Is there any row selected? */
    int selectedRow = dataView->GetSelectedRow();
    if (selectedRow == wxNOT_FOUND || selectedRow < 0)
        return 0;

    /* Retrieve ID */
    wxVariant val;
    long userId;
    dataView->GetValue(val, (unsigned int)selectedRow, 0);
    if (!val.Convert(&userId) || userId > INT_MAX)
        return 0;
    else
        return (int)userId;
}
