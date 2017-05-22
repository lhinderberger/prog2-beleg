#ifndef PROG2_BELEG_CORE_EXCEPTIONS_H
#define PROG2_BELEG_CORE_EXCEPTIONS_H

#include <stdexcept>

namespace pb2 {
    class Exception : public std::exception {};
    class CoreException : public Exception {};

    class NotImplementedException : public CoreException {
    public:
        NotImplementedException() {}
    };
}

#endif
