#include "core/exceptions.h"
#include "core/Medium.h"
#include "core/Medium.priv.h"

#include <cctype>

using namespace pb2;
using namespace std;

//Helper functions
string pb2::buildEANString(const std::vector<int> & eanDigits) {
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

Medium::Medium(shared_ptr<Database> database, const string & ean)
        : Medium(database, extractEANDigits(ean)){
}

Medium::Medium(shared_ptr<Database> database, const vector<int> & eanDigits)
        : DatabaseObject(database){
    priv = make_unique<Medium_priv>();

    /* Validate and set EAN */
    validateEAN(eanDigits);
    priv->ean = buildEANString(eanDigits);
}

Medium::~Medium() {}

const string & Medium::getTableName() const {
    static string t("media");
    return t;
}


void Medium::loadImpl(SqlPreparedStatement & query, const map<string, int> & columnIndexes) {
    throw NotImplementedException();
}

void Medium::persistImpl() {
    throw NotImplementedException();
}


const vector<string>& Medium::allowedFormats() {
    static vector<string> formats {
        "book", "vinyl", "cassette", "floppy", "vhs", "cd", "dvd", "memory-card",
        "usb-stick", "download", "online-access", "other"
    };
    return formats;
}

vector<shared_ptr<MediumCopy>> Medium::queryCopies() const {
    //TODO: Query database, return MediumCopies
    throw NotImplementedException();
}

string Medium::getEAN() const {
    return priv->ean;
}

string Medium::getFormat() const {
    return priv->format;
}

void Medium::setFormat(const string & format) {
    //TODO: Validate
    throw NotImplementedException();

    priv->format = format;
}

shared_ptr<Author> Medium::getAuthor() const {
    return priv->author;
}

void Medium::setAuthor(shared_ptr<Author> author) {
    if (!author)
        throw NullPointerException();
    priv->author = author;
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
