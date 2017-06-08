#ifndef PROG2_BELEG_AUTHOR_H
#define PROG2_BELEG_AUTHOR_H

#include <memory>
#include <string>

#include "core/DatabaseObject.h"

namespace pb2 {
    class Author_priv;

    class Author : public DatabaseObject {
    private:
        std::unique_ptr<Author_priv> priv;
    protected:
        Author(std::shared_ptr<Database> database);

    public:
        static std::shared_ptr<Author> construct(std::shared_ptr<Database> database);

        virtual const std::string & getTableName() const override;
        virtual const std::string & getType() const override;

        virtual void load(SqlPreparedStatement & query,
            const std::map<std::string, std::string> & alternativeColumnNames
            = std::map<std::string, std::string>()) override;
        virtual void persist() override;
        
        std::string getFirstName() const;
        void setFirstName(const std::string & firstName);

        std::string getLastName() const;
        void setLastName(const std::string & lastName);
    };
}

#endif