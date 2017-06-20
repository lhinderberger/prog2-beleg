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
    user->persist();

    /* Create a lending */
    lending = lendingFactory.construct(mediumCopy, user);
}