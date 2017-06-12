#include "core/Database.priv.h"

const char * initializingSQL = R"---(
    CREATE TABLE meta(name text PRIMARY KEY, value TEXT);
    CREATE TABLE authors(
        id INTEGER PRIMARY KEY,
        first_name TEXT, -- Usually, this would be something like a CHAR(32), but SQLite does not have fixed-size strings
        last_name TEXT
    );
    CREATE TABLE media(
        ean TEXT PRIMARY KEY,
        type TEXT,
        format TEXT, title TEXT, subtitle TEXT, author_id INTEGER,
        FOREIGN KEY(author_id) REFERENCES authors(id)
    );
    CREATE TABLE media_copies(
        medium_ean TEXT,
        serial_number INT,
        deaccessioned INT, -- boolean value, but SQLite lacks a dedicated type
        location TEXT,
        PRIMARY KEY(medium_ean, serial_number),
        FOREIGN KEY(medium_ean) REFERENCES media(ean)
    );
    INSERT INTO meta(name, value) VALUES('version', '1');
)---";
