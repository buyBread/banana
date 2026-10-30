#pragma once

#include "vc8/vector.hh"
#include "game/singleton.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class cross_neighborhood_spawn;

    // built by the constructor (sub_94D260)
    struct spawn_table_manager_unk_04c {
        i32                                    max_active_spawn_items;
        u8                                     reserved_004[0xCC];
        vc8::vector<cross_neighborhood_spawn*> cross_neighborhood_spawns;
        u8                                     reserved_0e0[0x20];
        vc8::vector<cross_neighborhood_spawn*> unk_100;
        u8                                     reserved_110[0x34];
        bool                                   stars_mode;
        u8                                     reserved_145[0x53];
    };

    class spawn_table_manager : public singleton<spawn_table_manager, 0x010FB2B4> {

    public:
        u8                           reserved_004[0x04];
        f32                          spawn_placement_figures[4];
        bool                         street_percent_override;
        u8                           pad_019[0x03];
        f32                          street_percent;
        u8                           reserved_020[0x2A];
        bool                         collectibles_disabled;
        u8                           pad_04b;
        spawn_table_manager_unk_04c* unk_04c;
        u8                           reserved_050[0x04];
    };

    ASSERT_SIZEOF  (spawn_table_manager_unk_04c,                            0x198);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, cross_neighborhood_spawns, 0xD0);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, unk_100,                   0x100);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, stars_mode,                0x144);

    ASSERT_SIZEOF  (spawn_table_manager,                          0x54);
    ASSERT_OFFSETOF(spawn_table_manager, spawn_placement_figures, 0x08);
    ASSERT_OFFSETOF(spawn_table_manager, street_percent_override, 0x18);
    ASSERT_OFFSETOF(spawn_table_manager, street_percent,          0x1C);
    ASSERT_OFFSETOF(spawn_table_manager, collectibles_disabled,   0x4A);
    ASSERT_OFFSETOF(spawn_table_manager, unk_04c,                 0x4C);
} // treyarch
