#include "gui/App.h"

using namespace pb2;
using namespace std;

App::~App() = default;

bool App::OnInit() {
    /* Handle fatal exceptions */
    wxHandleFatalExceptions(true);

    /* Initialize locale */
    locale = make_unique<wxLocale>(wxLocale::GetSystemLanguage());

    /* Initialize main frame */
    mainFrame = new MainFrame(_("Bibliotheksverwaltung"), wxDefaultPosition, wxSize(1024, 768));
    mainFrame->Show(true);
    return true;
}

void App::OnFatalException() {
    try {
        throw;
    }
    catch (exception & e) {
        cerr << e.what() << endl;
        throw;
    }
}

void App::OnUnhandledException() {
    try {
        throw;
    }
    catch (exception & e) {
        cerr << e.what() << endl;
        throw;
    }
}
