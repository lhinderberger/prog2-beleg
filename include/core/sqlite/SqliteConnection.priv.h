#ifndef PROG2_BELEG_SQL_CONNECTION_PRIV_H
#define PROG2_BELEG_SQL_CONNECTION_PRIV_H

#include <sqlite3.h>
#include <mutex>

namespace pb2 {
    struct SqliteConnection_priv {
        sqlite3 * connection = nullptr;
        std::mutex writeLock;
    };
}

#endif
