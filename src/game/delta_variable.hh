#pragma once

namespace treyarch {
    // SM3 delta_variable
    template<typename T>
    struct delta_variable {
        T previous;
        T current;

        void force(const T &value) {
            current  = value;
            previous = current;
        }

        delta_variable &operator=(const T &value) {
            previous = current;
            current  = value;

            return *this;
        }
    };
} // treyarch
