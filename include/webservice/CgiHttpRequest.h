#ifndef PROG2_BELEG_CGIHTTPREQUEST_H
#define PROG2_BELEG_CGIHTTPREQUEST_H

#include "HttpRequest.h"

namespace pb2 {
    class CgiHttpRequest : public HttpRequest {
    public:
        virtual std::string path_info();
        virtual std::string query_string();
        virtual std::string header(const std::string & header_name, bool * exists);

    protected:
        virtual void construct_request_body_stream();
    };
}

#endif
