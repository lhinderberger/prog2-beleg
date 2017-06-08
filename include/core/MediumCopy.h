#ifndef PROG2_BELEG_MEDIUMCOPY_H
#define PROG2_BELEG_MEDIUMCOPY_H

#include <memory>
#include "Medium.h"

namespace pb2 {
    class Lending;
    class MediumCopy_priv;

    /**
     * A Medium can be available in multiple copies distributed throughout multiple
     * locations within the library.
     * Each copy is identified by the medium's EAN and a serial number, starting with
     * 1 for the first copy, 2 for the second copy and so on.
     * Copies can be lent to Users.
     */
    class MediumCopy : public DatabaseObject {
    private:
        std::unique_ptr<MediumCopy_priv> priv;
        MediumCopy(std::shared_ptr<Database> database,
                   std::shared_ptr<Medium> medium, int serialNumber);

    public:
        /**
         * Creates a new Medium Copy
         *
         * @param medium Pass in a valid pointer to a medium. Make sure the medium
         * is persisted before persisting the MediumCopy.
         * @param serialNumber Either a number starting from 1 or 0 or less to
         * auto-generate a new serialNumber on insert.
         */
        static std::shared_ptr<MediumCopy> construct(std::shared_ptr<Database> database,
            std::shared_ptr<Medium> medium, int serialNumber);

        virtual const std::string & getTableName() const override;
        virtual const std::string & getType() const override;

        virtual void load(SqlPreparedStatement & query,
            const std::map<std::string, std::string> & alternativeColumnNames
            = std::map<std::string, std::string>()) override;
        virtual void persist() override;

        /**
         * If a LibraryUser has currently lent the copy, this will return a pointer
         * to the relevant Lending instance. Otherwise it will return a null pointer.
         */
        std::shared_ptr<Lending> getActiveLending() const;

        std::shared_ptr<Medium> getMedium() const;

        /**
         * If a medium is deaccessioned, it is no longer available for lending.
         */
        bool getDeaccessioned() const;
        void setDeaccessioned(bool deaccessioned);

        /**
         * Pass in any string that best describes the location of the medium.
         */
        std::string getLocation() const;
        void setLocation(const std::string & location);

        int getSerialNumber() const;
    };
}

#include "Lending.h"

#endif