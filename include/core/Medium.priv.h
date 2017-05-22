#ifndef PROG2_BELEG_MEDIUM_PRIV_H
#define PROG2_BELEG_MEDIUM_PRIV_H

#include <string>

namespace pb2 {
    struct Medium_priv {
        std::string ean, format, subtitle, title;
    };

    struct Book_priv {};
    struct Audio_priv {};
    struct Video_priv {};
    struct Software_priv {};
}

#endif
