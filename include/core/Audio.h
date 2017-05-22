#ifndef PROG2_BELEG_AUDIO_H
#define PROG2_BELEG_AUDIO_H

#include "Medium.h"

namespace pb2 {
    class Audio_priv;

    class Audio : public Medium {
    private:
        std::unique_ptr<Audio_priv> priv;

    public:
        /**
         * Creates a new Audio Medium
         * @param ean @see Medium constructor
         */
        Audio(const std::string & ean);

        virtual std::string getType() const;
    };
}

#endif
