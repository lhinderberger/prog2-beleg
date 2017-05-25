#include "core/Book.h"
#include "core/exceptions.h"

#include "core/Medium.priv.h" // Book_priv is defined there as long as it's empty.

using namespace std;
using namespace pb2;

string isbn_to_ean(const string & isbn) {
    throw NotImplementedException();
}

Book::Book(const string &isbn)
: Medium(isbn_to_ean(isbn)){

}

shared_ptr<Book> Book::construct(const string &isbn) {
    return shared_ptr<Book>(new Book(isbn));
}

string Book::getType() const {
    return "book";
}
