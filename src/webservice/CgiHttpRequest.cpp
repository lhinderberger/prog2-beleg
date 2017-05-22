#include "webservice/CgiHttpRequest.h"

#include <fstream>

using namespace pb2;
using namespace std;

void CgiHttpRequest::construct_request_body_stream() {
    request_body_stream = make_unique<ifstream>();
    request_body_stream->rdbuf(cin.rdbuf());
}

string CgiHttpRequest::header(const string &header_name, bool *exists) {
    /* Construct name of environment variable */
    string env_var_name = string("HTTP_") + header_name;
    for (char & c: env_var_name)
        c = c == '-' ? '_' : (char)toupper(c);

    /* Retrieve environment variable (if any) and return */
    const char * env = getenv(env_var_name.c_str());
    if (exists)
        *exists = env != NULL;
    return string(env);
}

string CgiHttpRequest::path_info() {
    return string(getenv("PATH_INFO"));
}

string CgiHttpRequest::query_string() {
    return string(getenv("QUERY_STRING"));
}