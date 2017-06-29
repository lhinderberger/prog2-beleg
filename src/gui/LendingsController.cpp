#include "gui/LendingsController.h"

#include <wx/numdlg.h>

using namespace pb2;
using namespace std;

bool LendingsController::extend(shared_ptr<Lending> lending) {
    static wxString dialogCaption = _("Verlängerung");

    /* Make sure the lending is not already returned */
    if (lending->isReturned()) {
        wxMessageBox(
                _("Das Medium wurde bereits zurückgegeben und kann daher nicht verlängert werden!"),
                dialogCaption, wxICON_ERROR | wxOK
        );
        return false;
    }

    /* Warn user if lending is overdue */
    warnOverdue(lending, dialogCaption);

    /* Present summary and ask how long to extend */
    wxString message;
    message.Printf(_("Zu verlängern ist folgendes Medium:\n%s"), getMediumCopyLabel(lending->getMediumCopy()));
    int daysExtend = (int)wxGetNumberFromUser(
            message,
            _("Um wie viele Tage soll ab heute verlängert werden?"),
            dialogCaption,
            Lending::getDefaultExtensionDays(lending->getDatabase())
    );
    if (daysExtend < 0)
        return false;

    /* Call extension method and persist */
    try {
        lending->extend(daysExtend);
        lending->persist();
        lending->getDatabase()->getConnection()->commit();
        wxMessageBox(_("Das Medium wurde erfolgreich verlängert!"), dialogCaption, wxICON_INFORMATION);
    }
    catch (pb2::NotExtensibleException & e) {
        lending->getDatabase()->getConnection()->rollback();
        wxMessageBox(_("Das Medium ist nicht verlängerbar. Liegt vielleicht das neue Rückgabedatum vor dem alten?"), dialogCaption, wxICON_ERROR);
        return false;
    }
    catch (pb2::Exception & e) {
        lending->getDatabase()->getConnection()->rollback();
        wxMessageBox(_("Es ist ein Fehler aufgetreten. Versuchen Sie es erneut."), dialogCaption, wxICON_ERROR);
        return false;
    }

    return true;
}

bool LendingsController::newLending(shared_ptr<LibraryUser> libraryUser, shared_ptr<Basket> basket) {
    static wxString dialogCaption = _("Neue Ausleihe");
    auto database = libraryUser->getDatabase();

    /* Abort and inform user if there are no Media in the Basket */
    if (basket->getList().empty()) {
        wxMessageBox(
                _("Um Medien zu verleihen, fügen Sie diese zuerst dem Warenkorb hinzu!"),
                dialogCaption,
                wxICON_INFORMATION
        );
        return false;
    }

    /* Warn if user has any overdue Lendings */
    auto lendings = libraryUser->queryLendings();
    for (auto & lending : lendings) {
        if (lending->isOverdue()) {
            /* Warn user and abort, if requested */
            if (wxMessageBox(
                    _("Warnung: Der Nutzer hat noch überfällige Ausleihen! Wollen Sie trotzdem fortfahren?"),
                    dialogCaption,
                    wxICON_WARNING
            ) == wxNO)
                return false;
            break; // No need to search on from this point
        }
    }

    /* Reload all medium copies in the basket */
    DatabaseObjectFactory<MediumCopy> mediumCopyFactory(database);
    BasketList reloadedCopies;

    SqlitePreparedStatement query(database->getConnection(), "SELECT * FROM media_copies WHERE medium_ean = ? AND serial_number = ?");
    for (auto & source : basket->getList()) {
        query.bind(1, source->getMediumEAN());
        query.bind(2, source->getSerialNumber());
        query.step();
        reloadedCopies.push_back(mediumCopyFactory.load(query));
        query.reset();
    }

    /* Abort if any copy is unavailable (i.e. deaccessioned or already lent) */
    BasketList unavailableCopies;
    for (auto & mediumCopy : reloadedCopies) {
        if (mediumCopy->getDeaccessioned() || mediumCopy->getActiveLending())
            unavailableCopies.push_back(mediumCopy);
    }
    if (unavailableCopies.size() > 0) {
        /* Inform user about the error */
        wxString message = _("Folgende Medien sind nicht verfügbar (z.B. anderweitig verliehen / aus dem Bestand gegangen):\n\n");
        for (auto & mediumCopy : unavailableCopies)
            message += getMediumCopyLabel(mediumCopy) + "\n";
        message += _("\n\nBitte nehmen Sie die Medien aus dem Warenkorb und versuchen es erneut!");

        wxMessageBox(message, dialogCaption, wxICON_ERROR | wxOK);
        return false;
    }

    /* Present summary */
    wxString message = _("Folgende Medien werden ausgeliehen:\n\n");
    for (auto & mediumCopy : reloadedCopies)
        message += getMediumCopyLabel(mediumCopy) + "\n";
    message += _("\n\nWollen Sie fortfahren?");
    if (wxMessageBox(message, dialogCaption, wxICON_QUESTION | wxYES_NO) != wxYES)
        return false;

    /* Create and persist lendings for each medium */
    DatabaseObjectFactory<Lending> lendingFactory(database);
    try {
        for (auto & mediumCopy : reloadedCopies) {
            auto lending = lendingFactory.construct(mediumCopy, libraryUser, -1);
            lending->persist();
        }

        database->getConnection()->commit();
        wxMessageBox(_("Die Medien wurden erfolgreich verliehen!"), dialogCaption, wxICON_INFORMATION);
    }
    catch (pb2::Exception) {
        database->getConnection()->rollback();
        wxMessageBox(_("Es ist ein Fehler aufgetreten. Versuchen Sie es erneut."), dialogCaption, wxICON_ERROR);
        return false;
    }

    /* Clear basket */
    basket->clear();
    return true;
}

