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
        wxMessageBox(_("Das Medium ist nicht verlängerbar. Liegt vielleicht das neue Rückgabedatum vor dem alten?"), dialogCaption, wxICON_ERROR);
        return false;
    }
    catch (pb2::Exception & e) {
        wxMessageBox(_("Es ist ein Fehler aufgetreten. Versuchen Sie es erneut."), dialogCaption, wxICON_ERROR);
        return false;
    }

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
