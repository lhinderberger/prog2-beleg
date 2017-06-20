#include "common.h"
#include "core/domain/Book.h"

using namespace pb2;

ConnectionFixture::ConnectionFixture() {
    connection = pb2::SqliteConnection::construct(":memory:", true);
}

DatabaseFixture::DatabaseFixture() {
    database = pb2::Database::initialize(connection);
}

LendingFixture::LendingFixture() {
    auto lendingFactory = DatabaseObjectFactory<Lending>(database);
    auto bookFactory = DatabaseObjectFactory<Book>(database);
    auto mediumCopyFactory = DatabaseObjectFactory<MediumCopy>(database);
    auto libraryUserFactory = DatabaseObjectFactory<LibraryUser>(database);
    auto postalAddressFactory = DatabaseObjectFactory<PostalAddress>(database);

    /* Create a medium incl. copy */
    auto medium = bookFactory.construct(valid_ean);
    medium->setTitle("Test Book");
    medium->persist();

    auto mediumCopy = mediumCopyFactory.construct(medium, -1);
    mediumCopy->persist();

    /* Create a default library user */
    auto user = libraryUserFactory.construct(-1);
    user->setFirstName("Lucas");
    user->setLastName("Hinderberger");

    auto address = postalAddressFactory.construct(-1);
    address->setStreet("Teststraße");
    address->setHouseNumber("1a");
    address->setZip("01187");
    address->setCity("Dresden");
    address->persist();

    user->setPostalAddress(address);
    user->persist();

    /* Create a lending */
    lending = lendingFactory.construct(mediumCopy, user, someTimestamp);
}