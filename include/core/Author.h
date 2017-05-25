#ifndef PROG2_BELEG_AUTHOR_H
#define PROG2_BELEG_AUTHOR_H

#include <memory>
#include <string>

namespace pb2 {
    class Author_priv;

    class Author : public std::enable_shared_from_this<Author> {
    private:
        std::unique_ptr<Author_priv> priv;
    protected:
        Author();

    public:
        static std::shared_ptr<Author> construct();
        
        std::string getFirstName() const;
        void setFirstName(const std::string & firstName);

        std::string getLastName() const;
        void setLastName(const std::string & lastName);
    };
}

#endif