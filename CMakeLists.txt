# Configure CMake
cmake_minimum_required(VERSION 3.3)
project(prog2-beleg)

# Configure compiler
add_definitions(-Wall)
set (CMAKE_CXX_STANDARD 14)

# Set global include directory
include_directories(include/)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(domain
        include/core/exceptions.h
        include/core/Medium.h
        include/core/Medium.priv.h
        src/core/Medium.cpp
        include/core/Book.h
        src/core/Book.cpp
        include/core/Video.h
        include/core/Audio.h
        include/core/Software.h
        include/core/Author.h
        include/core/Author.priv.h
        src/core/Author.cpp
        src/core/Software.cpp
        src/core/Video.cpp
        src/core/Audio.cpp
        )

add_executable(api
        src/webservice/main.cpp
        include/webservice/HttpRequest.h
        src/webservice/HttpRequest.cpp
        include/webservice/CgiHttpRequest.h
        src/webservice/CgiHttpRequest.cpp
        include/webservice/exceptions.h)

set_target_properties(api PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cgi-bin)
