#ifndef PROG2_BELEG_AUDIO_H
#define PROG2_BELEG_AUDIO_H

#include "Medium.h"
#include "core/DatabaseObjectFactory.h"

namespace pb2 {
    class Audio_priv;

    /**
     * Represents one Audio type Medium entity in the Database.
     */
    class Audio : public Medium {
        friend class DatabaseObjectFactory<Audio>;

    private:
        std::unique_ptr<Audio_priv> priv;

        /**
         * Creates a new Audio Medium
         * @param ean @see Medium constructor
         */
        Audio(std::shared_ptr<Database> database, const std::string & ean);
        PB2_DECLARE_LOAD_CONSTRUCTOR(Audio);

    public:
        virtual ~Audio();

        virtual const std::string & getType() const override;
    };
}

#endif
