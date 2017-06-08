#include "core/Author.h"
#include "core/Author.priv.h"
#include "core/exceptions.h"

using namespace std;
using namespace pb2;

Author::Author(shared_ptr<Database> database, int id) : DatabaseObject(database) {
    priv = make_unique<Author_priv>();
    priv->id = id;
}

shared_ptr<Author> Author::construct(shared_ptr<Database> database, int id) {
    return shared_ptr<Author>(new Author(database, id));
}

const string & Author::getTableName() const {
    static string t("author");
    return t;
}

const std::string & Author::getType() const {
    static string t("author");
    return t;
}


void Author::loadImpl(SqlPreparedStatement & query, const map<string, int> & columnIndexes) {
    throw NotImplementedException();
}

void Author::persistImpl() {
    throw NotImplementedException();
}

string Author::getFirstName() const {
    return priv->firstName;
}

void Author::setFirstName(const string & firstName) {
    priv->firstName = firstName;
}

string Author::getLastName() const {
    return priv->lastName;
}

void Author::setLastName(const string & lastName) {
    priv->lastName = lastName;
}
