#include "webservice/HttpRequest.h"

#include <cassert>

using namespace pb2;
using namespace std;

std::istream& HttpRequest::request_body() {
    /* Lazy construction of input stream */
    if (!request_body_stream)
        construct_request_body_stream();

    /* Returning stream to user */
    assert(request_body_stream);
    return *request_body_stream;
}