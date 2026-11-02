#pragma once

#include <new>

#include "vc8/vector.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/string_hash.hh"
#include "game/singleton.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class active_spawn_item;
    struct quest_giver_spawn;
    struct spawn_table_unk_68;
    struct spawn_table_unk_68_entry;

    // one pack the region pack manager loaded; built and destroyed by retail
    struct held_pack {
        string_hash                     unk_00;
        mash::string                    pack_name;
        mash::string                    unk_10;
        vc8::vector<active_spawn_item*> unk_1c;
        i32                             sub_pool;

        mash::string* get_pack_name(mash::string* result) { // a copy of the name
            return new (result) mash::string(pack_name);
        }
    };

    // only the name is known
    struct region_pack_manager_unk_094 {
        u8           reserved_00[0x0C];
        mash::string pack_name;
    };

    // created through sub_93D700 by region_spawn_manager's constructor (sub_951A90)
    class region_pack_manager : public singleton<region_pack_manager, 0x010FB18C> {

    public:
        u8                           reserved_004[0x04];
        u32                          unk_008;                       // one more for every held pack built
        vc8::vector<held_pack*>      unk_00c;
        vc8::vector<held_pack*>      unk_01c;
        vc8::vector<held_pack*>      unk_02c;
        vc8::vector<held_pack*>      unk_03c;
        vc8::vector<held_pack*>      released_packs;                // freed one a frame
        i32*                         unk_05c;                       // a sub-pool count, then the sub-pools
        u8                           reserved_060[0x10];
        i32                          unk_070;                       // the sub-pool of unk_08c's packs
        i32                          unk_074;                       // the sub-pool of unk_090's packs
        u8                           reserved_078[0x08];
        spawn_table_unk_68*          unk_080;
        quest_giver_spawn*           held_quest_giver_spawn;        // its pack is loaded
        quest_giver_spawn*           next_quest_giver_spawn;        // takes over once its pack is in
        spawn_table_unk_68*          unk_08c;
        spawn_table_unk_68*          unk_090;
        region_pack_manager_unk_094* unk_094;
        region_pack_manager_unk_094* unk_098;
        i32                          state;
        u8                           reserved_0a0[0x0C];
        bool                         unk_0ac;                       // only releases, layouts and swaps start while set
        bool                         selection_load_requested;
        bool                         release_requested;
        bool                         layout_requested;
        bool                         held_load_requested;
        bool                         unk_0b1;
        bool                         swap_requested;
        bool                         quest_giver_locked_in_mission;
        bool                         unk_0b4;
        u8                           pad_0b5[0x03];

        void despawn_and_lock_quest_giver_in_mission(bool lock);

        bool hold_quest_giver_spawn(quest_giver_spawn* spawn);

        void frame_advance(f32 dt);

        void load_selection_pack(spawn_table_unk_68_entry* entry, i32 sub_pool);
    };

    namespace references {
        inline util::memory_reference<bool> unk_010fb37c { 0x010FB37C };
        inline util::memory_reference<bool> unk_010fb37d { 0x010FB37D };
    } // references

    ASSERT_SIZEOF  (held_pack,           0x30);
    ASSERT_OFFSETOF(held_pack, pack_name, 0x04);
    ASSERT_OFFSETOF(held_pack, unk_10,    0x10);
    ASSERT_OFFSETOF(held_pack, unk_1c,    0x1C);
    ASSERT_OFFSETOF(held_pack, sub_pool,  0x2C);

    ASSERT_OFFSETOF(region_pack_manager_unk_094, pack_name, 0x0C);

    ASSERT_SIZEOF  (region_pack_manager,                                0xB8);
    ASSERT_OFFSETOF(region_pack_manager, unk_008,                       0x08);
    ASSERT_OFFSETOF(region_pack_manager, unk_00c,                       0x0C);
    ASSERT_OFFSETOF(region_pack_manager, unk_02c,                       0x2C);
    ASSERT_OFFSETOF(region_pack_manager, unk_03c,                       0x3C);
    ASSERT_OFFSETOF(region_pack_manager, released_packs,                0x4C);
    ASSERT_OFFSETOF(region_pack_manager, unk_05c,                       0x5C);
    ASSERT_OFFSETOF(region_pack_manager, unk_070,                       0x70);
    ASSERT_OFFSETOF(region_pack_manager, unk_080,                       0x80);
    ASSERT_OFFSETOF(region_pack_manager, held_quest_giver_spawn,        0x84);
    ASSERT_OFFSETOF(region_pack_manager, next_quest_giver_spawn,        0x88);
    ASSERT_OFFSETOF(region_pack_manager, unk_08c,                       0x8C);
    ASSERT_OFFSETOF(region_pack_manager, unk_094,                       0x94);
    ASSERT_OFFSETOF(region_pack_manager, state,                         0x9C);
    ASSERT_OFFSETOF(region_pack_manager, unk_0ac,                       0xAC);
    ASSERT_OFFSETOF(region_pack_manager, swap_requested,                0xB2);
    ASSERT_OFFSETOF(region_pack_manager, quest_giver_locked_in_mission, 0xB3);
    ASSERT_OFFSETOF(region_pack_manager, unk_0b4,                       0xB4);
} // treyarch
