#include "gui/MainFrame.h"

using namespace pb2;
using namespace std;

MainFrame::MainFrame(const wxString & title, const wxPoint & pos, const wxSize & size)
    : wxFrame(NULL, wxID_ANY, title, pos, size) {
    CreateStatusBar();
}

MainFrame::~MainFrame() = default;
