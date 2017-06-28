#ifndef PROG2_BELEG_MEDIA_BROWSE_PANEL_H
#define PROG2_BELEG_MEDIA_BROWSE_PANEL_H

#include <wx/wx.h>

#include "core/Database.h"
#include "Basket.h"
#include "MediaSearchTable.h"

namespace pb2 {
    class MediaBrowsePanel : public wxPanel {
    private:
        MediaSearchTable * searchTable = nullptr;
        std::shared_ptr<Basket> basket;

        void evAddToBasket(wxCommandEvent & ev);

    public:
        enum class ID { BTN_ADD_TO_BASKET = 1 };

        MediaBrowsePanel(
                wxWindow * parent, std::shared_ptr<Database> database,
                std::shared_ptr<Basket> basket
        );

        DECLARE_EVENT_TABLE();
    };
}

#endif
