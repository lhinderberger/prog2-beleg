# Configure CMake
cmake_minimum_required(VERSION 3.3)
project(prog2-beleg)

# Configure compiler
add_definitions(-Wall)
set (CMAKE_CXX_STANDARD 14)

# Set global include directory
include_directories(include/)

# Set up file paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Collect files
file(GLOB CORE_INCLUDE_FILES "include/core/*")
file(GLOB CORE_SRC_FILES "src/core/*")
file(GLOB CORE_TEST_FILES "tests/core/*")
file(GLOB WEBSERVICE_INCLUDE_FILES "include/webservice/*")
file(GLOB WEBSERVICE_SRC_FILES "src/webservice/*")
file(GLOB WEBSERVICE_TEST_FILES "tests/webservice/*")

# Define core / webservice targets
add_library(core ${CORE_INCLUDE_FILES} ${CORE_SRC_FILES})
add_executable(webservice ${WEBSERVICE_INCLUDE_FILES} ${WEBSERVICE_SRC_FILES})

target_link_libraries(webservice core)
set_target_properties(webservice PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cgi-bin)

# Define tests
add_executable(test-core ${CORE_TEST_FILES})
add_test(test-core test-core)
target_link_libraries(test-core gtest_main gtest pthread)

add_executable(test-webservice ${WEBSERVICE_TEST_FILES})
add_test(test-webservice test-webservice)
target_link_libraries(test-webservice gtest_main gtest pthread)
