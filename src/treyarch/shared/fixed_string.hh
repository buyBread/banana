#pragma once

#include "util/macros/sanity_assert.hh"
#include "treyarch/shared/hash/string_hash.hh"

namespace treyarch {
    struct fixed_string {
        char*       text;
        string_hash hash;

        fixed_string &operator=(const fixed_string &other) {
            text = other.text;
            hash = other.hash;

            return *this;
        }
    };

    ASSERT_SIZEOF(fixed_string, 0x08);

    ASSERT_OFFSETOF(fixed_string, text, 0x00);
    ASSERT_OFFSETOF(fixed_string, hash, 0x04);
}