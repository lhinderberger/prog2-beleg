#include "core/Author.h"
#include "core/Author.priv.h"
#include "core/exceptions.h"

using namespace std;
using namespace pb2;

Author::Author() {
    priv = make_unique<Author_priv>();
}


string Author::getFirstName() const {
    return priv->firstName;
}

void Author::setFirstName(const string & firstName) {
    //TODO: Validate
    throw NotImplementedException();

    priv->firstName = firstName;
}

string Author::getLastName() const {
    return priv->lastName;
}

void Author::setLastName(const string & lastName) {
    //TODO: Validate
    throw NotImplementedException();

    priv->lastName = lastName;
}
