#include "core/Author.h"
#include "core/Author.priv.h"
#include "core/exceptions.h"

using namespace std;
using namespace pb2;

Author::Author(shared_ptr<Database> database) : DatabaseObject(database) {
    priv = make_unique<Author_priv>();
}

shared_ptr<Author> Author::construct(shared_ptr<Database> database) {
    return shared_ptr<Author>(new Author(database));
}

const string & Author::getTableName() const {
    static string t("author");
    return t;
}

const std::string & Author::getType() const {
    static string t("author");
    return t;
}


void Author::load(SqlPreparedStatement & query,
                  const map<string, string> & alternativeColumnNames) {
    throw NotImplementedException();
}

void Author::persist() {
    throw NotImplementedException();
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
