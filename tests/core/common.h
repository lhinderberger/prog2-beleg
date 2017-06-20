#ifndef PROG2_BELEG_TESTS_COMMON_H
#define PROG2_BELEG_TESTS_COMMON_H

#include <gtest/gtest.h>

#include "core/Database.h"
#include "core/domain/Lending.h"

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

class LendingFixture : public DatabaseFixture {
public:
    std::shared_ptr<pb2::Lending> lending;

    LendingFixture();
};

/* Commonly used constants */
const std::string invalid_ean = "1 234567 89012-3";
const std::string valid_ean = "5 449000 09624-1";
const std::string invalid_isbn = "1-23-456789-0";
const std::string valid_isbn = "3-12-732320-4";
const time_t someTimestamp = 1479859200; // Choose a fixed timestamp to keep test result reproducible

#endif