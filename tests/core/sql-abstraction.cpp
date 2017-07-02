#include <memory>
#include <string>

#include <gtest/gtest.h>
#include "core/domain/Author.h"
#include "core/Database.h"
#include "core/DatabaseObjectFactory.h"
#include "core/sqlite/SqliteConnection.h"
#include "core/sqlite/SqlitePreparedStatement.h"

#include "common.h"

using namespace std;
using namespace pb2;

using AuthorFactory = pb2::DatabaseObjectFactory<Author>;

/*
 * Learning test to find out which error codes SQLite emits on common SQL failures.
 */
TEST_F(ConnectionFixture, SQLFailureCodesTest) {
    try {
        SqlitePreparedStatement query(connection, "SELECT * FROM tblDoesntexist");
    }
    catch (SqliteException & e) {
        EXPECT_EQ(SQLITE_ERROR, e.getSqliteErrorCode());
    }
}

/**
 * Find out if full column names are derived correctly
 */
TEST_F(ConnectionFixture, FullColumnNamesTest) {
    connection->executeSQL("CREATE TABLE test(id INT PRIMARY KEY, value TEXT); INSERT INTO test VALUES(1,'bla'),(2,'blubb');");

    SqlitePreparedStatement query(connection, "SELECT id, value AS valueAlias, COUNT(*), COUNT(*) AS bla FROM test as testi");

    EXPECT_EQ(4, query.getColumnCount());
    EXPECT_EQ("test.id", query.getFullColumnName(0));
    EXPECT_EQ("test.value", query.getFullColumnName(1));
    EXPECT_EQ(".COUNT(*)", query.getFullColumnName(2));
    EXPECT_EQ(".bla", query.getFullColumnName(3));
}

/**
 * Basic test to find out if SQL persist / load works as intended.
 */
TEST_F(DatabaseFixture, PersistAuthorTest) {
    auto authorFactory = AuthorFactory(database);

    /* Create and persist new author */
    auto author = authorFactory.construct(123);
    author->setFirstName("Lucas");
    author->setLastName("Hinderberger");
    author->persist();
    connection->commit();

    /* Query for author and ensure they're equal */
    SqlitePreparedStatement query(connection, string("SELECT * FROM ") + Author::tableName + " WHERE id = ?");
    query.bind(1, 123);
    query.step();

    auto author2 = authorFactory.load(query);

    EXPECT_EQ(123, author2->getId());
    EXPECT_EQ("Lucas", author2->getFirstName());
    EXPECT_EQ("Hinderberger", author2->getLastName());
}
