#include "core/domain/TelephoneNumber.h"
#include "core/exceptions.h"

using namespace std;
using namespace pb2;

void TelephoneNumber::fromFQTN(const string & fqtn) {
    //TODO: Validate and deserialize
    throw NotImplementedException();
}

string TelephoneNumber::toFQTN() const {
    //TODO: Write test, then serialization code
    throw NotImplementedException();
}