bool LendingsController::returnL(shared_ptr<Lending> lending) {
    static wxString dialogCaption = _("Rückgabe");

    /* Make sure the lending is not already returned */
    if (lending->isReturned()) {
        wxMessageBox(
                _("Das Medium wurde bereits zurückgegeben und kann daher nicht zurückgegeben werden!"),
                dialogCaption, wxICON_ERROR | wxOK
        );
        return false;
    }

    /* Warn user if lending is overdue */
    warnOverdue(lending, dialogCaption);

    /* Present summary and ask for confirmation */
    wxString message;
    message.Printf(_("Zurückzunehmen ist folgendes Medium:\n\n%s\n\nWollen Sie fortfahren?"), getMediumCopyLabel(lending->getMediumCopy()));
    if (wxMessageBox(message, dialogCaption, wxICON_QUESTION | wxYES_NO) != wxYES)
        return false;

    /* Call extension method and persist */
    try {
        lending->returnL();
        lending->persist();
        lending->getDatabase()->getConnection()->commit();
        wxMessageBox(_("Das Medium wurde erfolgreich zurückgenommen!"), dialogCaption, wxICON_INFORMATION);
    }
    catch (pb2::Exception & e) {
        wxMessageBox(_("Es ist ein Fehler aufgetreten. Versuchen Sie es erneut."), dialogCaption, wxICON_ERROR);
        return false;
    }

    return true;
}

wxString LendingsController::getMediumCopyLabel(shared_ptr<MediumCopy> mediumCopy) const {
    auto medium = mediumCopy->getMedium();
    auto author = medium->getAuthor();

    wxString result;
    result.Printf(_("%s: %s (S/N %d)"), author->getLastName(), medium->getTitle(), mediumCopy->getSerialNumber());
    return result;
}

bool LendingsController::warnOverdue(shared_ptr<Lending> lending, const wxString & dialogCaption) const {
    if (lending->isOverdue()) {
        wxString message;
        message.Printf(
                _("Hinweis: Die Ausleihe ist bereits %d Tage überzogen! Wollen Sie trotzdem fortsetzen?"),
                (-1 * lending->getDaysLeft())
        );
        if (wxMessageBox(message, dialogCaption, wxICON_INFORMATION | wxYES_NO) == wxNO)
            return false;
    }
    return true;
}
