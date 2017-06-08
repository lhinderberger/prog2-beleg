#ifndef PROG2_BELEG_VIDEO_H
#define PROG2_BELEG_VIDEO_H

#include "Medium.h"

namespace pb2 {
    class Video_priv;

    class Video : public Medium {
    private:
        std::unique_ptr<Video_priv> priv;
        Video(std::shared_ptr<Database> database, const std::string & ean);

    public:
        /**
         * Creates a new Video Medium
         * @param ean @see Medium constructor
         */
        static std::shared_ptr<Video> construct(std::shared_ptr<Database> database,
                                                const std::string & ean);

        virtual const std::string & getType() const override;
    };
}

#endif
