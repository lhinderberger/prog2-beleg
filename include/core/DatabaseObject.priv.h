#ifndef PROG2_BELEG_DATABASE_OBJECT_PRIV_H
#define PROG2_BELEG_DATABASE_OBJECT_PRIV_H

#include <memory>
#include "Database.h"

namespace pb2 {
    struct DatabaseObject_priv {
        std::shared_ptr<Database> database;
        bool loaded = false;
    };
}

#endif
