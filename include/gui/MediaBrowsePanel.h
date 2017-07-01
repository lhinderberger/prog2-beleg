#ifndef PROG2_BELEG_MEDIA_BROWSE_PANEL_H
#define PROG2_BELEG_MEDIA_BROWSE_PANEL_H

#include <wx/wx.h>

#include "core/Database.h"
#include "Basket.h"
#include "BrowsePanel.h"
#include "MediaSearchTable.h"

namespace pb2 {
    class MediaBrowsePanel : public BrowsePanel {
    private:
        MediaSearchTable * searchTable = nullptr;
        std::shared_ptr<Basket> basket;

        void evAddToBasket(wxCommandEvent & ev);
        void evDeleteMedium(wxCommandEvent & ev);
        void evEditMedium(wxCommandEvent & ev);
        void evNewCopy(wxCommandEvent & ev);

    public:
        enum class ID {
            BTN_ADD_TO_BASKET = 1, BTN_DELETE_MEDIUM, BTN_NEW_COPY, BTN_EDIT_MEDIUM
        };

        MediaBrowsePanel(
                wxWindow * parent, std::shared_ptr<Database> database,
                std::shared_ptr<Basket> basket
        );

        virtual void refresh() override;

        DECLARE_EVENT_TABLE();
    };
}

#endif
