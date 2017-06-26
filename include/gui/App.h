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
        MainFrame * mainFrame = nullptr;
        std::unique_ptr<wxLocale> locale;

    public:
        virtual ~App();
        virtual bool OnInit() override;
        virtual void OnFatalException() override;
        virtual void OnUnhandledException() override;

        inline const MainFrame & getMainFrame() const { return *mainFrame; } //TODO: Is this correct?
        inline MainFrame & getMainFrame() { return *mainFrame; }
        inline wxLocale & getLocale() { return *locale; }
    };
}

#endif
