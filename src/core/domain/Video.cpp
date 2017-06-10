#include "core/domain/Video.h"

#include "core/domain/Medium.priv.h" // Video_priv is defined there as long as it's empty.

using namespace std;
using namespace pb2;

Video::Video(shared_ptr<Database> database, const string & ean)
        : Medium(database, ean){

}

Video::Video(shared_ptr<Database> database, SqlitePreparedStatement & query,
                   const map<string, int> & columnIndexes
) : Medium(database, query, columnIndexes){

}

Video::~Video() = default;


const string & Video::getType() const {
    static string t("video");
    return t;
}
