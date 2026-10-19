#pragma once

#include "treyarch/game/static_colgeom_root.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/array_replacement.hh"
#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class subdivision_node;
    struct region;

    // sound event names, one per act (mission_manager::current_act);
    // the current box's go to the environment game object, by whether the listener is below 80 altitude
    class audio_box_events {

    public:
        u32 unk_00;
        u32 unk_04;
        u32 unk_08;

        mash::array_replacement
            <string_hash, 3> low_altitude_events;
        mash::array_replacement
            <string_hash, 3> high_altitude_events;
        mash::array_replacement
            <string_hash, 3> low_altitude_stop_events; // posted when the box stops being current
        mash::array_replacement
            <string_hash, 3> high_altitude_stop_events;
        mash::array_replacement
            <string_hash, 3> unk_1c;

        mash::array_replacement
            <mash::array_replacement
                <string_hash, 5>, 3> unk_20;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    // the set's activate links each box to its node in the root
    class audio_box {

    public:
        string_hash       id;
        audio_box_events  events;
        subdivision_node* node;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    // activate posts each one's event at its position, with the entry's address as the game object
    struct region_audio_box_set_unk_4c_entry {
        u8          unk_00[0x10];
        string_hash event;
        u8          unk_14[0x44];
        u32         unk_58;
        u32         unk_5c;

        void construct_mashed_class() {}
        void destruct_mashed_class() {}
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}
    };

    struct region_audio_box_set_unk_4c {
        mash::vector
            <region_audio_box_set_unk_4c_entry> unk_00;
        region*                                 my_region;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    // each region's audio boxes, one per object in the root
    class region_audio_box_set {

    public:
        void* self; // the resource handler points this at the set itself

        region*                     my_region;
        static_colgeom_root         obb_root;
        mash::vector<audio_box>     audio_boxes;
        region_audio_box_set_unk_4c unk_4c;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (audio_box_events,                            0x24);
    ASSERT_OFFSETOF(audio_box_events, low_altitude_events,       0x0C);
    ASSERT_OFFSETOF(audio_box_events, high_altitude_stop_events, 0x18);
    ASSERT_OFFSETOF(audio_box_events, unk_20,                    0x20);

    ASSERT_SIZEOF  (audio_box,         0x2C);
    ASSERT_OFFSETOF(audio_box, events, 0x04);
    ASSERT_OFFSETOF(audio_box, node,   0x28);

    ASSERT_SIZEOF  (region_audio_box_set_unk_4c_entry,         0x60);
    ASSERT_OFFSETOF(region_audio_box_set_unk_4c_entry, event,  0x10);
    ASSERT_OFFSETOF(region_audio_box_set_unk_4c_entry, unk_58, 0x58);

    ASSERT_SIZEOF  (region_audio_box_set_unk_4c,            0x18);
    ASSERT_OFFSETOF(region_audio_box_set_unk_4c, my_region, 0x14);

    ASSERT_SIZEOF  (region_audio_box_set,              0x64);
    ASSERT_OFFSETOF(region_audio_box_set, my_region,   0x04);
    ASSERT_OFFSETOF(region_audio_box_set, obb_root,    0x08);
    ASSERT_OFFSETOF(region_audio_box_set, audio_boxes, 0x38);
    ASSERT_OFFSETOF(region_audio_box_set, unk_4c,      0x4C);
} // treyarch
