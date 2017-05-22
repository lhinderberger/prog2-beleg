#include "core/Video.h"

#include "core/Medium.priv.h" // Video_priv is defined there as long as it's empty.

using namespace std;
using namespace pb2;

Video::Video(const string & ean)
        : Medium(ean){

}

string Video::getType() const {
    return "video";
}
