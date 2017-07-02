#ifndef PROG2_BELEG_VIDEO_H
#define PROG2_BELEG_VIDEO_H

#include "Medium.h"
#include "core/DatabaseObjectFactory.h"

namespace pb2 {
    class Video_priv;

    /**
     * Represents one Video type Medium entity in the Database.
     */
    class Video : public Medium {
        friend class DatabaseObjectFactory<Video>;

    private:
        std::unique_ptr<Video_priv> priv;

        /**
         * Creates a new Video Medium
         * @param ean @see Medium constructor
         */
        Video(std::shared_ptr<Database> database, const std::string & ean);
        PB2_DECLARE_LOAD_CONSTRUCTOR(Video);

    public:
        virtual ~Video();

        virtual const std::string & getType() const override;
    };
}

#endif
