#include <gtest/gtest.h>

#include "common.h"

using namespace std;
using namespace pb2;

/*
 * Test date calculation assumptions for Lending objects
 */
TEST_F(LendingFixture, LendingDateAndStateTest) {
    int defaultRuntime = atoi(database->getMeta("default_lending_runtime").c_str());
    int defaultExtension = atoi(database->getMeta("default_extend_days").c_str());

    /* Check initial state */
    EXPECT_EQ(lending->isReturned(), false);
    EXPECT_EQ(lending->getTimesExtended(), 0);

    /* Check days left before any extension */
    EXPECT_EQ(lending->getDaysLeft(someTimestamp), defaultRuntime);
    EXPECT_EQ(lending->getDaysLeft(someTimestamp + 1), defaultRuntime);
    EXPECT_EQ(lending->getDaysLeft(someTimestamp + 86399), defaultRuntime);
    EXPECT_EQ(lending->getDaysLeft(someTimestamp + 86400), defaultRuntime - 1);
    EXPECT_EQ(lending->getDaysLeft(someTimestamp + 86401), defaultRuntime - 1);

    /* Check days left after extension */
    time_t extensionPoint = someTimestamp + (defaultRuntime - 1) * 86400;
    lending->extend(extensionPoint, defaultExtension);
    EXPECT_EQ(lending->isReturned(), false);
    EXPECT_EQ(lending->getTimesExtended(), 1);
    EXPECT_EQ(lending->getDaysLeft(extensionPoint), defaultExtension);
    EXPECT_EQ(lending->getDaysLeft(extensionPoint + 1), defaultExtension);
    EXPECT_EQ(lending->getDaysLeft(extensionPoint + 86399), defaultExtension);
    EXPECT_EQ(lending->getDaysLeft(extensionPoint + 86400), defaultExtension - 1);
    EXPECT_EQ(lending->getDaysLeft(extensionPoint + 86401), defaultExtension - 1);

    /* Check days left after return */
    lending->returnL(extensionPoint + 90000);
    EXPECT_EQ(lending->isReturned(), true);
    EXPECT_EQ(lending->getTimesExtended(), 1);
    EXPECT_EQ(lending->getDaysLeft(extensionPoint + 9999999), defaultRuntime + defaultExtension - 2);
}

/*
 * Tests general input validation for Lending objects
 */
TEST_F(DatabaseFixture, LendingGeneralValidationTest) {
    //TODO
}
