#ifndef PROG2_BELEG_POSTAL_ADDRESS_H
#define PROG2_BELEG_POSTAL_ADDRESS_H

#include "core/DatabaseObject.h"
#include "core/DatabaseObjectFactory.h"

#include <string>

namespace pb2 {
    class PostalAddress_priv;

    class PostalAddress : public DatabaseObject {
        friend class DatabaseObjectFactory<PostalAddress>;

    private:
        std::unique_ptr<PostalAddress_priv> priv;

        /**
         * Creates a new PostalAddress
         *
         * @param id Pass in 0 or less to auto-generate an ID on insert.
         * Otherwise pass in a valid ID.
         */
        PostalAddress(std::shared_ptr<Database> database, int id);
        PB2_DECLARE_LOAD_CONSTRUCTOR(PostalAddress);

    protected:
        virtual void persistImpl() override;

    public:
        static const std::string tableName;

        virtual ~PostalAddress();

        int getId() const;

        std::string getStreet() const;
        void setStreet(const std::string & street);

        std::string getHouseNumber() const;
        void setHouseNumber(const std::string & houseNumber);

        std::string getAddition() const;
        void setAddition(const std::string & addition);

        std::string getZip() const;
        void setZip(const std::string & zip);

        std::string getCity() const;
        void setCity(const std::string & city);
    };
}

#endif