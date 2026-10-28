#pragma once

#include <cstdlib>

#include "banana/util/types.hh"

namespace treyarch {
    template<typename T>
    class variance_variable {

    public:
        T base;
        T variance;

        // sub_434A70
        T get_random_value() const { // uniform in [base - variance, base + variance]
            T random = (T)std::rand() * (T)(1.0f / 32767.0f) * (T)2;

            return (random - (T)1) * variance + base;
        }

        // inlined @ sub_434AD0
        // inlined @ sub_84D1E0
        // inlined @ sub_84DB60
        T get_random_value_inline() const { // get_random_value as retail inlines it: doubles between two roundings
            const T random = (T)((f64)(T)std::rand() * (f64)(1.0f / 32767.0f) * 2.0);

            return (T)(((f64)random - 1.0) * variance + base);
        }
    };
} // treyarch
