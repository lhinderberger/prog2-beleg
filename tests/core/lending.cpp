#include <gtest/gtest.h>

#include "common.h"

using namespace std;
using namespace pb2;

/*
 * Test date calculation assumptions for Lending objects
 */
TEST_F(LendingFixture, LendingDateTest) {
    /* Check days left before any extension */
    EXPECT_EQ(lending->getDaysLeft(), atoi(database->getMeta("default_lending_runtime").c_str()));
}

/*
 * Tests general input validation for Lending objects
 */
TEST_F(DatabaseFixture, LendingGeneralValidationTest) {
    //TODO
}
