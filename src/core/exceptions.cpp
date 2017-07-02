#include "core/exceptions.h"

#include <sstream>

using namespace std;
using namespace pb2;

Exception::Exception() : Exception("Unknown Error") {}
Exception::Exception(const std::string & whatString) : whatString(whatString) {}
const char* Exception::what() const noexcept { return whatString.c_str(); }

BASIC_EXCEPTION_IMPL(CoreException, Exception, "Error in core")

/* === General Exceptions === */
BASIC_EXCEPTION_IMPL(LogicError, Exception, "Program Logic Error")
BASIC_EXCEPTION_IMPL(NotImplementedException, Exception, "Feature not implemented")
BASIC_EXCEPTION_IMPL(NullPointerException, Exception, "Null pointer is not allowed here")

/* === Core Exceptions === */
BASIC_EXCEPTION_IMPL(DatabaseFileExistsException, CoreException, "The database file given would be overwritten")
BASIC_EXCEPTION_IMPL(DatabaseFormatException, CoreException, "The database integrity has been compromised")
BASIC_EXCEPTION_IMPL(DatabaseIntegrityException, CoreException, "The database integrity has been compromised")
BASIC_EXCEPTION_IMPL(NotExtensibleException, CoreException, "The lending is not (yet) extensible")
BASIC_EXCEPTION_IMPL(SqlColumnNotFoundException, CoreException, "A requested SQL column was not found in the result set")
BASIC_EXCEPTION_IMPL(SqlStringTooLongException, CoreException, "A string passed into / from sqlite was too long")
BASIC_EXCEPTION_IMPL(ValidationException, CoreException, "Input validation failed")

SqliteException::SqliteException(int errorCode) : CoreException(buildWhatString(errorCode)) {
    sqliteErrorCode = errorCode;
}
SqliteException::SqliteException(sqlite3 * db) : CoreException(buildWhatString(db)) {
    sqliteErrorCode = sqlite3_extended_errcode(db);
}

string SqliteException::buildWhatString(int errorCode) {
    return buildWhatString(errorCode, sqlite3_errstr(errorCode));
}

string SqliteException::buildWhatString(sqlite3 * db) {
    return buildWhatString(sqlite3_extended_errcode(db), sqlite3_errmsg(db));
}

string SqliteException::buildWhatString(int errCode, const std::string & errString) {
    stringstream stream;
    stream << "SqliteException - Code " << errCode << ": " << errString;
    return stream.str();
}
