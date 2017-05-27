#ifndef PROG2_BELEG_LIBRARY_USER_PRIV_H
#define PROG2_BELEG_LIBRARY_USER_PRIV_H

#include "TelephoneNumber.h"
#include "PostalAddress.h"

#include <memory>
#include <string>

namespace pb2 {
    struct LibraryUser_priv {
        int id;
        std::string firstName, lastName;
        TelephoneNumber telephone;
        std::shared_ptr<PostalAddress> postalAddress;
    };
}

#endif