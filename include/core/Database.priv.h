#ifndef PROG2_BELEG_DATABASE_PRIV_H
#define PROG2_BELEG_DATABASE_PRIV_H

#include <map>
#include <memory>
#include <string>
#include <sqlite3.h>

#include "core/sqlite/SqliteConnection.h"

extern const char * exampleDataSQL;
extern const char * initializingSQL;

namespace pb2 {
    struct Database_priv {
        std::shared_ptr<SqliteConnection> connection = nullptr;
        std::map<std::string,std::string> meta;
    };
}

#endif
