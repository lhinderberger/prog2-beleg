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
file(GLOB_RECURSE CORE_INCLUDE_FILES "include/core/*")
file(GLOB_RECURSE CORE_SRC_FILES "src/core/*")
file(GLOB_RECURSE CORE_TEST_FILES "tests/core/*")
file(GLOB_RECURSE GUI_INCLUDE_FILES "include/gui/*")
file(GLOB_RECURSE GUI_SRC_FILES "src/gui/*")

# Define core target
add_library(core ${CORE_INCLUDE_FILES} ${CORE_SRC_FILES})
target_link_libraries(core sqlite3)

# Define GUI target
find_package(wxWidgets COMPONENTS core base adv)
if (wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
    add_executable(gui ${GUI_INCLUDE_FILES} ${GUI_SRC_FILES})
    target_link_libraries(gui ${wxWidgets_LIBRARIES} core)
endif()

# Define tests
add_executable(test-core ${CORE_TEST_FILES})
add_test(test-core test-core)
target_link_libraries(test-core gtest_main gtest pthread core)
