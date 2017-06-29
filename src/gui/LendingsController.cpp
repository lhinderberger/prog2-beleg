#include "gui/LendingsController.h"

using namespace pb2;
using namespace std;

LendingsController::LendingsController(std::shared_ptr<Basket> basket)
    : basket(basket) {

}

void LendingsController::extend(shared_ptr<Lending> lending) {
    throw NotImplementedException();
}

void LendingsController::returnL(shared_ptr<Lending> lending) {
    throw NotImplementedException();
}
