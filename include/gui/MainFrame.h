#ifndef PROG2_BELEG_GUI_MAIN_FRAME_H
#define PROG2_BELEG_GUI_MAIN_FRAME_H

#include <wx/wx.h>

namespace pb2 {
    class MainFrame : public wxFrame {
    public:
        MainFrame(const wxString & title, const wxPoint & pos, const wxSize & size);
        virtual ~MainFrame();
    };
}

#endif
