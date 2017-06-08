#include "core/Audio.h"

#include "core/Medium.priv.h" // Audio_priv is defined there as long as it's empty.

using namespace std;
using namespace pb2;

Audio::Audio(shared_ptr<Database> database, const string & ean)
        : Medium(database, ean){

}

shared_ptr<Audio> Audio::construct(shared_ptr<Database> database, const string &ean) {
    // make_shared does not see the private constructor, so we need this instead
    return shared_ptr<Audio>(new Audio(database, ean));
}

const string & Audio::getType() const {
    static string t("audio");
    return t;
}
