#pragma once

namespace treyarch {
    // SM3 bounded_variable
    template<typename T>
    struct bounded_variable {
        T val;
        T min_val;
        T max_val;

        const T &get() const {
            return val;
        }

        const T &get_max() const {
            return max_val;
        }

        T get_max_val() const {
            return max_val;
        }

        void set(const T &b) {
            val = b;

            if (val > max_val)
                val = max_val;

            if (val < min_val)
                val = min_val;
        }

        void set_max_value(const T &b) {
            max_val = b;

            if (max_val < min_val) {
                T tmp   = max_val;
                max_val = min_val;
                min_val = tmp;
            }

            set(val);
        }

        void set_max(const T &b) {
            set_max_value(b);
        }
    };
} // treyarch
