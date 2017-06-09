#ifndef PROG2_BELEG_DATABASE_PRIV_H
#define PROG2_BELEG_DATABASE_PRIV_H

#include <memory>
#include <sqlite3.h>

#include "SqlConnection.h"

extern const char * initializingSQL;

namespace pb2 {
    struct Database_priv {
        std::shared_ptr<SqlConnection> connection = nullptr;
    };
}

#endif
