#include "core/Audio.h"

#include "core/Medium.priv.h" // Audio_priv is defined there as long as it's empty.

using namespace std;
using namespace pb2;

Audio::Audio(const string & ean)
        : Medium(ean){

}

string Audio::getType() const {
    return "audio";
}
