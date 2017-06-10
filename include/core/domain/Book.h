#ifndef PROG2_BELEG_BOOK_H
#define PROG2_BELEG_BOOK_H

#include <memory>
#include <string>
#include "Medium.h"
#include "core/DatabaseObjectFactory.h"

namespace pb2 {
    class Book_priv;

    class Book : public Medium {
        friend class DatabaseObjectFactory<Book>;

    private:
        std::unique_ptr<Book_priv> priv;

        /**
         * Creates a new Book Medium
         * @param isbn Either supply an EAN-13 or the keyword "generate" here (as
         * specified in the constructor of Medium) or enter any valid ISBN-10 for it to
         * be transferred into the corresponding EAN-13.
         */
        Book(std::shared_ptr<Database> database, const std::string & isbn);
        PB2_DECLARE_LOAD_CONSTRUCTOR(Book);

    public:
        virtual ~Book();

        virtual const std::string & getType() const override;
    };
}

#endif
