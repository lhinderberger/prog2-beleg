#include "core/Video.h"

#include "core/Medium.priv.h" // Video_priv is defined there as long as it's empty.

using namespace std;
using namespace pb2;

Video::Video(shared_ptr<Database> database, const string & ean)
        : Medium(database, ean){

}

shared_ptr<Video> Video::construct(shared_ptr<Database> database, const string &ean) {
    return shared_ptr<Video>(new Video(database, ean));
}

const string & Video::getType() const {
    static string t("video");
    return t;
}
