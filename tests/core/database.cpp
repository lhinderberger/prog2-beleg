#include <gtest/gtest.h>

#include "common.h"

using namespace std;
using namespace pb2;

/**
 * Try to persist and reload configuration/meta values
 */
TEST_F(DatabaseFixture, DatabaseMetaTest) {
    /* Try inserting a new value */
    string key = "newkey";
    string value = "sometestvalue";
    database->setMeta(key, value);
    EXPECT_EQ(database->getMeta(key), value);

    /* Try overwriting that new value */
    string newValue = "anothertestvalue";
    database->setMeta(key, newValue);
    EXPECT_EQ(database->getMeta(key), newValue);
}

/**
 * Tests whether the example data can be successfully generated.
 */
TEST_F(DatabaseFixture, ExampleDataTest) {
    database->generateExampleData();
    connection->commit();
}
