#include "gui/App.h"

using namespace pb2;
using namespace std;

App::~App() = default;

bool App::OnInit() {
    mainFrame = new MainFrame("prog2-beleg GUI", wxDefaultPosition, wxSize(1024, 768));
    mainFrame->Show(true);
    return true;
}
