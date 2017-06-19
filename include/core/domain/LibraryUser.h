#ifndef PROG2_BELEG_LIBRARY_USER_H
#define PROG2_BELEG_LIBRARY_USER_H

#include <memory>
#include <string>

#include "core/DatabaseObject.h"
#include "core/DatabaseObjectFactory.h"
#include "PostalAddress.h"

namespace pb2 {
    class LibraryUser_priv;

    class LibraryUser : public DatabaseObject {
        friend class DatabaseObjectFactory<LibraryUser>;

    private:
        std::unique_ptr<LibraryUser_priv> priv;

        /**
         * Creates a new LibraryUser
         *
         * @param id Pass in 0 or less to auto-generate an ID on insert.
         * Otherwise pass in a valid ID.
         */
        LibraryUser(std::shared_ptr<Database> database, int id);
        PB2_DECLARE_LOAD_CONSTRUCTOR(LibraryUser);

    protected:
        virtual void persistImpl() override;

    public:
        static const std::string tableName;

        virtual ~LibraryUser();


        int getId() const;

        std::string getFirstName() const;
        void setFirstName(const std::string & firstName);

        std::string getLastName() const;
        void setLastName(const std::string & lastName);

        std::string getTelephone() const;
        void setTelephone(const std::string & telephone);


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