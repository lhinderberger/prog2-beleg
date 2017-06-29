#include "gui/BrowsePanel.h"
#include "gui/events.h"

using namespace pb2;
using namespace std;

BrowsePanelEventFilter::BrowsePanelEventFilter(BrowsePanel * panel)
    : panel(panel) {
    wxEvtHandler::AddFilter(this);
}

BrowsePanelEventFilter::~BrowsePanelEventFilter() {
    wxEvtHandler::RemoveFilter(this);
}

int BrowsePanelEventFilter::FilterEvent(wxEvent & event) {
    if (event.GetEventType() == PB2_EVT_REFRESH_REQUIRED && panel) {
        panel->refresh();
    }

    return Event_Skip;
}


BrowsePanel::BrowsePanel(wxWindow * parent)
        : wxPanel(parent, wxID_ANY), filter(this) {

}

void BrowsePanel::refreshCascade() {
    /* Send event to all, the event filter will take care of refreshing */
    wxCommandEvent event(PB2_EVT_REFRESH_REQUIRED, GetId());
    wxPostEvent(GetParent(), event);
}
