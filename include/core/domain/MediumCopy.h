#ifndef PROG2_BELEG_MEDIUMCOPY_H
#define PROG2_BELEG_MEDIUMCOPY_H

#include <memory>
#include "Medium.h"
#include "core/DatabaseObjectFactory.h"

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
        friend class DatabaseObjectFactory<MediumCopy>;

    private:
        std::unique_ptr<MediumCopy_priv> priv;

        /**
         * Creates a new Medium Copy
         *
         * @param medium Pass in a valid pointer to a medium. Make sure the medium
         * is persisted before persisting the MediumCopy.
         * @param serialNumber Either a number starting from 1 or 0 or less to
         * auto-generate a new serialNumber on insert.
         */
        MediumCopy(
                std::shared_ptr<Database> database, std::shared_ptr<Medium> medium,
                int serialNumber
        );
        PB2_DECLARE_LOAD_CONSTRUCTOR(MediumCopy);

    protected:
        virtual void persistImpl() override;

    public:
        static const std::string tableName;

        virtual ~MediumCopy();

        virtual void del() override;

        /**
         * If a LibraryUser has currently lent the copy, this will return a pointer
         * to the relevant Lending instance. Otherwise it will return a null pointer.
         */
        std::shared_ptr<Lending> getActiveLending() const;

        std::shared_ptr<Medium> getMedium() const;
        std::string getMediumEAN() const;

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

        /**
         * Pass in any string that hints the availability of the medium.
         * Note: This is usually set by Lending!
         * Caution: Do not rely on this for availability checking - this shall only serve
         * as a general hint (i.e. in overview tables).
         */
        std::string getAvailabilityHint() const;
        void setAvailabilityHint(const std::string & availabilityHint);

        int getSerialNumber() const;
    };
}

#include "Lending.h"

#endif