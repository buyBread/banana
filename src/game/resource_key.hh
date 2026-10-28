#pragma once

#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace mash {
        class mash_info_struct;
    } // mash

    struct resource_key {
        string_hash hash;
        u32         type;

        // sub_5FD120
        string_hash get_hash() const {
            return hash;
        }

        // sub_72DDE0
        void set(string_hash value, u32 resource_type) {
            hash = value;
            type = resource_type;
        }

        // inlined @ sub_5D5F70
        bool is_set() const {
            string_hash none;
            none.initialize(mash::ALLOCATED);

            return hash != none;
        }

        void construct_mashed_class() {}
        void destruct_mashed_class() {}

        // sub_636AB0
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}
    };

    ASSERT_SIZEOF  (resource_key,       0x08);
    ASSERT_OFFSETOF(resource_key, hash, 0x00);
    ASSERT_OFFSETOF(resource_key, type, 0x04);
} // treyarch
