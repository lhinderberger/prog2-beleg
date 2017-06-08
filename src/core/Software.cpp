#include "core/Software.h"

#include "core/Medium.priv.h" // Software_priv is defined there as long as it's empty.

using namespace std;
using namespace pb2;

Software::Software(shared_ptr<Database> database, const string & ean)
        : Medium(database, ean){

}

shared_ptr<Software> Software::construct(shared_ptr<Database> database, const string &ean) {
    return shared_ptr<Software>(new Software(database, ean));
}

const string & Software::getType() const {
    static string t("software");
    return t;
}
