#pragma once

#include <array>

#include "util/types.hh"

namespace treyarch {
    // 1 << n, read by tl_fixed_bitvector in place of a shift
    inline constexpr std::array<u64, 64> bit_masks = [] {
        std::array<u64, 64> masks {};

        for (u32 i = 0; i < 64; i++)
            masks[i] = 1ull << i;

        return masks;
    }();

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

        i32 base_index_for_bit(i32 x) const {
            return x >> log_base_type_bits;
        }

        base_type bit_mask_for_bit(i32 x) const {
            return (base_type)bit_masks[x & (base_type_bits - 1)];
        }

        tl_fixed_bitvector() {
            set_all_zeroes();
        }

        void set_all_zeroes() {
            for (i32 i = 0; i < data_count; i++)
                data[i] = 0;
        }

        void set_one(i32 bit_position) {
            data[base_index_for_bit(bit_position)] |= bit_mask_for_bit(bit_position);
        }

        void set_zero(i32 bit_position) {
            data[base_index_for_bit(bit_position)] &= ~bit_mask_for_bit(bit_position);
        }

        base_type is_one(i32 bit_position) const {
            return data[base_index_for_bit(bit_position)] & bit_mask_for_bit(bit_position);
        }
    };
} // treyarch
