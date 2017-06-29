#ifndef PROG2_BELEG_GUI_LENDINGS_CONTROLLER_H
#define PROG2_BELEG_GUI_LENDINGS_CONTROLLER_H

#include <memory>
#include "Basket.h"
#include "core/domain/Lending.h"

namespace pb2 {
    /**
     * Provides GUI dialogs for Lending extension / return.
     * Actual business logic is in core/domain/Lending
     */
    class LendingsController {
    private:
        std::shared_ptr<Basket> basket;

    public:
        LendingsController(std::shared_ptr<Basket> basket);

        void extend(std::shared_ptr<Lending> lending);
        void returnL(std::shared_ptr<Lending> lending);
    };
}

#endif
