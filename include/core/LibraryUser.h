#ifndef PROG2_BELEG_LIBRARY_USER_H
#define PROG2_BELEG_LIBRARY_USER_H

#include <memory>
#include <string>

#include "PostalAddress.h"
#include "TelephoneNumber.h"

namespace pb2 {
    class LibraryUser_priv;

    class LibraryUser : public std::enable_shared_from_this<LibraryUser> {
    private:
        std::unique_ptr<LibraryUser_priv> priv;

        LibraryUser(int id);

    public:
        /**
         * Creates a new LibraryUser
         *
         * @param id Pass in 0 or less to auto-generate an ID on insert.
         * Otherwise pass in a valid ID.
         */
        static std::shared_ptr<LibraryUser> construct(int id);

        int getId() const;

        std::string getFirstName() const;
        void setFirstName(const std::string & firstName);

        std::string getLastName() const;
        void setLastName(const std::string & lastName);

        TelephoneNumber getTelephone() const;
        void setTelephone(const TelephoneNumber & telephone);


        /** Lazy-loads the postal address of the LibraryUser */
        std::shared_ptr<PostalAddress> getPostalAddress() const;
        /**
         * Sets the postal address of the LibraryUser.
         *
         * Make sure the address you set here is already persisted before persisting
         * the LibraryUser.
         */
        void setPostalAddress(std::shared_ptr<PostalAddress> postalAddress);
    };
}

#endif