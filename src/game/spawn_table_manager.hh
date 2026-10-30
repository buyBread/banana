#pragma once

#include "vc8/vector.hh"
#include "treyarch/mash/string.hh"
#include "game/math/algebra.hh"
#include "game/singleton.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class cross_neighborhood_spawn;

    // a script's saved value; the records are only ever made by retail, so these never run their own vtable
    class persistence_data_base {

    public:
        // sub_71F870
        virtual bool is_array() const { return false; }

        // sub_71F870
        virtual bool is_vector3d() const { return false; }

        // sub_71F870
        virtual bool is_str() const { return false; }

        // sub_71F870
        virtual bool is_num() const { return false; }
    };

    class persistence_num : public persistence_data_base {

    public:
        f32 value;
    };

    class persistence_str : public persistence_data_base {

    public:
        mash::string value;
    };

    class persistence_vector3d : public persistence_data_base {

    public:
        vector3d value;
    };

    class persistence_num_array : public persistence_data_base {

    public:
        vc8::vector<f32> values;
    };

    class persistence_str_array : public persistence_data_base {

    public:
        vc8::vector<mash::string> values;
    };

    class persistence_vector3d_array : public persistence_data_base {

    public:
        vc8::vector<vector3d> values;
    };

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

    ASSERT_SIZEOF  (persistence_num,        0x08);
    ASSERT_OFFSETOF(persistence_num, value, 0x04);

    ASSERT_SIZEOF  (persistence_str,        0x10);
    ASSERT_OFFSETOF(persistence_str, value, 0x04);

    ASSERT_SIZEOF  (persistence_vector3d,        0x10);
    ASSERT_OFFSETOF(persistence_vector3d, value, 0x04);

    ASSERT_SIZEOF  (persistence_num_array,         0x14);
    ASSERT_OFFSETOF(persistence_num_array, values, 0x04);

    ASSERT_SIZEOF  (persistence_str_array,         0x14);
    ASSERT_OFFSETOF(persistence_str_array, values, 0x04);

    ASSERT_SIZEOF  (persistence_vector3d_array,         0x14);
    ASSERT_OFFSETOF(persistence_vector3d_array, values, 0x04);

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
