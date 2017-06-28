#include "gui/Basket.h"

using namespace std;
using namespace pb2;

void Basket::add(shared_ptr<MediumCopy> mediumCopy) {
    /* Abort if exactly the same object is already in the list */
    if (rawFind(mediumCopy.get()) != list.cend())
        return;

    /* Abort if an object with the same primary key is already in the list */
    if (primaryKeyFind(mediumCopy) != list.cend())
        return;

    /* Insert and notify listeners */
    list.push_back(mediumCopy);
    notifyListeners(BasketEventType::ADD, mediumCopy);
}

int Basket::addListener(const BasketListenerFunc & listener) {
    listeners.insert({ nextToken, listener });
    return nextToken++; // Post-increment - returns value of nextToken before increment (as inserted)
}

void Basket::clear() {
    list.clear();
    notifyListeners(BasketEventType::CLEAR, nullptr);
}

void Basket::notifyListeners(BasketEventType type, shared_ptr<MediumCopy> mediumCopy) {
    for (auto & listenerPair : listeners)
        listenerPair.second(type, mediumCopy);
}

BasketList::iterator Basket::rawFind(const MediumCopy * rawPtr) {
    for (BasketList::iterator it = list.begin(); it != list.end(); it++) {
        const shared_ptr<MediumCopy> & smartPtr = *it;
        if (smartPtr.get() == rawPtr)
            return it;
    }
    return list.end();
}

BasketList::iterator Basket::primaryKeyFind(const shared_ptr<MediumCopy> & mediumCopy) {
    for (BasketList::iterator it = list.begin(); it != list.end(); it++) {
        const MediumCopy & otherMediumCopy = **it;
        if (
                otherMediumCopy.getSerialNumber() == mediumCopy->getSerialNumber() &&
                otherMediumCopy.getMediumEAN() == mediumCopy->getMediumEAN()
        )
            return it;
    }
    return list.end();
}

// Note: Remove functions use iterators + erase, as duplicates are guaranteed to not be present
void Basket::remove(const shared_ptr<MediumCopy> & mediumCopy) {
    remove(rawFind(mediumCopy.get()));
}

void Basket::remove(const MediumCopy * mediumCopy) {
    remove(rawFind(mediumCopy));
}

void Basket::remove(BasketList::iterator it) {
    /* Abort if invalid iterator is passed (i.e. due to item not found) */
    if (it == list.end())
        return;

    /* Erase and notify */
    auto mediumCopy = *it;
    list.erase(it);
    notifyListeners(BasketEventType::REMOVE, mediumCopy);
}

void Basket::removeListener(int token) {
    listeners.erase(token);
}
