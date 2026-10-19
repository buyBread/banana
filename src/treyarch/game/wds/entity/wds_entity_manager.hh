#pragma once

#include "treyarch/shared/mutex.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class wds_entity_manager {

    public:
        engine_recursive_lock lock;
        u8                    unk_10[0x0C];   // slots' entity instance lists are allocated from here
        u8                    reserved_01c[0x14];
        void*                 unk_30;         // a vector of the slots pushed as resource context
        u8                    reserved_034[0x04];
    };

    ASSERT_SIZEOF  (wds_entity_manager,         0x38);
    ASSERT_OFFSETOF(wds_entity_manager, unk_10, 0x10);
    ASSERT_OFFSETOF(wds_entity_manager, unk_30, 0x30);
} // treyarch
