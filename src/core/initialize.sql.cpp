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
        author_id INTEGER,
        type TEXT NOT NULL,
        format TEXT, title TEXT, subtitle TEXT,
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
    CREATE TABLE postal_addresses(
        id INTEGER PRIMARY KEY,
        street TEXT NOT NULL,
        house_number TEXT NOT NULL,
        addition TEXT NOT NULL,
        zip TEXT NOT NULL,
        city TEXT NOT NULL,
        CONSTRAINT address_unique UNIQUE (street, house_number, addition, zip, city)
    );
    CREATE TABLE library_users(
        id INTEGER PRIMARY KEY,
        first_name TEXT NOT NULL,
        last_name TEXT NOT NULL,
        telephone TEXT NOT NULL,
        postal_address_id INTEGER NOT NULL,
        FOREIGN KEY(postal_address_id) REFERENCES postal_addresses(id)
    );
    CREATE TABLE lendings(
        medium_ean TEXT NOT NULL,
        medium_copy_serial_number INTEGER NOT NULL,
        library_user_id INTEGER NOT NULL,
        timestamp_lent INTEGER NOT NULL,
        timestamp_returned INTEGER NOT NULL,
        times_extended UNSIGNED INTEGER NOT NULL,
        due_date TEXT NOT NULL,
        FOREIGN KEY(medium_ean, medium_copy_serial_number) REFERENCES media_copies(medium_ean, serial_number),
        FOREIGN KEY(library_user_id) REFERENCES library_users(id),
        PRIMARY KEY (medium_ean, medium_copy_serial_number, timestamp_lent),
        CONSTRAINT returned_unique UNIQUE (medium_ean, medium_copy_serial_number, timestamp_returned)
    );
    INSERT INTO meta(name, value) VALUES('version', '1');
    INSERT INTO meta(name, value) VALUES('default_lending_runtime', '28');
    INSERT INTO meta(name, value) VALUES('default_extend_days', '14');
)---";
