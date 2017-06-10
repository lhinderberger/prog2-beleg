#ifndef PROG2_BELEG_SOFTWARE_H
#define PROG2_BELEG_SOFTWARE_H

#include "Medium.h"
#include "DatabaseObjectFactory.h"

namespace pb2 {
    class Software_priv;

    class Software : public Medium {
        friend class DatabaseObjectFactory<Software>;

    private:
        std::unique_ptr<Software_priv> priv;

        /**
         * Creates a new Software Medium
         * @param ean @see Medium constructor
         */
        Software(std::shared_ptr<Database> database, const std::string & ean);
        PB2_DECLARE_LOAD_CONSTRUCTOR(Software);

    public:
        virtual ~Software();

        virtual const std::string & getType() const override;
    };
}

#endif
