#ifndef PROG2_BELEG_AUTHOR_H
#define PROG2_BELEG_AUTHOR_H

#include <memory>
#include <string>

#include "core/DatabaseObject.h"
#include "core/DatabaseObjectFactory.h"

namespace pb2 {
    class Author_priv;

    class Author : public DatabaseObject {
        friend class DatabaseObjectFactory<Author>;

    private:
        std::unique_ptr<Author_priv> priv;

        /**
         * Creates a new Author. If the ID passed in is 0 or less, an ID will be auto-
         * generated.
         */
        Author(std::shared_ptr<Database> database, int id);
        PB2_DECLARE_LOAD_CONSTRUCTOR(Author);

    protected:
        virtual void persistImpl() override;

    public:
        virtual ~Author();

        static const std::string tableName;

        int getId() const;
        
        std::string getFirstName() const;
        void setFirstName(const std::string & firstName);

        std::string getLastName() const;
        void setLastName(const std::string & lastName);
    };
}

#endif