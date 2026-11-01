#pragma once

#include "treyarch/mash/types.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // SM3 smoothilator
    template<typename T>
    class smoothilator {

        T   val;
        f32 weight;

    public:
        smoothilator() : weight(0.5f) {}

        explicit smoothilator(mash::generating_vtable_version) {}

        const T &get_val() const {
            return val;
        }
    };
} // treyarch
