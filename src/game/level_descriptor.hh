#pragma once

#include "vc8/list.hh"
#include "vc8/map.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "game/resource_key.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // SM3's generated PackerAndEngine.h layout
    struct level_descriptor {
        mash::string level_name;
        mash::string path;
        mash::string short_level_name;
        i32          memory_map_index;
        i32          district_count;
        void*        blocks_in_this_level;
        
        vc8::map
            <resource_key, vc8::list<resource_key>>* sounds_by_mission;

        ~level_descriptor();

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    // the GAME pack's type-1 "level" resource
    struct level_descriptor_set {
        void*                  self; // the resource handler points this at the set itself
        mash::vector
            <level_descriptor> descriptors;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (level_descriptor,                    0x34);
    ASSERT_OFFSETOF(level_descriptor, path,              0x0C);
    ASSERT_OFFSETOF(level_descriptor, short_level_name,  0x18);
    ASSERT_OFFSETOF(level_descriptor, memory_map_index,  0x24);
    ASSERT_OFFSETOF(level_descriptor, sounds_by_mission, 0x30);

    ASSERT_SIZEOF  (level_descriptor_set,              0x18);
    ASSERT_OFFSETOF(level_descriptor_set, descriptors, 0x04);
} // treyarch
