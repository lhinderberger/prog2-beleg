#include "core/Database.priv.h"

const char * initializingSQL = R"---(
    CREATE TABLE meta(name text PRIMARY KEY, value TEXT);
    CREATE TABLE authors(
        id INTEGER PRIMARY KEY,
        first_name TEXT, -- Usually, this would be something like a CHAR(32), but SQLite does not have fixed-size strings
        last_name TEXT
    );
    INSERT INTO meta(name, value) VALUES('version', '1');
)---";
