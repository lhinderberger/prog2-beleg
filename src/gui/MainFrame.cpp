#include "gui/MainFrame.h"
#include "gui/StatusbarDbConnection.h"

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

    /* Maximize */
    Maximize(true);
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
    static wxString dialogHeading = _("Datenbank erstellen");

    /* Close currently open database, if any */
    closeDatabase();

    /* Open file chooser dialog to determine file name */
    wxFileDialog fileDialog(
            this, dialogHeading, wxEmptyString, wxEmptyString,
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

    /* Ask to overwrite file, if it already exists */
    if (wxFileExists(filename)) {
        int confirmOverwrite = wxMessageBox(
                _("Soll die bestehende Datei " + filename + " wirklich überschrieben werden?"),
                dialogHeading, wxICON_QUESTION |wxYES_NO
        );
        if (confirmOverwrite != wxYES) {
            newDatabase(); // Start over
            return;
        } else
            wxRemoveFile(filename);
    }

    /* Initialize database */
    auto connection = StatusbarDbConnection::construct(GetStatusBar(), filename, true);
    auto database = Database::initialize(connection);

    /* Ask to generate example data */
    int generateExampleData = wxMessageBox(
            _("Soll die neue Datenbank mit Beispieldaten gefüllt werden?"),
            dialogHeading, wxICON_QUESTION |wxYES_NO
    );
    if (generateExampleData == wxYES) {
        database->generateExampleData();
        connection->commit();
    }

    /* Open DatabasePanel */
    //Note: This has to be behind generating example data, as otherwise example data
    //wouldn't be visible right away in the MediaBrowsePanel opened by default
    //by DatabasePanel
    setDatabaseAndOpenPanel(database);

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

    /* Open database + open DatabasePanel */
    try {
        auto connection = StatusbarDbConnection::construct(GetStatusBar(), fileDialog.GetPath().ToStdString(), false);
        setDatabaseAndOpenPanel(Database::open(connection));
    }
    catch (UnsupportedDatabaseVersionException & e) {
        wxMessageBox(
                _("Die Datenbank verwendet eine neuere Version. Bitte installieren Sie die neueste Version der Software!"),
                _("Fehler"), wxICON_ERROR
        );
        return;
    }
    catch (SqliteException & e) {
        wxMessageBox(
                _("Die Datenbankdatei konnte nicht geöffnet werden!\n") + e.what(),
                _("Fehler"), wxICON_ERROR
        );
        return;
    }

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
