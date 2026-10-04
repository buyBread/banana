#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    class igo_3d_text;

    class region_spawn_manager {

    public:
        void** vtable;
        u8     reserved_004[0x04];
        u8     unk_008;

        static void draw_text();
    };

    namespace references {
        inline util::memory_reference<u8> region_spawns_enabled { 0x00BE73FE };

        inline util::memory_reference<region_spawn_manager*> region_spawn_manager { 0x010FA2C4 };

        inline util::memory_reference<igo_3d_text*> region_spawn_text { 0x010F9BC4 };
    } // references

    ASSERT_OFFSETOF(region_spawn_manager, unk_008, 0x08);
} // treyarch
