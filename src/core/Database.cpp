#include "core/Database.h"
#include "core/Database.priv.h"
#include "core/exceptions.h"
#include "core/sqlite/SqlitePreparedStatement.h"

#include "core/domain/LibraryUser.h"

using namespace std;
using namespace pb2;

Database::Database(shared_ptr<SqliteConnection> connection) {
    if (!connection)
        throw NullPointerException();

    /* Initialize */
    priv = make_unique<Database_priv>();
    priv->connection = connection;

    /* Load meta values */
    SqlitePreparedStatement query(connection, "SELECT name, value FROM meta");
    while (query.step())
        priv->meta[query.columnString(0)] = query.columnString(1);
}

Database::~Database() = default;


shared_ptr<Database> Database::initialize(shared_ptr<SqliteConnection> connection) {
    /* Run initializing SQL */
    connection->executeSQL(initializingSQL);
    connection->commit();

    /* Open and return Database object */
    return open(connection);
}

shared_ptr<Database> Database::migrate(shared_ptr<SqliteConnection> connection) {
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

shared_ptr<Database> Database::open(shared_ptr<SqliteConnection> connection) {
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

int Database::getFormatVersion(shared_ptr<SqliteConnection> connection) {
    /* Create query to retrieve version field */
    SqlitePreparedStatement query(connection, "SELECT value FROM meta WHERE name = 'version'");

    /* Retrieve and return version */
    if (!query.step())
        throw DatabaseFormatException("Database does not contain a version field! "
                                      "Maybe it is not a prog2-beleg database?");
    return stoi(query.columnString(0));
}

shared_ptr<SqliteConnection> Database::getConnection() const {
    return priv->connection;
}

const map<string,string>& Database::getMeta() const {
    return priv->meta;
}

string Database::getMeta(const string & name) const {
    return priv->meta.at(name);
}

void Database::setMeta(const string & name, const string & value) {
    priv->connection->rollback();

    auto existingIt = priv->meta.find(name);
    bool insert = existingIt == priv->meta.end();

    /* Insert into database */
    SqlitePreparedStatement statement(
        priv->connection,
        insert ? "INSERT INTO meta(value, name) VALUES(?,?)" : "UPDATE meta SET value=? WHERE name=?"
    );
    statement.bind(1, value);
    statement.bind(2, name);
    statement.step();

    priv->connection->commit();

    /* Insert into cache */
    if (insert)
        priv->meta.insert({name, value});
    else
        existingIt->second = value;
}

void Database::generateExampleData() {
    priv->connection->executeSQL(exampleDataSQL);
}
