#pragma once

#include "treyarch/mash/mash_info.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/types.hh"
#include "banana/util/macros/sanity_assert.hh"

namespace treyarch {
    struct text_resource {
        void*        self; // the resource handler points this at the resource itself
        mash::string text;

        // inlined @ sub_764B60
        void construct_mashed_class() {
            self = nullptr;

            text.construct_mashed_class();
        }

        // inlined @ sub_749FF0
        void destruct_mashed_class() {
            text.destruct_mashed_class();
        }

        // inlined @ sub_764B60
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            text.unmash(mash_info, this, buffer);
        }
    };

    ASSERT_SIZEOF  (text_resource,       0x10);
    ASSERT_OFFSETOF(text_resource, text, 0x04);
} // treyarch
