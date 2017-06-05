#include <memory>
#include <iostream>

#include <gtest/gtest.h>
#include "core/SqlConnection.h"
#include "core/SqlPreparedStatement.h"

using namespace pb2;

/*
 * Learning test to find out which error codes SQLite emits on common SQL failures.
 */
TEST(SQLiteLearningTest, SQLFailureCodesTest) {
    auto connection = SqlConnection::construct(":memory:", true);
    try {
        SqlPreparedStatement query(connection, "SELECT * FROM tblDoesntexist");
    }
    catch (SqliteException & e) {
        EXPECT_EQ(e.getSqliteErrorCode(), SQLITE_ERROR);
    }
}
