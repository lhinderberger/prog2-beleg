#include "core/exceptions.h"

#include <sstream>

using namespace std;
using namespace pb2;

FileExistsException::FileExistsException(const string & filename) {
    this->filename = filename;
}


ColumnNotFoundException::ColumnNotFoundException(const string & fullColumnName) {
    this->fullColumnName = fullColumnName;
    description = "Column not found: " + fullColumnName;
}


SqliteException::SqliteException(int errorCode) {
    sqliteErrorCode = errorCode;
    sqliteErrorString = sqlite3_errstr(errorCode);
    buildWhatString();
}

SqliteException::SqliteException(sqlite3 * db) {
    sqliteErrorCode = sqlite3_extended_errcode(db);
    sqliteErrorString = sqlite3_errmsg(db);
    buildWhatString();
}

void SqliteException::buildWhatString() {
    stringstream stream;
    stream << "SqliteException - Code " << sqliteErrorCode << ": " << sqliteErrorString;
    whatString = stream.str();
}

const char * SqliteException::what() const noexcept {
    return whatString.c_str();
}

