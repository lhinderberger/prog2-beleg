#ifndef PROG2_BELEG_GUI_BASKET_WINDOW_H
#define PROG2_BELEG_GUI_BASKET_WINDOW_H

#include <wx/wx.h>

namespace pb2 {
    class BasketWindow : public wxWindow {
    private:
        wxListBox * listBox = nullptr;

    public:
        BasketWindow(wxWindow * parent);
    };
}

#endif
