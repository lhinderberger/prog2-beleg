#include "core/Database.h"
#include "core/Database.priv.h"
#include "core/exceptions.h"
#include "core/SqlPreparedStatement.h"

using namespace std;
using namespace pb2;

Database::Database(shared_ptr<SqlConnection> connection) {
    if (!connection)
        throw NullPointerException();

    priv = make_unique<Database_priv>();
    priv->connection = connection;
}

shared_ptr<Database> Database::initialize(shared_ptr<SqlConnection> connection) {
    /* Run initializing SQL */
    connection->executeSQL(initializingSQL);

    /* Open and return Database object */
    return open(connection);
}

shared_ptr<Database> Database::migrate(shared_ptr<SqlConnection> connection) {
    /* Version check and migration */
    int version = getFormatVersion(connection);
    if (version < getCurrentFormatVersion()) {
        /* We already are on the lowest format version, so this code path should
         * never be entered. */
        throw logic_error("Error: getFormatVersion() returned lower than possible version");
    }
    else if (version > getCurrentFormatVersion())
        throw DatabaseVersionException(version);

    /* Open and return Database object */
    return open(connection);
}

shared_ptr<Database> Database::open(shared_ptr<SqlConnection> connection) {
    /* Version check */
    int version = getFormatVersion(connection);
    if (version != getCurrentFormatVersion())
        throw DatabaseVersionException(version);

    /* Open and return Database object */
    return shared_ptr<Database>(new Database(connection));
}

int Database::getCurrentFormatVersion() {
    return 1;
}

int Database::getFormatVersion(shared_ptr<SqlConnection> connection) {
    /* Create query to retrieve version field */
    SqlPreparedStatement query(connection, "SELECT value FROM meta WHERE name = 'version'");

    /* Retrieve and return version */
    if (!query.step())
        throw DatabaseFormatException("Database does not contain a version field! "
                                      "Maybe it is not a prog2-beleg database?");
    return stoi(query.columnString(0));
}

shared_ptr<SqlConnection> Database::getConnection() const {
    return priv->connection;
}
