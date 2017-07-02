#include <gtest/gtest.h>

#include "common.h"

using namespace std;
using namespace pb2;

const int secondsPerDay = 60*60*24;

void testAgainstNormalizedDueDate(shared_ptr<Lending> lending) {
    /* Fetch and normalize due date */
    tm dueDate = lending->getDueDate();
    Lending::normalizeTm(dueDate);
    time_t tsDueDateBegin = mktime(&dueDate);

    /* Perform tests */
    EXPECT_EQ(1, lending->getDaysLeft(tsDueDateBegin - 1));
    EXPECT_EQ(0, lending->getDaysLeft(tsDueDateBegin));
    EXPECT_EQ(0, lending->getDaysLeft(tsDueDateBegin + secondsPerDay - 1));
    EXPECT_EQ(-1, lending->getDaysLeft(tsDueDateBegin + secondsPerDay));
}

/*
 * Test date calculation assumptions for Lending objects
 */
TEST_F(LendingFixture, LendingDateAndStateTest) {
    int defaultRuntime = Lending::getDefaultLendingRuntime(database);
    int defaultExtension = Lending::getDefaultExtensionDays(database);

    /* Check initial state */
    EXPECT_EQ(false, lending->isReturned());
    EXPECT_EQ(0, lending->getTimesExtended());

    /* Calculate some reference points */
    time_t tsLent = lending->getTimestampLent();

    /* Check days left before any extension */
    EXPECT_EQ(defaultRuntime, lending->getDaysLeft(tsLent));
    testAgainstNormalizedDueDate(lending);

    /* Check days left after extension */
    time_t extensionPoint = tsLent + (defaultRuntime - 1) * secondsPerDay;
    lending->extend(extensionPoint, defaultExtension);

    EXPECT_EQ(false, lending->isReturned());
    EXPECT_EQ(1, lending->getTimesExtended());

    EXPECT_EQ(defaultExtension, lending->getDaysLeft(extensionPoint));
    testAgainstNormalizedDueDate(lending);

    /* Check days left after return */
    lending->returnL(extensionPoint);
    EXPECT_EQ(true, lending->isReturned());
    EXPECT_EQ(1, lending->getTimesExtended());
    EXPECT_EQ(defaultExtension, lending->getDaysLeft(extensionPoint + 9999999));
}

/*
 * Tests load / persist for Lending objects
 */
TEST_F(LendingFixture, LendingLoadPersistTest) {
    /* Persist lending */
    EXPECT_EQ(lending->getMediumCopy()->getAvailabilityHint(), string("now"));
    lending->persist();
    EXPECT_EQ(lending->getMediumCopy()->getAvailabilityHint(), lending->getDueDateISOString());

    /* Load lending from database */
    SqlitePreparedStatement query(connection, string("SELECT * FROM ") + Lending::tableName);
    query.step();
    auto lending2 = DatabaseObjectFactory<Lending>(database).load(query);

    /* Verify */
    EXPECT_EQ(lending->getTimesExtended(), lending2->getTimesExtended());
    EXPECT_EQ(lending->getLibraryUser()->getId(), lending2->getLibraryUser()->getId());
    EXPECT_EQ(lending->getMediumCopy()->getMedium()->getEAN(), lending2->getMediumCopy()->getMedium()->getEAN());
    EXPECT_EQ(lending->getMediumCopy()->getSerialNumber(), lending2->getMediumCopy()->getSerialNumber());
    EXPECT_EQ(lending->getTimestampLent(), lending2->getTimestampLent());
    EXPECT_EQ(lending->isReturned(), lending2->isReturned());

    std::tm l1dueTm = lending->getDueDate();
    std::tm l2dueTm = lending2->getDueDate();
    time_t l1due = mktime(&l1dueTm);
    time_t l2due = mktime(&l2dueTm);
    EXPECT_EQ(l1due, l2due);

    /* Extend, return and verify availability hint */
    string oldDueDate = lending->getDueDateISOString();
    lending->extend();
    EXPECT_EQ(lending->getMediumCopy()->getAvailabilityHint(), oldDueDate);
    lending->persist();
    EXPECT_EQ(lending->getMediumCopy()->getAvailabilityHint(), lending->getDueDateISOString());

    oldDueDate = lending->getDueDateISOString();
    lending->returnL();
    EXPECT_EQ(lending->getMediumCopy()->getAvailabilityHint(), oldDueDate);
    lending->persist();
    EXPECT_EQ(lending->getMediumCopy()->getAvailabilityHint(), string("now"));
}
