#ifndef PROG2_BELEG_MEDIA_BROWSE_PANEL_H
#define PROG2_BELEG_MEDIA_BROWSE_PANEL_H

#include <wx/wx.h>

#include "MediaSearchTable.h"

namespace pb2 {
    class MediaBrowsePanel : public wxPanel {
    private:
        MediaSearchTable * searchTable = nullptr;

    public:
        MediaBrowsePanel(wxWindow * parent);
    };
}

#endif
