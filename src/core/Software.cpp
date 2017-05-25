#include "core/Software.h"

#include "core/Medium.priv.h" // Software_priv is defined there as long as it's empty.

using namespace std;
using namespace pb2;

Software::Software(const string & ean)
        : Medium(ean){

}

shared_ptr<Software> Software::construct(const string &ean) {
    return shared_ptr<Software>(new Software(ean));
}

string Software::getType() const {
    return "software";
}
