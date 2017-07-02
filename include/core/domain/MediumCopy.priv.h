#ifndef PROG2_BELEG_MEDIUMCOPY_PRIV_H
#define PROG2_BELEG_MEDIUMCOPY_PRIV_H

#include <memory>
#include <string>

#include "Medium.h"
#include "core/Database.h"
#include "core/PolymorphicDatabaseObjectFactory.h"
#include "core/ManyToOneLazyLoader.h"

namespace pb2 {
    class MediumCopy_priv {
    public:
        MediumCopy_priv(std::shared_ptr<Database> database) : medium(database) {}

        ManyToOneLazyLoader<Medium,std::string,PolymorphicDatabaseObjectFactory<Medium>> medium;
        bool deaccessioned;
        std::string location, availabilityHint;
        int serialNumber;
    };
}

#endif
