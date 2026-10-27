#pragma once

namespace treyarch {
    // SM3 bounded_variable; only the layout so far
    template<typename T>
    struct bounded_variable {
        T val;
        T min_val;
        T max_val;

        T get_max_val() const {
            return max_val;
        }
    };
} // treyarch
