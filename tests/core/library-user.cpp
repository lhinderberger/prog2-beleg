#include <gtest/gtest.h>

#include "common.h"

using namespace pb2;
using namespace std;

TEST_F(DatabaseFixture, LibraryUserPersistLoadTest) {
    /* Create user WITHOUT address */
    auto userFactory = DatabaseObjectFactory<LibraryUser>(database);
    auto user = userFactory.construct(-1);
    user->setFirstName("Lucas");
    user->setLastName("Hinderberger");
    user->setTelephone("01234-56789");

    /* Saving a user without a postal address should fail */
    EXPECT_THROW(user->persist(), NullPointerException);

    /* Create a postal address */
    auto postalAddressFactory = DatabaseObjectFactory<PostalAddress>(database);
    auto address = postalAddressFactory.construct(-1);
    address->setStreet("Teststraße");
    address->setHouseNumber("1a");
    address->setZip("01187");
    address->setCity("Dresden");
    address->persist();
    EXPECT_NE(address->getId(), 0); // make sure ID auto generation has worked
    EXPECT_NE(address->getId(), -1);

    /* Try to save user WITH address */
    user->setPostalAddress(address);
    user->persist();

    /* Query user */
    SqlitePreparedStatement query(connection, string("SELECT * FROM ") + LibraryUser::tableName + " WHERE id = ?");
    query.bind(1, user->getId());
    query.step();
    auto user2 = userFactory.load(query);

    /* Verify */
    EXPECT_EQ(user2->getFirstName(), user->getFirstName());
    EXPECT_EQ(user2->getLastName(), user->getLastName());
    EXPECT_EQ(user2->getTelephone(), user->getTelephone());
    EXPECT_EQ(user2->getPostalAddress()->getId(), user->getPostalAddress()->getId());
    EXPECT_EQ(user2->getPostalAddress()->getStreet(), user->getPostalAddress()->getStreet());
    EXPECT_EQ(user2->getPostalAddress()->getHouseNumber(), user->getPostalAddress()->getHouseNumber());
    EXPECT_EQ(user2->getPostalAddress()->getAddition(), user->getPostalAddress()->getAddition());
    EXPECT_EQ(user2->getPostalAddress()->getZip(), user->getPostalAddress()->getZip());
    EXPECT_EQ(user2->getPostalAddress()->getCity(), user->getPostalAddress()->getCity());
}