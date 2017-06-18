#include <gtest/gtest.h>

#include "core/Database.h"

using namespace std;
using namespace pb2;

/**
 * Try to persist and reload configuration/meta values
 */
TEST(DatabaseTest, MetaTest) {
    auto connection = SqliteConnection::construct(":memory:", true); //TODO: Move to fixture?
    auto database = Database::initialize(connection);

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
