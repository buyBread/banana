#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    class spawn_table_manager {

    public:
        void** vtable;
        u8     reserved_004[0x48];
        void*  unk_04c; // a 0x198-byte object built by the constructor (sub_94D260)
    };

    namespace references {
        inline util::memory_reference<spawn_table_manager*> spawn_table_manager { 0x010FB2B4 };
    } // references

    ASSERT_OFFSETOF(spawn_table_manager, unk_04c, 0x4C);
} // treyarch
