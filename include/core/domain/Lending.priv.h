#ifndef PROG2_BELEG_LENDING_PRIV_H
#define PROG2_BELEG_LENDING_PRIV_H

#include <ctime>
#include <memory>

#include "LibraryUser.h"
#include "MediumCopy.h"

namespace pb2 {
    struct Lending_priv {
        std::shared_ptr<MediumCopy> mediumCopy;
        std::shared_ptr<LibraryUser> libraryUser;
        time_t timestampLent, timestampReturned;
        std::tm dueDate;
        unsigned int timesExtended;
    };
}

#endif
