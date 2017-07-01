#ifndef PROG2_BELEG_GUI_BROWSE_PANEL_H
#define PROG2_BELEG_GUI_BROWSE_PANEL_H

#include <wx/wx.h>
#include <wx/aui/auibook.h>

namespace pb2 {
    class BrowsePanel;

    /**
     * Event filter to catch global refresh messages (PB2_EVT_REFRESH_REQUIRED).
     */
    class BrowsePanelEventFilter : public wxEventFilter {
    private:
        BrowsePanel * panel = nullptr;
    public:
        BrowsePanelEventFilter(BrowsePanel * panel);
        virtual ~BrowsePanelEventFilter();

        virtual int FilterEvent(wxEvent& event) override;
    };

    /**
     * Base class for UserBrowsePanel and MediaBrowsePanel
     * Centralizes View refresh handling logic.
     */
    class BrowsePanel : public wxPanel {
    private:
        BrowsePanelEventFilter filter;
    public:
        BrowsePanel(wxAuiNotebook * parent);

        /* Refresh own Views, send event to parent for other tables */
        void refreshCascade();

        /* Refresh ONLY own views, do NOT cascade */
        virtual void refresh() = 0;

        inline wxAuiNotebook * castedParent() const { return (wxAuiNotebook*)GetParent(); }
    };
}

#endif
