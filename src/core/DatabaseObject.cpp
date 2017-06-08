#include "core/DatabaseObject.h"
#include "core/exceptions.h"

using namespace std;
using namespace pb2;

DatabaseObject::DatabaseObject(std::shared_ptr<Database> database) {
    if (!database)
        throw NullPointerException();

    this->database = database;
}

DatabaseObject::~DatabaseObject() {}
