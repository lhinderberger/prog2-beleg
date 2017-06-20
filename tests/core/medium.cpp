#include <memory>
#include <string>

#include <gtest/gtest.h>
#include "core/domain/Author.h"
#include "core/domain/MediumCopy.h"
#include "core/domain/Software.h"
#include "core/domain/Book.h"
#include "core/exceptions.h"
#include "core/DatabaseObjectFactory.h"

#include "common.h"

using namespace std;
using namespace pb2;

using AuthorFactory = DatabaseObjectFactory<Author>;
using BookFactory = DatabaseObjectFactory<Book>;
using SoftwareFactory = DatabaseObjectFactory<Software>;
using MediumCopyFactory = DatabaseObjectFactory<MediumCopy>;

/*
 * Tests EAN validation on Media object construction
 */
TEST_F(DatabaseFixture, MediumEANValidationTest) {
    auto bookFactory = BookFactory(database);
    auto softwareFactory = SoftwareFactory(database);

    /* Try to pass an invalid EAN */
    //TODO: Check many types of invalid EANs?
    EXPECT_THROW(softwareFactory.construct(invalid_ean), ValidationException);

    /* Try to pass a valid EAN */
    EXPECT_NO_THROW(softwareFactory.construct(valid_ean));

    /* Try to pass an invalid ISBN */
    //TODO: Check many types on invalid ISBNs?
    EXPECT_THROW(bookFactory.construct(invalid_isbn), ValidationException);

    /* Try to pass a valid ISBN */
    EXPECT_NO_THROW(bookFactory.construct(valid_isbn));
}

/*
 * Tests general input validation for Media objects
 */
TEST_F(DatabaseFixture, MediumGeneralValidationTest) {
    auto bookFactory = BookFactory(database);

    auto medium = bookFactory.construct(valid_isbn);

    /* Ensure no invalid formats can be set */
    EXPECT_THROW(medium->setFormat("some-invalid-format"), ValidationException);

    /* Ensure all allowed formats can be set */
    EXPECT_NO_THROW({
        for (const std::string &format : Medium::allowedFormats())
            medium->setFormat(format);
    });
}

/*
 * Tests whether MediumCopy entities are correctly persisted and restored.
 */
TEST_F(DatabaseFixture, MediumCopyPersistLoadTest) {
    auto authorFactory = AuthorFactory(database);
    auto mediumCopyFactory = MediumCopyFactory(database);
    auto softwareFactory = SoftwareFactory(database);

    /* Create a fully-featured medium */
    auto author = authorFactory.construct(0);
    author->setFirstName("Lucas");
    author->setLastName("Hinderberger");
    author->persist();

    auto medium = softwareFactory.construct(valid_ean);
    string validFormat = *(Medium::allowedFormats().cbegin());
    medium->setAuthor(author);
    medium->setFormat(validFormat);
    medium->setTitle("prog2-beleg");
    medium->setSubtitle("Oder: Wie ich aus Versehen eine ORM-Lösung gebastelt habe");
    medium->persist();

    auto copy1 = mediumCopyFactory.construct(medium, -1);
    copy1->setLocation("Regal 1 Fach 2");
    copy1->persist();

    auto copy2 = mediumCopyFactory.construct(medium, -1);
    copy2->setLocation("Regal 2 Fach 3");
    copy2->setDeaccessioned(true);
    copy2->persist();

    connection->commit();

    /* Query for medium */
    SqlitePreparedStatement query(connection, "SELECT * FROM " + Medium::tableName + " WHERE ean = ?");
    query.bind(1, medium->getEAN());
    query.step();

    auto medium2 = softwareFactory.load(query);

    EXPECT_EQ(medium->getEAN(), medium2->getEAN());
    EXPECT_EQ(medium->getAuthor()->getId(), medium2->getAuthor()->getId());
    EXPECT_EQ(medium->getFormat(), medium2->getFormat());
    EXPECT_EQ(medium->getTitle(), medium2->getTitle());
    EXPECT_EQ(medium->getSubtitle(), medium2->getSubtitle());

    auto copies = medium2->queryCopies();
    EXPECT_EQ(2, copies.size());
    bool copy1compared = false;
    bool copy2compared = false;
    for (const auto & queried : copies) {
        EXPECT_EQ(medium->getEAN(), queried->getMedium()->getEAN());

        shared_ptr<MediumCopy> actual = copy1;
        if (queried->getSerialNumber() == 1)
            copy1compared = true;
        else if (queried->getSerialNumber() == 2) {
            actual = copy2;
            copy2compared = true;
        }

        EXPECT_EQ(actual->getLocation(), queried->getLocation());
        EXPECT_EQ(actual->getDeaccessioned(), queried->getDeaccessioned());
    }

    EXPECT_TRUE(copy1compared && copy2compared);
}