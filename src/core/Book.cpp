#include "core/Book.h"
#include "core/exceptions.h"

#include "core/Medium.priv.h" // Book_priv is defined there as long as it's empty.

using namespace std;
using namespace pb2;

void validateISBN10(const vector<int> & isbnDigits) {
    /* Check size */
    if (isbnDigits.size() != 10)
        throw ValidationException("ISBN10 with less than 9 digits given!");

    /* Calculate and compare checksum */
    int checksum = 0;
    for (int i = 0; i <= 8; i++)
        checksum += (i+1) * isbnDigits[i];
    checksum %= 11;

    if (checksum != isbnDigits[9])
        throw ValidationException("Invalid ISBN10 checksum!");
}

vector<int> convertISBNtoEANDigits(const string & isbn) {
    vector<int> eanDigits = extractEANDigits(isbn);
    auto nDigits = eanDigits.size();

    // For ISBN13, there is no need for conversion
    if (nDigits == 13)
        return eanDigits;

    // For ISBN10, check the ISBN10 then convert to EAN
    else if (nDigits == 10) {
        validateISBN10(eanDigits);

        eanDigits.insert(eanDigits.begin(), {9,7,8});
        eanDigits.pop_back();
        eanDigits.push_back(calculateEANChecksum(eanDigits));

        return eanDigits;
    }
    else
        throw ValidationException("ISBN has invalid length");
}

Book::Book(shared_ptr<Database> database, const string &isbn)
: Medium(database, convertISBNtoEANDigits(isbn)){

}

Book::Book(shared_ptr<Database> database, SqlPreparedStatement & query,
             const map<string, int> & columnIndexes
) : Medium(database, query, columnIndexes){

}

Book::~Book() = default;


const string & Book::getType() const {
    static string t("book");
    return t;
}
