#include "common.h"

using namespace pb2;

ConnectionFixture::ConnectionFixture() {
    connection = pb2::SqliteConnection::construct(":memory:", true);
}

DatabaseFixture::DatabaseFixture() {
    database = pb2::Database::initialize(connection);
}