#ifndef PROG2_BELEG_BOOK_H
#define PROG2_BELEG_BOOK_H

#include <memory>
#include <string>
#include "Medium.h"

namespace pb2 {
    class Book_priv;

    class Book : public Medium {
    private:
        std::unique_ptr<Book_priv> priv;
        Book(std::shared_ptr<Database> database, const std::string & isbn);

    public:
        /**
         * Creates a new Book Medium
         * @param isbn Either supply an EAN-13 or the keyword "generate" here (as
         * specified in the constructor of Medium) or enter any valid ISBN-10 for it to
         * be transferred into the corresponding EAN-13.
         */
        static std::shared_ptr<Book> construct(std::shared_ptr<Database> database,
                                               const std::string & isbn);

        virtual const std::string & getType() const override;
    };
}

#endif
