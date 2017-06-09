#include "core/Database.priv.h"

const char * initializingSQL = R"---(
    CREATE TABLE meta(name CHAR(16) PRIMARY KEY, value TEXT);
    CREATE TABLE authors(
        id INTEGER PRIMARY KEY,
        first_name CHAR(32),
        last_name CHAR(32)
    );
    INSERT INTO meta(name, value) VALUES('version', '1');
)---";
