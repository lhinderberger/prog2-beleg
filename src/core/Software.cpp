#include "core/Software.h"

#include "core/Medium.priv.h" // Software_priv is defined there as long as it's empty.

using namespace std;
using namespace pb2;

Software::Software(const string & ean)
        : Medium(ean){

}

string Software::getType() const {
    return "software";
}
