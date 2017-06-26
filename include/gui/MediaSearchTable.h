#ifndef PROG2_BELEG_MEDIA_SEARCH_TABLE_H
#define PROG2_BELEG_MEDIA_SEARCH_TABLE_H

#include "DatabaseSearchTable.h"

namespace pb2 {
    class MediaSearchTable : public DatabaseSearchTable {
    public:
        MediaSearchTable(wxWindow * parent);
        virtual ~MediaSearchTable() = default;
    };
}

#endif
