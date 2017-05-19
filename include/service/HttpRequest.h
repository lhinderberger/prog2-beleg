#ifndef PROG2_BELEG_HTTPREQUEST_H
#define PROG2_BELEG_HTTPREQUEST_H

#include <iostream>
#include <memory>
#include <string>

namespace pb2 {
    /**
     * General abstraction for an HTTP request
     *
     * This abstract class can be subclassed to add support for new ways of interfacing
     * with the web server - i.e. if you want to add a FastCGI implementation.
     * By default prog2-beleg comes with a CGI implementation of HttpRequest.
     *
     * For every request passed to the application, there will be one corresponding
     * instance of HttpRequest. This instance can be passed to request handlers, which
     * in turn don't need to know about the inner workings of your CGI / FastCGI
     * implementation, separating technical details from business logic.
     */
    class HttpRequest {
    public:
        /**
         * Returns the equivalent to the CGI PATH_INFO environment variable.
         *
         * This means it will return the part of the requested URL between the cgi
         * executable's path and the query string.
         *
         * When "https://example.com/cgi-bin/api" is the path of the cgi executable,
         * for a user requesting "https://example.com/cgi-bin/api/test?param=value"
         * "/test" would be the PATH_INFO returned.
         */
        virtual std::string path_info() = 0;

        /**
         * Returns the equivalent to the CGI QUERY_STRING environment variable.
         *
         * The query string contains all GET parameters and refers to the part
         * of the URI preceded by a question mark (excluding the question mark itself).
         */
        virtual std::string query_string() = 0;

        /**
         * Tries to retrieve an HTTP header.
         *
         * If the given header does not exist, an empty string will be returned and
         * the optional "exists" output variable will be set to false.
         *
         * @param header_name The name of the HTTP header to retrieve i.e. "Accept"
         * @param exists Pass in a pointer to a bool variable if you want to know whether
         * retrieving the header was successful - otherwise pass NULL.
         * @return Contents of the requested HTTP header, empty string if not found.
         */
        virtual std::string header(const std::string & header_name, bool * exists) = 0;

        /**
         * Retrieve the input stream for the HTTP request body.
         *
         * This function returns the equivalent of stdin for CGI, meaning the request
         * body (without the header strings).
         *
         * Attention: The stream returned by this function will always point to the
         * same stream instance! You will not get a fresh, rewound stream each time
         * you call this function.
         */
        std::istream & request_body();

    protected:
        std::unique_ptr<std::istream> request_body_stream;

        /**
         * Creates a new std::ifstream for the request body
         *
         * This function initializes @var request_body_stream and is called automatically
         * be HttpRequest on first call to request_body()
         */
        virtual void construct_request_body_stream() = 0;
    };
}

#endif
