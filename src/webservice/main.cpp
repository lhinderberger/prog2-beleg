#include <iostream>

#include "webservice/CgiHttpRequest.h"
#include "webservice/exceptions.h"

using namespace std;
using namespace pb2;

typedef void (*request_handler)(HttpRequest&);

request_handler determine_request_handler(HttpRequest & request) {
    return NULL;
}

void process_request(HttpRequest & request) {
    request_handler handler = determine_request_handler(request);
    if (!handler)
        throw NotFoundException();
}

int main() {
    try {
        /* For CGI, initializing the request object is trivial.
         * If we were using FastCGI, this would be the place to implement the FastCGI
         * event loop.
         * In any case we pass on the request to process_request();
         */
        CgiHttpRequest request;
        process_request(request);
    }
    catch(NotFoundException & e) {
        cout << "Status: 404 Not Found" << endl
             << "Content-type: text/html" << endl << endl

             << "<h1>Not Found</h1>";
        return 1;
    }
    catch(exception & e) {
        cout << "Status: 500 Internal Server Error" << endl
             << "Content-type: text/html" << endl << endl

             << "<h1>Internal Server Error</h1>";
        return 1;
    }

    return 0;
}