#ifndef PROG2_BELEG_GUI_APP_H
#define PROG2_BELEG_GUI_APP_H

#include <memory>
#include <wx/wx.h>
#include "MainFrame.h"

namespace pb2 {
    /**
     * The App class is the root of the prog2-beleg gui class hierarchy.
     * It overrides wxApp and as such is responsible for defining core initialization
     * and control flow logic.
     */
    class App : public wxApp {
    private:
        MainFrame * mainFrame;

    public:
        virtual ~App();
        virtual bool onInit();

        inline const MainFrame & getMainFrame() const { return *mainFrame; } //TODO: Is this correct?
        inline MainFrame & getMainFrame() { return *mainFrame; }
    };
}

#endif
