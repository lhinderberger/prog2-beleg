#ifndef PROG2_BELEG_GUI_LENDINGS_CONTROLLER_H
#define PROG2_BELEG_GUI_LENDINGS_CONTROLLER_H

#include <memory>
#include <wx/wx.h>
#include "Basket.h"
#include "core/domain/Lending.h"

namespace pb2 {
    /**
     * Provides GUI dialogs for Lending extension / return.
     * Actual business logic is in core/domain/Lending
     */
    class LendingsController {
    private:
        wxString getMediumCopyLabel(std::shared_ptr<MediumCopy> mediumCopy) const;
        bool warnOverdue(std::shared_ptr<Lending> lending, const wxString & dialogCaption) const;

    public:
        bool extend(std::shared_ptr<Lending> lending);
        bool returnL(std::shared_ptr<Lending> lending);
    };
}

#endif
