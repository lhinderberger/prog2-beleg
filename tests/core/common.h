#ifndef PROG2_BELEG_TESTS_COMMON_H
#define PROG2_BELEG_TESTS_COMMON_H

#include <gtest/gtest.h>

#include "core/Database.h"

/* Test fixtures */
class ConnectionFixture : public ::testing::Test {
public:
    std::shared_ptr<pb2::SqliteConnection> connection;

    ConnectionFixture();
};

class DatabaseFixture : public ConnectionFixture {
public:
    std::shared_ptr<pb2::Database> database;

    DatabaseFixture();
};

#endif