#ifndef PROG2_BELEG_MEDIUM_H
#define PROG2_BELEG_MEDIUM_H

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "Author.h"
#include "core/DatabaseObject.h"

namespace pb2 {
    class Medium_priv;
    class MediumCopy;

    /**
     * Represents one abstract Medium entity in the Database.
     *
     * A Medium can have multiple copies in different locations and in different statuses.
     * At least one copy should be there in order for it to be visible in the GUI.
     * LibraryUsers can lend MediumCopies; Medium entities only provide centralized,
     * descriptive information for a set of MediumCopies.
     */
    class Medium : public DatabaseObject {
    private:
        std::unique_ptr<Medium_priv> priv;

    protected:
        /**
         * Creates a new Medium
         *
         * @param ean A valid EAN number. If the medium does not have an EAN number, you
         * can use a custom, unused local EAN number in the GS1 020-029 range.
         */
        Medium(std::shared_ptr<Database> database, const std::string & ean);
        Medium(std::shared_ptr<Database> database, const std::vector<int> & eanDigits);
        PB2_DECLARE_LOAD_CONSTRUCTOR(Medium);

        virtual void persistImpl() override;

    public:
        static const std::string tableName;
        static const std::string primaryKeyColumn;
        static const std::set<std::string> & allowedFormats();

        static std::unique_ptr<AbstractDatabaseObjectFactory> polymorphicFactory(
            std::shared_ptr<Database> database, const std::string & type
        );

        virtual ~Medium();

        virtual void del() override;

        /**
         * Lower-case class name of the concrete medium object
         */
        virtual const std::string & getType() const = 0;

        /**
         * Lazy-loads all copies of the Medium that currently are available in the library.
         */
        std::vector<std::shared_ptr<MediumCopy>> queryCopies() const;



        // Getters / setters:

        /**
         * The EAN identifies any Medium in the library (acts as primary key).
         * If the Medium you're trying to add to the library does not have an EAN
         * number, the GS1 prefixes 020-029 shall be used to create an EAN number
         * local to your library (see constructor for details).
         */
        std::string getEAN() const;


        /** The physical format used to store the Medium. */
        std::string getFormat() const;

        /**
         * Sets the physical format of the Medium.
         * @param format One of the allowed formats. @see allowedFormats().
         */
        void setFormat(const std::string & format);


        std::shared_ptr<Author> getAuthor() const;
        void setAuthor(std::shared_ptr<Author> author);

        std::string getSubtitle() const;
        void setSubtitle(const std::string & subtitle);

        std::string getTitle();
        void setTitle(const std::string & title);
    };
}

#include "MediumCopy.h"

#endif
