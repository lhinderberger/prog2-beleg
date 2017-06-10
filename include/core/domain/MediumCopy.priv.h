#ifndef PROG2_BELEG_MEDIUMCOPY_PRIV_H
#define PROG2_BELEG_MEDIUMCOPY_PRIV_H

#include <memory>
#include <string>

#include "Medium.h"

namespace pb2 {
    struct MediumCopy_priv {
        std::shared_ptr<Medium> medium;
        bool deaccessioned;
        std::string location;
        int serialNumber;
    };
}

#endif
