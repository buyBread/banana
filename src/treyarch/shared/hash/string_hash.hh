#pragma once

#include "treyarch/shared/hash/algo.hh"
#include "treyarch/shared/mash/types.hh"
#include "util/types.hh"

namespace treyarch {
    struct string_hash {
        u32 source_hash_code;

                 constexpr string_hash()         noexcept : source_hash_code(0)    {}
        explicit constexpr string_hash(u32 hash) noexcept : source_hash_code(hash) {}

        // sub_A6C7E0
        void initialize(      mash::allocation_scope scope,
                        const char*                  requested_string = nullptr,
                              u32                    requested_hash   = 0) {

            if (scope != mash::ALLOCATED)
                return;

            if (requested_hash)
                source_hash_code = requested_hash;
            else if (requested_string && *requested_string)
                source_hash_code = hash::djb2(requested_string);
            else
                source_hash_code = 0;
        }

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