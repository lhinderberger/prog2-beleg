#include "core/exceptions.h"
#include "core/Medium.h"
#include "core/Medium.priv.h"

using namespace pb2;
using namespace std;

Medium::Medium(shared_ptr<Database> database, const string & ean)
        : DatabaseObject(database){
    priv = make_unique<Medium_priv>();

    //TODO: Validate
    throw NotImplementedException();

    priv->ean = ean;
}

Medium::~Medium() {}

const string & Medium::getTableName() const {
    static string t("medium");
    return t;
}


void Medium::load(SqlPreparedStatement & query,
                       const map<string, string> & alternativeColumnNames) {
    throw NotImplementedException();
}

void Medium::persist() {
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

string Medium::getSubtitle() const {
    return priv->subtitle;
}

void Medium::setSubtitle(const string &subtitle) {
    //TODO: Validate
    throw NotImplementedException();

    priv->subtitle = subtitle;
}

string Medium::getTitle() {
    return priv->title;
}

void Medium::setTitle(const string &title) {
    //TODO: Validate
    throw NotImplementedException();

    priv->title = title;
}
