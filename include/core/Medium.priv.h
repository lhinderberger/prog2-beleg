#ifndef PROG2_BELEG_MEDIUM_PRIV_H
#define PROG2_BELEG_MEDIUM_PRIV_H

#include <memory>
#include <string>

#include "Author.h"

namespace pb2 {
    struct Medium_priv {
        std::string ean, format, subtitle, title;
        std::shared_ptr<Author> author;
    };

    struct Book_priv {};
    struct Audio_priv {};
    struct Video_priv {};
    struct Software_priv {};
}

#endif
