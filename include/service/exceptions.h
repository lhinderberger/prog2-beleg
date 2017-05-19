#ifndef PROG2_BELEG_EXCEPTIONS_H
#define PROG2_BELEG_EXCEPTIONS_H

#include <stdexcept>

namespace pb2 {
    class NotFoundException : public std::exception {
    public:
        NotFoundException() {}
    };
}

#endif
