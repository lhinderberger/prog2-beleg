#include "core/domain/Software.h"

#include "core/domain/Medium.priv.h" // Software_priv is defined there as long as it's empty.

using namespace std;
using namespace pb2;

Software::Software(shared_ptr<Database> database, const string & ean)
        : Medium(database, ean){

}

Software::Software(shared_ptr<Database> database, SqlPreparedStatement & query,
           const map<string, int> & columnIndexes
) : Medium(database, query, columnIndexes){

}

Software::~Software() = default;


const string & Software::getType() const {
    static string t("software");
    return t;
}
