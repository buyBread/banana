#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class boolx {

        i8 value;

public:
        boolx()                   : value(0)             {}
        boolx(bool state)         : value(state ? 1 : 0) {}
        boolx(const boolx &other) : value(other.value)   {}

        boolx &operator=(const boolx &other) {
            value = other.value;

            return *this;
        }

        boolx &operator=(bool state) {
            value = state ? 1 : 0;

            return *this;
        }

        operator bool() const {
            return (bool)value;
        }

        bool to_bool() const {
            return (bool)value;
        }

        i8 get_goodies() const {
            return value;
        }

    };

    ASSERT_SIZEOF(boolx, 0x1);
}