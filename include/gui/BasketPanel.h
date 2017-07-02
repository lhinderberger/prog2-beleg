#ifndef PROG2_BELEG_GUI_BASKET_PANEL_H
#define PROG2_BELEG_GUI_BASKET_PANEL_H

#include <map>
#include <wx/wx.h>
#include "Basket.h"

namespace pb2 {
    /**
     * The BasketPanel is usually located in the sidebar of the application
     * and provides a view of the Basket contents as well as a way to remove individual
     * items from the Basket.
     */
    class BasketPanel : public wxPanel {
    private:
        wxListBox * listBox = nullptr;
        std::shared_ptr<Basket> basket;
        std::map<int, std::shared_ptr<MediumCopy>> mcIndexes;

        void evRemove(wxCommandEvent & ev);

    public:
        enum class ID { REMOVE_BTN = 1 };

        BasketPanel(wxWindow * parent, std::shared_ptr<Basket> basket);
        void rebuildListBox();

        DECLARE_EVENT_TABLE();
    };
}

#endif
