#ifndef PROG2_BELEG_WEBSERVICE_EXCEPTIONS_H
#define PROG2_BELEG_WEBSERVICE_EXCEPTIONS_H

#include "../core/exceptions.h"

namespace pb2 {
    class WebserviceException : public Exception {};

    class NotFoundException : public WebserviceException {
    public:
        NotFoundException() {}
    };
}

#endif
