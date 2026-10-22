#pragma once

#include "util/types.hh"

namespace treyarch { namespace nal {
    // bits of an escaped value, by code >> 2
    inline const u16 entropy_widths[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 15, 20, 30 };

    // nalBitstream, the 32-bit main stream; bits come low first
    struct bitstream {
        u32* ptr;
        u32  bit;

        // inlined @ sub_9CDEE0
        // inlined @ sub_9CE3E0
        u32 get_bit() {
            u32 value = (*ptr >> bit) & 1;

            bit++;
            ptr += (bit >> 5) & 1;
            bit &= 0x1F;

            return value;
        }

        // inlined @ sub_9CDEE0
        // inlined @ sub_9CE3E0
        u32 get(u32 count) { // reads the next word too, whether it's needed or not
            i8  spill = (i8)(count - (32 - bit));
            u32 low   = (*ptr >> bit) & ((1u << count) - 1);
            u32 high  = (ptr[1] & ((1u << (spill < 0 ? 0 : spill)) - 1)) << (32 - bit);

            ptr += spill >= 0;
            bit  = (count + bit) & 0x1F;

            return low | high;
        }

        // inlined @ sub_9CDEE0
        // inlined @ sub_9CE3E0
        i32 get_value(u16 code) { // only codes of 4 and up read a value
            i32 present = (code & 0xFFFC) ? get_bit() : 0;
            i32 escape  = present ? get_bit() : 0;
            i32 base    = (code & 3) + 2;
            i32 width   = -present & (((base & (escape - 1)) | (-escape & entropy_widths[code >> 2])) + 1);
            u32 bits    = get(width);

            i32 magnitude = (-escape & (1 << base)) + (i32)(bits >> 1) + 1;

            return -present & (magnitude - ((2 * magnitude) & -(i32)(bits & 1)));
        }
    };

    // one of the eight lane streams, read in 16-bit words; values are 16-bit throughout
    struct lane_bitstream {
        u16* ptr;
        u16  bit;

        // inlined @ sub_9CE3E0
        u16 get_bit() {
            u16 value = (*ptr >> bit) & 1;

            ptr += ((bit + 1) >> 4) & 1;
            bit  = (bit + 1) & 0xF;

            return value;
        }

        // inlined @ sub_9CE3E0
        u16 get(u16 count) {
            i8  spill = (i8)(count - (16 - bit));
            u16 high  = (u16)((ptr[1] & (u16)((1 << (spill < 0 ? 0 : spill)) - 1)) << (16 - bit));
            u16 low   = (u16)((*ptr >> bit) & ((1 << count) - 1));

            ptr += spill >= 0;
            bit  = (count + bit) & 0xF;

            return high | low;
        }

        // inlined @ sub_9CE3E0
        i16 get_value(u16 code) { // any nonzero code reads a value
            u16 present      = code ? get_bit() : 0;
            u16 present_mask = (u16)-present;
            u16 escape       = present_mask ? get_bit() : 0;
            u16 escape_mask  = (u16)-escape;
            u16 base         = (code & 3) + 2;
            u16 width        = (u16)((((entropy_widths[code >> 2] & escape_mask) | (base & ~escape_mask)) + 1) & present_mask);
            u16 bits         = get(width);

            u16 magnitude = (u16)((escape_mask & (1 << base)) + (bits >> 1) + 1);

            return (i16)(present_mask & (magnitude - ((2 * magnitude) & -(bits & 1))));
        }
    };
}} // treyarch::nal
