#ifndef PROG2_BELEG_SQL_PREPARED_STATEMENT_PRIV_H
#define PROG2_BELEG_SQL_PREPARED_STATEMENT_PRIV_H

#include <memory>
#include <map>
#include <string>
#include <sqlite3.h>

#include "SqlConnection.h"

namespace pb2 {
    struct SqlPreparedStatement_priv {
        sqlite3_stmt * statement;
        std::shared_ptr<SqlConnection> connection = nullptr;
        std::unique_ptr<std::map<std::string, int>> columnIndexes = nullptr;
    };
}

#endif
