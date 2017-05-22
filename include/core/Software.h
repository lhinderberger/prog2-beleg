#ifndef PROG2_BELEG_SOFTWARE_H
#define PROG2_BELEG_SOFTWARE_H

#include "Medium.h"

namespace pb2 {
    class Software_priv;

    class Software : public Medium {
    private:
        std::unique_ptr<Software_priv> priv;

    public:
        /**
         * Creates a new Software Medium
         * @param ean @see Medium constructor
         */
        Software(const std::string & ean);

        virtual std::string getType() const;
    };
}

#endif
