#pragma once

#include "treyarch/shared/singleton.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class spawn_table_manager : public singleton<spawn_table_manager, 0x010FB2B4> {

    public:
        u8    reserved_004[0x48];
        void* unk_04c; // a 0x198-byte object built by the constructor (sub_94D260)
    };

    ASSERT_OFFSETOF(spawn_table_manager, unk_04c, 0x4C);
} // treyarch
