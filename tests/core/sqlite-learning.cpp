#include <memory>

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
        EXPECT_EQ(SQLITE_ERROR, e.getSqliteErrorCode());
    }
}

TEST(SQLiteLearningTest, FullColumnNamesTest) {
    auto connection = SqlConnection::construct(":memory:", true); //TODO: Move to fixture
    connection->executeSQL("CREATE TABLE test(id INT PRIMARY KEY, value TEXT); INSERT INTO test VALUES(1,'bla'),(2,'blubb');");

    SqlPreparedStatement query(connection, "SELECT id, value AS valueAlias, COUNT(*), COUNT(*) AS bla FROM test as testi");

    EXPECT_EQ(4, query.getColumnCount());
    EXPECT_EQ("test.id", query.getFullColumnName(0));
    EXPECT_EQ("test.value", query.getFullColumnName(1));
    EXPECT_EQ(".COUNT(*)", query.getFullColumnName(2));
    EXPECT_EQ(".bla", query.getFullColumnName(3));
}
