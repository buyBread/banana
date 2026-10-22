#pragma once

#include "treyarch/shared/fixed_string.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace nal {
    // how to play one source transport list's anims on the map's rig
    struct skeleton_map_entry {
        fixed_string source_name;
    };

    // SKMP, owned by the target transport list
    struct skeleton_map {
        fixed_string         name;  // the target transport list's name
        skeleton_map_entry** entries;
        u32                  entry_count;

        skeleton_map_entry* get_entry(fixed_string* source_name);
    };

    ASSERT_OFFSETOF(skeleton_map_entry, source_name, 0x00);

    ASSERT_OFFSETOF(skeleton_map, name,        0x00);
    ASSERT_OFFSETOF(skeleton_map, entries,     0x08);
    ASSERT_OFFSETOF(skeleton_map, entry_count, 0x0C);
}} // treyarch::nal
