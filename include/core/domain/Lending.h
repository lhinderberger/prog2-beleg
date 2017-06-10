#ifndef PROG2_BELEG_LENDING_H
#define PROG2_BELEG_LENDING_H

#include <ctime>
#include <memory>

#include "core/DatabaseObject.h"
#include "core/DatabaseObjectFactory.h"
#include "LibraryUser.h"
#include "MediumCopy.h"

namespace pb2 {
    class Lending_priv;

    /**
     * Relationship between a MediumCopy and a LibraryUser, basically meaning what media
     * the user has lent and for how long.
     */
    class Lending : public DatabaseObject {
        friend class DatabaseObjectFactory<Lending>;

    private:
        std::unique_ptr<Lending_priv> priv;

        /**
         * Creates a new Lending.
         *
         * @param mediumCopy The medium to be lent to the user.
         * @param libraryUser The user to lend the medium. Must be eligible for new lending.
         * @param timestampLent The beginning time point of the Lending. Must be unique
         * for each mediumCopy. Pass in -1 to take the current time.
         * @return
         */
        Lending(
                std::shared_ptr<Database> database,
                std::shared_ptr<MediumCopy> mediumCopy,
                std::shared_ptr<LibraryUser> libraryUser,
                time_t timestampLent
        );

        PB2_DECLARE_LOAD_CONSTRUCTOR(Lending);

    protected:
        virtual void persistImpl() override;

    public:
        static const std::string tableName;

        virtual ~Lending();

        // Getters for read-only values set in constructor.
        std::shared_ptr<MediumCopy> getMediumCopy();
        std::shared_ptr<LibraryUser> getLibraryUser();
        time_t getTimestampLent() const;

        /**
         * Extends the Lending's runtime by a given number of days.
         * @param days
         */
        void extend(int days = -1);

        /**
         * Calculate how many days are left for the user to return the Medium. This can
         * be negative if the medium is or was overdue (for determining whether a Lending
         * is overdue, please use isOverdue()).
         * If the medium is already returned, the difference in days between
         * timestampLent and timestampReturned will be returned.
         */
        int getDaysLeft() const;

        /**
         * Returns for how many days the medium is lent to the LibraryUser, starting with
         * the day of timestampLent.
         */
        unsigned int getRuntime() const;

        /**
         * If the user has already returned the medium, this will return the timestamp
         * of when the medium was returned. Otherwise it will throw an exception.
         * @return
         */
        time_t getTimestampReturned() const;

        /**
         * Returns how often the Lending has been extended, starting with 0 for "has not
         * been extended at all".
         */
        unsigned int getTimesExtended() const;

        /**
         * Convenience function for (getDaysLeft() < 0 && !isReturned())
         */
        inline bool isOverdue() const { return getDaysLeft() < 0 && !isReturned(); }

        /**
         * Has the user returned the medium yet?
         */
        bool isReturned() const;

        /**
         * To finalize the Lending by the User returning it, call this function.
         * @param timestampReturned Pass in -1 to use the current timestamp. Otherwise
         * pass any timestamp greater than timestampLent.
         */
        void returnL(time_t timestampReturned = -1);
    };
}

#endif
