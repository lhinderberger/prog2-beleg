#include <memory>
#include <string>

#include <gtest/gtest.h>
#include "core/domain/Software.h"
#include "core/domain/Book.h"
#include "core/exceptions.h"
#include "core/DatabaseObjectFactory.h"

using namespace std;
using namespace pb2;

using BookFactory = DatabaseObjectFactory<Book>;
using SoftwareFactory = DatabaseObjectFactory<Software>;

const string invalid_ean = "1 234567 89012-3";
const string valid_ean = "5 449000 09624-1";
const string invalid_isbn = "1-23-456789-0";
const string valid_isbn = "3-12-732320-4";

/*
 * Tests EAN validation on Media object construction
 */
TEST(MediumTest, EANValidationTest) {
    auto connection = SqlConnection::construct(":memory:", true); //TODO: Consider fixture
    auto database = Database::initialize(connection);
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
TEST(MediumTest, GeneralValidationTest) {
    auto connection = SqlConnection::construct(":memory:", true); //TODO: Consider fixture
    auto database = Database::initialize(connection);
    auto bookFactory = BookFactory(database);

    shared_ptr<Book> medium = bookFactory.construct(valid_isbn);

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
TEST(MediumTest, CopyPersistLoadTest) {
    //TODO
}