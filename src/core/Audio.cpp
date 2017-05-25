#include "core/Audio.h"

#include "core/Medium.priv.h" // Audio_priv is defined there as long as it's empty.

using namespace std;
using namespace pb2;

Audio::Audio(const string & ean)
        : Medium(ean){

}

shared_ptr<Audio> Audio::construct(const string &ean) {
    // make_shared does not see the private constructor, so we need this instead
    return shared_ptr<Audio>(new Audio(ean));
}

string Audio::getType() const {
    return "audio";
}
