#pragma once

#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/mash/virtual_base.hh"
#include "treyarch/shared/mutex.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    // the base of every typed parameter; mashed ones get their retail vtable from the type registry
    class param_base : public mash::mash_virtual_base {

    public:
        u8 unk_04[0x08];
    };

    // the lookups into it stay native for now
    class param_block {

    public:
        mash::vector
            <param_base>*     params;
        u32                   reserved_004;
        engine_recursive_lock lock;
        u32                   unk_018;
        bool                  owns_params; // the list was allocated on the heap rather than unmashed
        u8                    pad_01d[0x03];

        ~param_block();

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (param_base, 0x0C);

    ASSERT_SIZEOF  (param_block,              0x20);
    ASSERT_OFFSETOF(param_block, params,      0x00);
    ASSERT_OFFSETOF(param_block, lock,        0x08);
    ASSERT_OFFSETOF(param_block, unk_018,     0x18);
    ASSERT_OFFSETOF(param_block, owns_params, 0x1C);
} // treyarch
