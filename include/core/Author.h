#ifndef PROG2_BELEG_AUTHOR_H
#define PROG2_BELEG_AUTHOR_H

#include <memory>
#include <string>

namespace pb2 {
    class Author_priv;

    class Author {
    private:
        std::unique_ptr<Author_priv> priv;
    public:
        Author();
        
        std::string getFirstName() const;
        void setFirstName(const std::string & firstName);

        std::string getLastName() const;
        void setLastName(const std::string & lastName);
    };
}

#endif