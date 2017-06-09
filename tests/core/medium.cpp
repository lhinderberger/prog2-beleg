#include <memory>
#include <string>

#include <gtest/gtest.h>
#include "core/Software.h"
#include "core/Book.h"
#include "core/exceptions.h"

using namespace std;
using namespace pb2;

const string invalid_ean = "some-invalid-ean";
const string valid_ean = "5 449000 09624-1";
const string invalid_isbn = "some-invalid-isbn";
const string valid_isbn = "3-12-732320-4";

/*
 * Tests EAN validation on Media object construction
 */
TEST(MediumTest, EANValidationTest) {
    auto connection = SqlConnection::construct(":memory:", true); //TODO: Consider fixture
    auto database = Database::initialize(connection);

    /* Try to pass an invalid EAN */
    //TODO: Check many types of invalid EANs?
    EXPECT_THROW(Software::construct(database, invalid_ean), ValidationException);

    /* Try to pass a valid EAN */
    EXPECT_NO_THROW(Software::construct(database, valid_ean));

    /* Try to pass an invalid ISBN */
    //TODO: Check many types on invalid ISBNs?
    EXPECT_THROW(Book::construct(database, invalid_isbn), ValidationException);

    /* Try to pass a valid ISBN */
    EXPECT_NO_THROW(Book::construct(database, valid_isbn));
}

/*
 * Tests general input validation for Media objects
 */
TEST(MediumTest, GeneralValidationTest) {
    auto connection = SqlConnection::construct(":memory:", true); //TODO: Consider fixture
    auto database = Database::initialize(connection);

    shared_ptr<Book> medium = Book::construct(database, valid_isbn);

    /* Ensure no invalid formats can be set */
    EXPECT_THROW(medium->setFormat("some-invalid-format"), ValidationException);

    /* Ensure all allowed formats can be set */
    EXPECT_NO_THROW({
        for (const std::string &format : Medium::allowedFormats())
            medium->setFormat(format);
    });

    /* Overly long input strings are caught by SQLite on insert, so here's
     * just a learning test */
    //TODO
}