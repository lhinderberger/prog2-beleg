#include "core/Book.h"
#include "core/exceptions.h"

#include "core/Medium.priv.h" // Book_priv is defined there as long as it's empty.

using namespace std;
using namespace pb2;

string isbn_to_ean(const string & isbn) {
    throw NotImplementedException();
}

Book::Book(shared_ptr<Database> database, const string &isbn)
: Medium(database, isbn_to_ean(isbn)){

}

shared_ptr<Book> Book::construct(shared_ptr<Database> database, const string &isbn) {
    return shared_ptr<Book>(new Book(database, isbn));
}

const string & Book::getType() const {
    static string t("book");
    return t;
}
