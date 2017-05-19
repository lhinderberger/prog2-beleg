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

add_executable(api src/service/main.cpp include/service/HttpRequest.h src/service/HttpRequest.cpp include/service/CgiHttpRequest.h src/service/CgiHttpRequest.cpp include/service/exceptions.h)
set_target_properties(api PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cgi-bin)
