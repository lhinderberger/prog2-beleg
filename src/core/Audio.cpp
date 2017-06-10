#include "core/Audio.h"

#include "core/Medium.priv.h" // Audio_priv is defined there as long as it's empty.

using namespace std;
using namespace pb2;

Audio::Audio(shared_ptr<Database> database, const string & ean)
        : Medium(database, ean){

}

Audio::Audio(shared_ptr<Database> database, SqlPreparedStatement & query,
             const map<string, int> & columnIndexes
) : Medium(database, query, columnIndexes){

}

Audio::~Audio() = default;

const string & Audio::getType() const {
    static string t("audio");
    return t;
}
