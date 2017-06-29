#include "core/exceptions.h"
#include "core/query-builder.h"
#include "core/domain/Medium.h"
#include "core/domain/Medium.priv.h"

#include "core/domain/Audio.h"
#include "core/domain/Book.h"
#include "core/domain/Software.h"
#include "core/domain/Video.h"

#include <cctype>
#include <iostream>

using namespace pb2;
using namespace std;

//Helper functions
string pb2::buildEANString(const vector<int> & eanDigits) {
    if (eanDigits.size() != 13)
        throw logic_error("EAN has not exactly 13 digits!");

    string result;
    for (int digit : eanDigits)
        result += '0' + digit;
    return result;
}

int pb2::calculateEANChecksum(const vector<int> & eanDigits) {
    if (eanDigits.size() < 12)
        throw logic_error("Partial EAN with less than 12 digits given");

    /* Calculate checksum */
    int checksum = 0;
    for (int i = 0; i <= 11; i++) {
        // This below looks wasteful, but actually it improves readability and every
        // good compiler should optimize this away

        int weight = 1 + ((i%2) * 2); // 1 for even indexes, otherwise 3
        checksum += weight * eanDigits[i]; // See Wikipedia for EAN-13 checksum calculation rules
    }
    return (10-(checksum%10)) % 10;
}

void pb2::validateEAN(const vector<int> & eanDigits) {
    /* Check length */
    if (eanDigits.size() != 13)
        throw ValidationException("The EAN does not contain exactly 13 digits");

    /* Compare checksum */
    if (calculateEANChecksum(eanDigits) != eanDigits[12])
        throw ValidationException("Invalid checksum on EAN");
}

vector<int> pb2::extractEANDigits(const string & eanString) {
    vector<int> result;
    for (char c : eanString) {
        if (isdigit(c))
            result.push_back(c - '0');
        else if (c != ' ' && c != '-')
            throw ValidationException("Invalid character in EAN!");
    }
    return result;
}


const string Medium::tableName = "media";

Medium::Medium(shared_ptr<Database> database, const string & ean)
        : Medium(database, extractEANDigits(ean)){
}

Medium::Medium(shared_ptr<Database> database, const vector<int> & eanDigits)
        : DatabaseObject(database){
    priv = make_unique<Medium_priv>(database);

    /* Validate and set EAN */
    validateEAN(eanDigits);
    priv->ean = buildEANString(eanDigits);
}

Medium::Medium(shared_ptr<Database> database, SqlitePreparedStatement & query,
               const map<string, int> & columnIndexes
) : DatabaseObject(database) {
    priv = make_unique<Medium_priv>(database);

    priv->ean = query.columnString(columnIndexes.at("ean"));
    priv->format = query.columnString(columnIndexes.at("format"));
    priv->title = query.columnString(columnIndexes.at("title"));
    priv->subtitle = query.columnString(columnIndexes.at("subtitle"));

    int authorPkIndex = columnIndexes.at("author_id");
    if (!query.columnIsNull(authorPkIndex))
        priv->author.set(query.columnInt(authorPkIndex));
}

Medium::~Medium() = default;


void Medium::del() {
    SqlitePreparedStatement query(getConnection(), "DELETE FROM " + tableName + " WHERE ean = ?");
    query.bind(1, priv->ean);
    query.step();
}

unique_ptr<AbstractDatabaseObjectFactory> Medium::polymorphicFactory(
        shared_ptr<Database> database, const string & type) {
    unique_ptr<AbstractDatabaseObjectFactory> factory = nullptr;
    if (type == "audio")
        factory.reset(new DatabaseObjectFactory<Audio>(database));
    else if (type == "book")
        factory.reset(new DatabaseObjectFactory<Book>(database));
    else if (type == "software")
        factory.reset(new DatabaseObjectFactory<Software>(database));
    else if (type == "video")
        factory.reset(new DatabaseObjectFactory<Video>(database));
    else {
        cerr << "Warning: invalid polymorphic type requested: " << type << endl;
        return nullptr;
    }

    return std::move(factory);
}

void Medium::persistImpl() {
    /* Prepare statement */
    vector<string> columns = {"format", "title", "subtitle", "author_id", "type"};
    if (!isLoaded())
        columns.push_back("ean");
    SqlitePreparedStatement statement(getConnection(), isLoaded() ?
        buildUpdateQuery<Medium>(columns, "WHERE ean=?") :
        buildInsertQuery<Medium>(columns, 1)
    );

    /* Bind parameters */
    statement.bind(1, priv->format);
    statement.bind(2, priv->title);
    statement.bind(3, priv->subtitle);
    //Param #4: See below
    statement.bind(5, getType());
    statement.bind(6, priv->ean);

    /* Bind author primary key */
    int primaryKey = 0;
    if (priv->author.isPrimaryKeySet())
        primaryKey = priv->author.getPrimaryKey();
    else if(priv->author.isLoaded())
        primaryKey = priv->author->getId();
    if (primaryKey == 0)
        statement.bind(4);
    else
        statement.bind(4, primaryKey);

    /* Execute */
    statement.step();
}


const set<string>& Medium::allowedFormats() {
    static set<string> formats {
        "hardcover", "softcover", "vinyl", "cassette", "floppy", "vhs", "cd", "dvd",
        "memory-card", "usb-stick", "download", "online-access", "other"
    };
    return formats;
}

vector<shared_ptr<MediumCopy>> Medium::queryCopies() const {
    /* Build query to get all copies of this medium */
    SqlitePreparedStatement query(getConnection(),
          string("SELECT * FROM ") + MediumCopy::tableName + " WHERE medium_ean = ?");
    query.bind(1, priv->ean);

    /* Try to retrieve at least one object and pass on control to factory on success */
    if (query.step())
        return DatabaseObjectFactory<MediumCopy>(getDatabase()).loadMany(query, -1);
    else
        return vector<shared_ptr<MediumCopy>>();
}

string Medium::getEAN() const {
    return priv->ean;
}

string Medium::getFormat() const {
    return priv->format;
}

void Medium::setFormat(const string & format) {
    if (allowedFormats().find(format) == allowedFormats().end())
        throw ValidationException(string("Invalid Medium format: ") + format);
    priv->format = format;
}

shared_ptr<Author> Medium::getAuthor() const {
    return priv->author.get();
}

void Medium::setAuthor(shared_ptr<Author> author) {
    if (!author)
        throw NullPointerException();
    priv->author.set(author);
}

string Medium::getSubtitle() const {
    return priv->subtitle;
}

void Medium::setSubtitle(const string &subtitle) {
    priv->subtitle = subtitle;
}

string Medium::getTitle() {
    return priv->title;
}

void Medium::setTitle(const string &title) {
    priv->title = title;
}
