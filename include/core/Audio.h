#ifndef PROG2_BELEG_AUDIO_H
#define PROG2_BELEG_AUDIO_H

#include "Medium.h"

namespace pb2 {
    class Audio_priv;

    class Audio : public Medium {
    private:
        std::unique_ptr<Audio_priv> priv;
        Audio(std::shared_ptr<Database> database, const std::string & ean);

    public:
        /**
         * Creates a new Audio Medium
         * @param ean @see Medium constructor
         */
        static std::shared_ptr<Audio> construct(std::shared_ptr<Database> database,
                                                const std::string & ean);

        virtual const std::string & getType() const override;
    };
}

#endif
