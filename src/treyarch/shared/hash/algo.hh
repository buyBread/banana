#pragma once

#include "util/types.hh"

namespace treyarch { namespace hash {
    inline u32 djb2(const char* string) {
        u32 hash = 0;

        for (u8 c = (u8)*string; c; c = (u8)*++string) {
            const u32 x = c - 'A';

            hash = hash * 33u + (u8)(c + ((x <= ('Z' - 'A')) << 5));
        }

        return hash;
    }

    inline u32 crc2(const void* data, size_t size, u32 previous_crc) {
        auto bytes = (const u8*)data;

        u32 crc = ~previous_crc;

        for (size_t index = 0; index < size; ++index) {
            crc ^= bytes[index];

            for (unsigned bit = 0; bit < 8; ++bit) {
                u32 mask = 0u - (crc & 1u);

                crc = (crc >> 1) ^ (0xEDB88320u & mask);
            }
        }

        return ~crc;
    }

    inline u32 crc2(const void* data, size_t size) {
        return crc2(data, size, 0);
    }
}} // treyarch::hash
