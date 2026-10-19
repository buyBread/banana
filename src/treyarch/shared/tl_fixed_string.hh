#pragma once

#include "treyarch/shared/hash/string_hash.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    // SM3's tlFixedString: the hash covers the whole string, the text is cut to fit
    struct alignas(8) tl_fixed_string {
        string_hash hash;
        char        text[28];

        void construct_mashed_class() {}
        void destruct_mashed_class() {}
    };

    ASSERT_SIZEOF  (tl_fixed_string,       0x20);
    ASSERT_OFFSETOF(tl_fixed_string, text, 0x04);
} // treyarch
