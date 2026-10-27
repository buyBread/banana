#pragma once

#include "game/fixed_string.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace nal {
    // how to play one source transport list's anims on the map's rig
    struct skeleton_map_entry {
        fixed_string source_name;
        u32          source_bone_count;
        i32*         bone_map;           // source bone to target bone, -1 drops it
        u32          target_counts[11];
        i32*         field_maps[11];     // target instance to source instance, -1 keeps the default
    };

    // SKMP, owned by the target transport list
    struct skeleton_map {
        fixed_string         name;  // the target transport list's name
        skeleton_map_entry** entries;
        u32                  entry_count;

        skeleton_map_entry* get_entry(fixed_string* source_name);
    };

    ASSERT_OFFSETOF(skeleton_map_entry, source_name,       0x00);
    ASSERT_OFFSETOF(skeleton_map_entry, source_bone_count, 0x08);
    ASSERT_OFFSETOF(skeleton_map_entry, bone_map,          0x0C);
    ASSERT_OFFSETOF(skeleton_map_entry, target_counts,     0x10);
    ASSERT_OFFSETOF(skeleton_map_entry, field_maps,        0x3C);

    ASSERT_OFFSETOF(skeleton_map, name,        0x00);
    ASSERT_OFFSETOF(skeleton_map, entries,     0x08);
    ASSERT_OFFSETOF(skeleton_map, entry_count, 0x0C);
}} // treyarch::nal
