#pragma once

#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/mash/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    // SM3's generated PackerAndEngine.h layout
    struct level_descriptor {
        mash::string level_name;
        mash::string path;
        mash::string short_level_name;
        i32          memory_map_index;
        i32          district_count;
        void*        blocks_in_this_level;
        void*        sounds_by_mission;
    };

    // the GAME pack's type-1 "level" resource
    struct level_descriptor_set {
        u8                     reserved_000[0x04];
        mash::vector
            <level_descriptor> descriptors;
    };

    ASSERT_OFFSETOF(level_descriptor, path,             0x0C);
    ASSERT_OFFSETOF(level_descriptor, short_level_name, 0x18);
    ASSERT_OFFSETOF(level_descriptor, memory_map_index, 0x24);

    ASSERT_OFFSETOF(level_descriptor_set, descriptors, 0x04);
} // treyarch
