#ifndef PROG2_BELEG_POSTAL_ADDRESS_PRIV_H
#define PROG2_BELEG_POSTAL_ADDRESS_PRIV_H

#include <string>

namespace pb2 {
    struct PostalAddress_priv {
        int id;
        std::string street, houseNumber, addition, zip, city;
    };
}

#endif
