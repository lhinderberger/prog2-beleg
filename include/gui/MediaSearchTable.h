#ifndef PROG2_BELEG_MEDIA_SEARCH_TABLE_H
#define PROG2_BELEG_MEDIA_SEARCH_TABLE_H

#include "TwoQuerySearchTable.h"
#include "core/Database.h"
#include "core/domain/MediumCopy.h"
#include "core/sqlite/SqlitePreparedStatement.h"

namespace pb2 {
    /**
     * DatabaseSearchTable for MediumCopy entities.
     */
    class MediaSearchTable : public TwoQuerySearchTable {
    protected:
        virtual std::string getListSQL() override;
        virtual std::string getSearchSQL() override;
        virtual wxString getColumnContent(int column) override;

    public:
        MediaSearchTable(wxWindow * parent, wxWindowID id, std::shared_ptr<Database> database);
        virtual ~MediaSearchTable() = default;

        std::pair<std::string,int> getSelectedEanSerial() const;
        std::shared_ptr<MediumCopy> getSelectedMediumCopy() const;
    };
}

#endif
