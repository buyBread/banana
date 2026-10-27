#pragma once

#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // SM3's tlFixedString: the hash covers the whole string, the text is cut to fit
    struct alignas(8) tl_fixed_string {
        string_hash hash;
        char        text[28];

        tl_fixed_string() : hash(), text() {}

        tl_fixed_string(const char* value);

        bool operator==(const tl_fixed_string &other) const;

        void construct_mashed_class() {}
        void destruct_mashed_class() {}
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}
    };

    ASSERT_SIZEOF  (tl_fixed_string,       0x20);
    ASSERT_OFFSETOF(tl_fixed_string, text, 0x04);
} // treyarch
