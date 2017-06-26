#include "gui/MainFrame.h"

#include <wx/artprov.h>

using namespace pb2;
using namespace std;

wxBEGIN_EVENT_TABLE(pb2::MainFrame, wxFrame)
    EVT_CLOSE(MainFrame::onClose)
    EVT_TOOL((int)MainFrame::ID::CLOSE_DATABASE, MainFrame::evCloseDatabase)
    EVT_TOOL((int)MainFrame::ID::NEW_DATABASE, MainFrame::evNewDatabase)
    EVT_TOOL((int)MainFrame::ID::OPEN_DATABASE, MainFrame::evOpenDatabase)
wxEND_EVENT_TABLE()


MainFrame::MainFrame(const wxString & title, const wxPoint & pos, const wxSize & size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size) {
    /* Create and initialize status bar */
    CreateStatusBar();
    SetStatusText(_("Willkommen in der Bibliotheksverwaltung!"));

    /* Create toolbar */
    wxToolBar * toolbar = CreateToolBar(wxTB_HORIZONTAL | wxTB_TEXT);
    toolbar->AddTool((int)ID::NEW_DATABASE, _("Neu"), wxArtProvider::GetBitmap(wxART_NEW));
    toolbar->AddTool((int)ID::OPEN_DATABASE, _("Öffnen"), wxArtProvider::GetBitmap(wxART_FILE_OPEN));
    toolbar->AddTool((int)ID::CLOSE_DATABASE, _("Schließen"), wxArtProvider::GetBitmap(wxART_CLOSE));

    /* Set minimum size */
    SetMinSize(wxSize(1024, 768));
}

bool MainFrame::closeDatabase() {
    if (!database)
        return true;

    /* Request user confirmation first */
    int userConfirm = wxMessageBox(
            _("Wollen Sie die Datenbank wirklich schließen?"),
            _("Datenbank schließen"), wxICON_QUESTION |wxYES_NO
    );
    if (userConfirm != wxYES)
        return false;

    /* Close database in every case, but destroy DatabaseWindow first */
    if (databasePanel) {
        databasePanel->Destroy();
        databasePanel = nullptr;
    }
    database = nullptr;

    /* Set status bar text */
    SetStatusText(_("Datenbank wurde geschlossen."));
    return true;
}

void MainFrame::newDatabase() {
    /* Close currently open database, if any */
    closeDatabase();

    /* Open file chooser dialog to determine file name */
    wxFileDialog fileDialog(
            this, _("Neue Datenbankdatei erstellen..."), wxEmptyString, wxEmptyString,
            _("Datenbankdateien (*.db)|*.db|Alle Dateien|*"), wxFD_SAVE
    );
    if (fileDialog.ShowModal() == wxID_CANCEL)
        return;

    /* Build filename */
    string filename = fileDialog.GetPath().ToStdString();
    auto filenameLen = filename.length();
    // Extend ".db" extension if that wildcard was chosen by the user and no extension was provided
    if (fileDialog.GetFilterIndex() == 0 && (filenameLen < 3 || filename.substr(filenameLen - 3) != ".db"))
        filename += ".db";

    /* Initialize database + open DatabaseWindow */
    auto connection = SqliteConnection::construct(filename, true);
    setDatabaseAndOpenPanel(Database::initialize(connection));

    /* Ask to generate example data */
    int generateExampleData = wxMessageBox(
            _("Soll die neue Datenbank mit Beispieldaten gefüllt werden?"),
            _("Datenbank erstellen"), wxICON_QUESTION |wxYES_NO
    );
    if (generateExampleData == wxYES) {
        database->generateExampleData();
        connection->commit();
    }

    /* Set status bar text */
    SetStatusText(_("Datenbank wurde angelegt!"));
}

void MainFrame::openDatabase() {
    /* Close currently open database, if any */
    closeDatabase();

    /* Open file chooser dialog to determine file name */
    wxFileDialog fileDialog(
            this, _("Datenbankdatei öffnen..."), wxEmptyString, wxEmptyString,
            _("Datenbankdateien (*.db)|*.db|Alle Dateien|*")
    );
    if (fileDialog.ShowModal() == wxID_CANCEL)
        return;

    /* Open database + open DatabaseWindow */
    auto connection = SqliteConnection::construct(fileDialog.GetPath().ToStdString(), false);
    setDatabaseAndOpenPanel(Database::open(connection));

    /* Set status bar text */
    SetStatusText(_("Datenbank wurde geöffnet."));
}

void MainFrame::onClose(wxCloseEvent & event) {
    if (event.CanVeto() && !closeDatabase())
        event.Veto();
    else
        event.Skip();
}

void MainFrame::setDatabaseAndOpenPanel(shared_ptr<Database> database) {
    /* Destroy current panel, if any */
    if (databasePanel)
        databasePanel->Destroy();

    /* Set database pointer */
    this->database = database;

    /* Initialize new panel */
    databasePanel = new DatabasePanel(this, database);
    databasePanel->SetSize(GetClientSize());
}
