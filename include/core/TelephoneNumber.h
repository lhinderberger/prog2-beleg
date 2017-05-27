#ifndef PROG2_BELEG_TELEPHONE_NUMBER_H
#define PROG2_BELEG_TELEPHONE_NUMBER_H

#include <string>

namespace pb2 {
    class TelephoneNumber {
    public:
        std::string countryCode, areaCode, phoneNumber;

        /**
         * Deserializes a FQTN (fully qualified telephone number) according to E.123
         */
        void fromFQTN(const std::string & fqtn);

        /**
         * Serializes the TelephoneNumber into a fully qualified telephone number
         * according to the international format of the E.123 standard.
         */
        std::string toFQTN() const;
    };
}

#endif
