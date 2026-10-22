#pragma once

#include "util/types.hh"

namespace treyarch {
    // SM3's tlFixedBitvector, one word more than the bit count needs
    template <typename base_type, u32 number_of_bits>
    struct tl_fixed_bitvector {
        enum {
            log_base_type_bits = sizeof(base_type) == 16 ? 7 :
                                 sizeof(base_type) == 8  ? 6 :
                                 sizeof(base_type) == 4  ? 5 :
                                 sizeof(base_type) == 2  ? 4 : 3,

            base_type_bits = 1 << log_base_type_bits,
            data_count     = number_of_bits / base_type_bits + 1
        };

        base_type data[data_count];

        tl_fixed_bitvector() {
            set_all_zeroes();
        }

        void set_all_zeroes() {
            for (i32 i = 0; i < data_count; i++)
                data[i] = 0;
        }
    };
} // treyarch
