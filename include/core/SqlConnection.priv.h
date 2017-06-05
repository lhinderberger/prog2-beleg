#ifndef PROG2_BELEG_SQL_CONNECTION_PRIV_H
#define PROG2_BELEG_SQL_CONNECTION_PRIV_H

#include <sqlite3.h>

namespace pb2 {
    struct SqlConnection_priv {
        sqlite3 * connection = nullptr;
    };
}

#endif
