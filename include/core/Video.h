#ifndef PROG2_BELEG_VIDEO_H
#define PROG2_BELEG_VIDEO_H

#include "Medium.h"

namespace pb2 {
    class Video_priv;

    class Video : public Medium {
    private:
        std::unique_ptr<Video_priv> priv;

    public:
        /**
         * Creates a new Video Medium
         * @param ean @see Medium constructor
         */
        Video(const std::string & ean);

        virtual std::string getType() const;
    };
}

#endif
