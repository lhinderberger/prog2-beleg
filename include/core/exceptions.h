#ifndef PROG2_BELEG_CORE_EXCEPTIONS_H
#define PROG2_BELEG_CORE_EXCEPTIONS_H

#include <stdexcept>
#include <sqlite3.h>

namespace pb2 {
    class Exception : public std::exception {};
    class CoreException : public Exception {};

    //TODO: Find a better hierarchy for exceptions
    //TODO: Too much repeated code here - refactor before finishing the exam
    //TODO: Either inline all exceptions or none

    class NotImplementedException : public CoreException {
    public:
        NotImplementedException() {}
    };

    class LendingNotReturnedException : public CoreException {
    public:
        LendingNotReturnedException() {}
    };

    class NullPointerException : public CoreException {
    public:
        NullPointerException() {}
    };

    class StringTooLongException : public CoreException {
    public:
        StringTooLongException() {}
    };

    class ValidationException : public CoreException {
    private:
        std::string description;

    public:
        ValidationException(const std::string & description) { this->description = description; }

        inline virtual const char * what() const noexcept override { return description.c_str(); }
    };

    class DatabaseIntegrityException : public CoreException {
    private:
        std::string description;

    public:
        DatabaseIntegrityException(const std::string & description) { this->description = description; }

        inline virtual const char * what() const noexcept override { return description.c_str(); }
    };

    class FileExistsException : public CoreException {
    private:
        std::string filename;
    public:
        FileExistsException(const std::string & filename);

        inline const std::string & getFilename() const { return filename; }
    };

    class ColumnNotFoundException : public CoreException {
    private:
        std::string fullColumnName;
    public:
        ColumnNotFoundException(const std::string & fullColumnName);

        inline const std::string & getFullColumnName() const { return fullColumnName; }
    };

    class DatabaseFormatException : public CoreException {
    private:
        std::string description;

    public:
        DatabaseFormatException(const std::string & description) { this->description = description; }

        inline virtual const char * what() const noexcept override { return description.c_str(); }
    };

    class DatabaseVersionException : public CoreException {
    private:
        int databaseVersion;
    public:
        inline DatabaseVersionException(int databaseVersion) { this->databaseVersion = databaseVersion; }
        inline int getDatabaseVersion() const { return databaseVersion; }
    };

    class SqliteException : public CoreException {
    private:
        int sqliteErrorCode;
        std::string sqliteErrorString, whatString;

        void buildWhatString();

    public:
        SqliteException(int errorCode);
        SqliteException(sqlite3 * db);

        virtual const char * what() const noexcept override;

        inline int getSqliteErrorCode() const { return sqliteErrorCode; }
        inline std::string getSqliteErrorString() const { return sqliteErrorString; }
    };
}

#endif
