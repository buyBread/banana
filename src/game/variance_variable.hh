#pragma once

#include <cstdlib>

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
    };
} // treyarch
