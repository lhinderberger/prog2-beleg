#include "core/Database.priv.h"

const char * exampleDataSQL = R"---(
    INSERT INTO postal_addresses(id, street, house_number, addition, zip, city) VALUES
        (1, "Nürnberger Straße", "22", "", "01187", "Dresden"),
        (2, "Nürnberger Straße", "24", "", "01187", "Dresden"),
        (3, "Friedrich-List-Platz", "1", "", "01069", "Dresden"),
        (4, "Dresdner Straße", "207", "", "01705", "Freital");

    INSERT INTO library_users(id, first_name, last_name, telephone, postal_address_id) VALUES
        (1, "Lucas", "Hinderberger", "0351 123456", 3),
        (2, "Max", "Mustermann", "0351 234567", 1),
        (3, "Maria", "Mustermann", "0151 123456", 2),
        (4, "Markus", "Mustermeier", "0151 234567", 4);

    INSERT INTO authors(id, first_name, last_name) VALUES
        (1, "Scott", "Meyers"), (2, "Kaiser", "Ulrich");

    INSERT INTO media(ean, author_id, type, format, title, subtitle) VALUES
        ("978-3958750494", 1, "book", "hardcover", "Effektives modernes C++",
         "43 Techniken für besseren C++11 und C++14-Code"),
        ("978-3827330789", 1, "book", "hardcover", "Effektiv C++ programmieren",
         "55 Möglichkeiten, Ihre Programme und Entwürfe zu verbessern"),
        ("978-3898428392", 2, "book", "hardcover", "C/C++", "Das umfassende Lehrbuch");

    INSERT INTO media_copies(medium_ean, serial_number, deaccessioned, location, availability_hint) VALUES
        ("978-3958750494", 1, 0, "A123", "now"), ("978-3958750494", 2, 0, "A123", "now"),
        ("978-3827330789", 1, 1, "A124", "now"),
        ("978-3898428392", 1, 0, "B125", "now"), ("978-3898428392", 2, 1, "B125", "now");
)---";