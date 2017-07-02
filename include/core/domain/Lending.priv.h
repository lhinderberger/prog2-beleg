#ifndef PROG2_BELEG_LENDING_PRIV_H
#define PROG2_BELEG_LENDING_PRIV_H

#include <ctime>
#include <memory>

#include "LibraryUser.h"
#include "MediumCopy.h"
#include "core/ManyToOneLazyLoader.h"

namespace pb2 {
    class Lending_priv {
    public:
        Lending_priv(std::shared_ptr<Database> database)
                : libraryUser(database) {}

        int id, mediumCopySerialNumber;
        std::shared_ptr<MediumCopy> mediumCopy;
        std::string mediumEan;
        ManyToOneLazyLoader<LibraryUser,int> libraryUser;
        time_t timestampLent, timestampReturned;
        std::tm dueDate;
        unsigned int timesExtended;
    };
}

#endif
