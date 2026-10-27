#pragma once

#include "treyarch/mash/types.hh"
#include "treyarch/mash/virtual_base.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // the base of every cowbell; mashed ones get their retail vtable from the type registry
    class cowbell : public mash::mash_virtual_base {};

    class cowbell_file {

    public:
        void*              self; // the resource handler points this at the file itself
        treyarch::cowbell* cowbell;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (cowbell, 0x04);

    ASSERT_SIZEOF  (cowbell_file,          0x08);
    ASSERT_OFFSETOF(cowbell_file, cowbell, 0x04);
} // treyarch
