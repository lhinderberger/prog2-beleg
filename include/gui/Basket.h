#ifndef PROG2_BELEG_GUI_BASKET_H
#define PROG2_BELEG_GUI_BASKET_H

#include <functional>
#include <list>
#include "core/domain/MediumCopy.h"

namespace pb2 {
    enum class BasketEventType { ADD, REMOVE, CLEAR };

    typedef std::list<std::shared_ptr<MediumCopy>> BasketList;
    typedef std::function<void(BasketEventType, std::shared_ptr<MediumCopy>)> BasketListenerFunc;

    /**
     * A basket of Medium Copies that are to be selected for later lending to a
     * Library User.
     * This class contains the storage / model part of BasketPanel, in order to separate
     * responsibilities.
     * You can register listeners on it that react to changes in the Basket contents.
     */
    class Basket {
    private:
        BasketList list;
        std::map<int, BasketListenerFunc> listeners;
        int nextToken = 1;

        void notifyListeners(BasketEventType type, std::shared_ptr<MediumCopy> mediumCopy);
        BasketList::iterator rawFind(const MediumCopy * rawPtr);
        BasketList::iterator primaryKeyFind(const std::shared_ptr<MediumCopy> & mediumCopy);
        void remove(BasketList::iterator it);

    public:
        /**
         * Add a listener function to the list of listener functions.
         * The listener will be called each time the contents of the Basket change.
         * @return An integer token that can be used for removing the listener again.
         */
        int addListener(const BasketListenerFunc & listener);

        /**
         * Removes the listener function with the given token (assigned by addListener())
         * from the Basket.
         */
        void removeListener(int token);

        /**
         * Add a medium copy to the basket list, provided it is not already on there
         * (in that case this call will be ignored).
         */
        void add(std::shared_ptr<MediumCopy> mediumCopy);

        /**
         * Remove all items from the basket list.
         */
        void clear();

        /**
         * Remove a medium copy from the basket list again, provided it is already on
         * there (otherwise this call will be ignored).
         * @param mediumCopy
         */
        void remove(const std::shared_ptr<MediumCopy> & mediumCopy);

        /**
         * Overload to be able to remove medium copies based on their raw pointer.
         */
        void remove(const MediumCopy * mediumCopy);

        inline const BasketList & getList() const { return list; }
    };
}

#endif