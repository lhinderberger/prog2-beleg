#ifndef PROG2_BELEG_MEDIUM_H
#define PROG2_BELEG_MEDIUM_H

#include <memory>
#include <string>
#include <vector>

namespace pb2 {
    class Medium_priv;

    class Medium {
    private:
        std::unique_ptr<Medium_priv> priv;

    public:
        static const std::vector<std::string> & allowedFormats();

        /**
         * Creates a new Medium
         *
         * @param ean A valid EAN number or the string "generate" to generate a new,
         * unused local EAN number in the GS1 020-029 range on insertion.
         */
        Medium(const std::string & ean);

        virtual ~Medium();

        /**
         * Lower-case class name of the concrete Medium object (such as "book", "video")
         */
        virtual std::string getType() const = 0;

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


        std::string getSubtitle() const;
        void setSubtitle(const std::string & subtitle);

        std::string getTitle();
        void setTitle(const std::string & title);
    };
}

#endif
