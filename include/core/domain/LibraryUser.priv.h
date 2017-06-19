#ifndef PROG2_BELEG_LIBRARY_USER_PRIV_H
#define PROG2_BELEG_LIBRARY_USER_PRIV_H

#include "PostalAddress.h"
#include "core/ManyToOneLazyLoader.h"

#include <memory>
#include <string>

namespace pb2 {
    struct LibraryUser_priv {
        LibraryUser_priv(std::shared_ptr<Database> database) : postalAddress(database, "id") {}

        int id;
        std::string firstName, lastName, telephone;
        ManyToOneLazyLoader<PostalAddress, int> postalAddress;
    };
}

#endif