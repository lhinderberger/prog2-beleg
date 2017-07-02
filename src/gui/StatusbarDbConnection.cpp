#include "gui/StatusbarDbConnection.h"

using namespace pb2;
using namespace std;

StatusbarDbConnection::StatusbarDbConnection(
        wxStatusBar * statusBar, const std::string & filename, bool create
) : SqliteConnection(filename, create), statusBar(statusBar) {

}

std::shared_ptr<SqliteConnection> StatusbarDbConnection::construct(
        wxStatusBar * statusBar, const std::string & filename, bool create
) {
    return shared_ptr<SqliteConnection>(new StatusbarDbConnection(statusBar, filename, create));
}


void StatusbarDbConnection::commit() {
    if (statusBar)
        statusBar->SetStatusText(_("Die Änderungen wurden gespeichert."));
}

void StatusbarDbConnection::rollback() {
    if (statusBar)
        statusBar->SetStatusText(_("Die Änderungen wurden verworfen."));
}
