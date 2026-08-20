#pragma once

#include "util/types.hh"

namespace treyarch {
    struct string_hash {
        u32 source_hash_code;

                 constexpr string_hash()         noexcept : source_hash_code(0)    {}
        explicit constexpr string_hash(u32 hash) noexcept : source_hash_code(hash) {}

        constexpr bool valid() const noexcept {
            return source_hash_code != 0;
        }

        friend constexpr bool operator==(string_hash lhs, string_hash rhs) noexcept {
            return lhs.source_hash_code == rhs.source_hash_code;
        }

        friend constexpr bool operator<(string_hash lhs, string_hash rhs) noexcept {
            return lhs.source_hash_code < rhs.source_hash_code;
        }        
    };
} // treyarch