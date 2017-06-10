#include <memory>
#include <string>

#include <gtest/gtest.h>
#include "core/Author.h"
#include "core/Database.h"
#include "core/DatabaseObjectFactory.h"
#include "core/SqlConnection.h"
#include "core/SqlPreparedStatement.h"

using namespace std;
using namespace pb2;

using AuthorFactory = pb2::DatabaseObjectFactory<Author>;

/*
 * Learning test to find out which error codes SQLite emits on common SQL failures.
 */
TEST(SQLAbstractionTest, SQLFailureCodesTest) {
    auto connection = SqlConnection::construct(":memory:", true);
    try {
        SqlPreparedStatement query(connection, "SELECT * FROM tblDoesntexist");
    }
    catch (SqliteException & e) {
        EXPECT_EQ(SQLITE_ERROR, e.getSqliteErrorCode());
    }
}

/**
 * Find out if full column names are derived correctly
 */
TEST(SQLAbstractionTest, FullColumnNamesTest) {
    auto connection = SqlConnection::construct(":memory:", true); //TODO: Move to fixture
    connection->executeSQL("CREATE TABLE test(id INT PRIMARY KEY, value TEXT); INSERT INTO test VALUES(1,'bla'),(2,'blubb');");

    SqlPreparedStatement query(connection, "SELECT id, value AS valueAlias, COUNT(*), COUNT(*) AS bla FROM test as testi");

    EXPECT_EQ(4, query.getColumnCount());
    EXPECT_EQ("test.id", query.getFullColumnName(0));
    EXPECT_EQ("test.value", query.getFullColumnName(1));
    EXPECT_EQ(".COUNT(*)", query.getFullColumnName(2));
    EXPECT_EQ(".bla", query.getFullColumnName(3));
}

/**
 * Basic test to find out if SQL persist / load works as intended.
 */
TEST(SQLAbstractionTest, PersistAuthorTest) {
    auto connection = SqlConnection::construct(":memory:", true); //TODO: Move to fixture
    auto database = Database::initialize(connection);
    auto authorFactory = AuthorFactory(database);

    /* Create and persist new author */
    auto author = authorFactory.construct(123);
    author->setFirstName("Lucas");
    author->setLastName("Hinderberger");
    author->persist();
    connection->commit();

    /* Query for author and ensure they're equal */
    SqlPreparedStatement query(connection, string("SELECT * FROM ") + Author::tableName + " WHERE id = ?");
    query.bindInt(1, 123);
    query.step();

    auto author2 = authorFactory.load(query);

    EXPECT_EQ(123, author2->getId());
    EXPECT_EQ("Lucas", author2->getFirstName());
    EXPECT_EQ("Hinderberger", author2->getLastName());
}

/**
 * Basic test to ensure that rollback works as intended
 */
TEST(SQLAbstractionTest, RollbackTest) {
    //TODO
}
