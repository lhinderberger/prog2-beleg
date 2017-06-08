#ifndef PROG2_BELEG_CORE_EXCEPTIONS_H
#define PROG2_BELEG_CORE_EXCEPTIONS_H

#include <stdexcept>
#include <sqlite3.h>

namespace pb2 {
    class Exception : public std::exception {};
    class CoreException : public Exception {};

    class NotImplementedException : public CoreException {
    public:
        NotImplementedException() {}
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
    public:
        ValidationException() {}
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
