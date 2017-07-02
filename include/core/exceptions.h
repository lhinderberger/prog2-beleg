#ifndef PROG2_BELEG_CORE_EXCEPTIONS_H
#define PROG2_BELEG_CORE_EXCEPTIONS_H

#include <stdexcept>
#include <sqlite3.h>

/**
 * This file declares all Exception classes for prog2-beleg.
 */

/* === Macroes to mitigate duplicate code === */
#define DEFAULT_EXCEPTION_CONSTRUCTORS(E_TYPE) E_TYPE(); \
E_TYPE(const std::string & whatString);

#define BASIC_EXCEPTION(E_TYPE, PARENT_TYPE) class E_TYPE : public PARENT_TYPE { \
public: \
    DEFAULT_EXCEPTION_CONSTRUCTORS(E_TYPE) \
};

#define BASIC_EXCEPTION_IMPL(E_TYPE, PARENT_TYPE, defaultWhat) E_TYPE::E_TYPE() : PARENT_TYPE(defaultWhat) {} \
E_TYPE::E_TYPE(const std::string & whatString) : PARENT_TYPE(std::string(defaultWhat) + "\n" + whatString) {}



namespace pb2 {
    /** Base class for all exceptions thrown from prog2-beleg */
    class Exception : public std::exception {
    private:
        const std::string whatString;

    public:
        DEFAULT_EXCEPTION_CONSTRUCTORS(Exception);

        virtual const char * what() const noexcept override;
    };

    /** Base class for all exceptions specifically thrown from the core library */
    BASIC_EXCEPTION(CoreException, Exception)

    /* === General Exceptions === */
    BASIC_EXCEPTION(LogicError, Exception)
    BASIC_EXCEPTION(NotImplementedException, Exception)
    BASIC_EXCEPTION(NullPointerException, Exception)

    /* === Core Exceptions === */
    BASIC_EXCEPTION(DatabaseFileExistsException, CoreException)
    BASIC_EXCEPTION(DatabaseFormatException, CoreException)
    BASIC_EXCEPTION(DatabaseIntegrityException, CoreException)
    BASIC_EXCEPTION(NotExtensibleException, CoreException)
    BASIC_EXCEPTION(SqlColumnNotFoundException, CoreException)
    BASIC_EXCEPTION(SqlStringTooLongException, CoreException)
    BASIC_EXCEPTION(ValidationException, CoreException)

    class SqliteException : public CoreException {
    private:
        int sqliteErrorCode;

        static std::string buildWhatString(int errorCode);
        static std::string buildWhatString(sqlite3 * db);
        static std::string buildWhatString(int errCode, const std::string & errString);

    public:
        SqliteException(int errorCode);
        SqliteException(sqlite3 * db);

        inline int getSqliteErrorCode() const { return sqliteErrorCode; }
    };

    class UnsupportedDatabaseVersionException : public CoreException {
    private:
        int databaseVersion;
    public:
        inline UnsupportedDatabaseVersionException(int databaseVersion) { this->databaseVersion = databaseVersion; }
        inline int getDatabaseVersion() const { return databaseVersion; }
    };
}

#endif
