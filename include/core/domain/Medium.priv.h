#ifndef PROG2_BELEG_MEDIUM_PRIV_H
#define PROG2_BELEG_MEDIUM_PRIV_H

#include <memory>
#include <string>
#include <vector>

#include "Author.h"
#include "core/ManyToOneLazyLoader.h"

namespace pb2 {
    class Medium_priv {
    public:
        Medium_priv(std::shared_ptr<Database> database) : author(database) {}

        std::string ean, format, subtitle, title;
        ManyToOneLazyLoader<Author,int> author;
    };

    struct Book_priv {};
    struct Audio_priv {};
    struct Video_priv {};
    struct Software_priv {};

    /**
     * Builds a well-formatted EAN string from a set of EAN digits.
     */
    std::string buildEANString(const std::vector<int> & eanDigits);

    /**
     * Calculates the checksum of the EAN given
     * @param ean The first 12 digits of the EAN number in question.
     * @return The integer checksum of the EAN given
     */
    int calculateEANChecksum(const std::vector<int> & eanDigits);

    /**
     * Extracts all digits from a string, ignores spaces and dashes, otherwise fails on
     * non-digit input.
     */
    std::vector<int> extractEANDigits(const std::string & eanString);

    /**
     * Validates an EAN number by checking the number of digits and comparing checksum.
     */
    void validateEAN(const std::vector<int> & eanDigits);
}

#endif
