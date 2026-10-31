#pragma once

#include "vc8/map.hh"
#include "vc8/vector.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/vector.hh"
#include "game/math/algebra.hh"
#include "game/spawn/persistence_data.hh"
#include "game/singleton.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace chuck { namespace vm {
        class script_instance;

        template<typename T>
        class script_dynamic_array;
    }} // chuck::vm

    class active_spawn_item;
    class cross_neighborhood_spawn;
    class entity;
    class no_spawn_base;
    class spawn_data_base;
    class spawn_item_collection;
    class spawn_table_interface;
    class static_spawn_item;
    struct city_life_category_entry;
    struct quest_giver_spawn;
    struct recent_despawn;
    struct spawn_item_collection_unk_38;
    struct spawn_item_collection_unk_4c;
    struct spawn_point;

    // every run-time list of city life; built by the constructor (sub_94D260)
    struct spawn_table_manager_unk_04c {
        i32 max_active_spawn_items;
        u32 quest_spawn_count;      // the quest items' share of spawn_count
        i32 unk_008;
        u32 city_spawn_count;       // the rest of spawn_count
        i32 unk_010;

        vc8::vector
            <quest_giver_spawn*>                      quest_giver_spawns;
        quest_giver_spawn*                            chosen_quest_giver_spawn;
        vc8::vector
            <active_spawn_item*>                      pending_active_spawn_items; // started once their slot 1 holds
        vc8::vector
            <active_spawn_item*>                      active_spawn_items;
        vc8::vector
            <recent_despawn*>                         recent_despawns;
        vc8::map
            <string_hash, persistence_data*>          saved_persistence_data;     // by script instance name, until the instance despawns
        vc8::map
            <active_spawn_item*, persistence_data*>   active_persistence_data;    // handed back to the next active of the same instance

        spawn_table_interface*                        table_interface;
        vc8::map
            <mash::string, no_spawn_base*>*           no_spawn_zones;
        vc8::map
            <mash::string, no_spawn_base*>*           arbitrary_regions;
        spawn_item_collection_unk_4c*                 no_spawn_zone_set;
        vc8::vector
            <static_spawn_item*>                      spawn_items;
        vc8::vector
            <static_spawn_item*>                      friendly_spawn_items;
        vc8::vector
            <static_spawn_item*>                      stars_mode_spawn_items;
        vc8::vector
            <static_spawn_item*>                      always_on_spawn_items;
        vc8::vector
            <static_spawn_item*>                      random_spawn_items;
        vc8::vector
            <cross_neighborhood_spawn*>               cross_neighborhood_spawns;
        vc8::vector
            <static_spawn_item*>                      quest_spawn_items;
        vc8::vector
            <static_spawn_item*>                      quest_always_on_spawn_items;
        vc8::vector
            <cross_neighborhood_spawn*>               quest_cross_neighborhood_spawns;

        spawn_item_collection_unk_38* pacing;
        spawn_item_collection_unk_38* mission_pacing_override;

        vc8::map
            <mash::string, vc8::vector<city_life_category_entry>>* city_life_categories;

        bool quest_spawn_uncapped;
        u8   pad_11d[0x03];
        i32  quest_spawn_cap;
        f32  unk_124;
        f32  unk_128;
        f32  quest_spawn_share;
        f32  unk_130;
        u32  spawn_count;
        u32  unk_138;
        i32  pacing_phase;           // 0 ramps up, 1 steps down, anything else holds
        f32  unk_140;                // seconds since LONG_TIME_LAST_CITY_LIFE was raised
        bool stars_mode;
        u8   pad_145[0x03];
        i32  stars_level;
        i32  stars_mode_effects[2];  // looping effects on the hero, -1 for none
        i32  unk_154;
        i32  unk_158;

        entity* fixed_entities[15];
    };

    class spawn_table_manager : public singleton<spawn_table_manager, 0x010FB2B4> {

    public:
        i32                          unk_004;
        f32                          spawn_placement_figures[4];
        bool                         street_percent_override;
        u8                           pad_019[0x03];
        f32                          street_percent;
        f32                          unk_020;
        vc8::vector
            <spawn_point*>           owned_spawn_records;    // heap records, freed with free_spawn_record
        vc8::vector
            <spawn_point*>           borrowed_spawn_records; // mashed spawn points, only forgotten
        u32                          unk_044;
        bool                         unk_048;
        bool                         unk_049;
        bool                         collectibles_disabled;
        u8                           pad_04b;
        spawn_table_manager_unk_04c* unk_04c;
        bool                         unk_050;
        u8                           pad_051[0x03];

        spawn_item_collection* find_neighborhood_set();
        spawn_item_collection* find_city_wide_set();
        spawn_item_collection* find_meta_set();

        void free_spawn_record(spawn_point* record);

        // the script queries; `inst` is the calling script instance, matched through each active's has_instance
        void             get_entities(chuck::vm::script_instance* inst,
                                      i32                         number,
                                      vc8::vector<entity*>*       found,
                                      mash::string*               name);
        vector3d*        get_spawn_position(vector3d* out, chuck::vm::script_instance* inst);
        spawn_data_base* find_optional_data(chuck::vm::script_instance* inst, mash::string name);
        bool             has_optional_data(chuck::vm::script_instance* inst);
        void             update_point_of_interest(chuck::vm::script_instance* inst, mash::string name, vector3d position);
        void             has_succeeded_city_life(chuck::vm::script_instance* inst);
        void             force_despawn_city_life(chuck::vm::script_instance* inst);
        bool             is_valid_spawn(i32 spawn_id);
        void             set_safe_zone_wall_index(chuck::vm::script_instance* inst, i32 index);
        i32              get_safe_zone_wall_index(chuck::vm::script_instance* inst);
        vector3d*        get_neighborhood_center(vector3d* out, chuck::vm::script_instance* inst);
        vector3d*        get_meta_neighborhood_center(vector3d* out, chuck::vm::script_instance* inst);
        void             get_current_flagsets(chuck::vm::script_instance* inst, chuck::vm::script_dynamic_array<u32>* flagsets);
        void             flag_always_on_for_despawn(chuck::vm::script_instance* inst);
        bool             has_persistence_data(chuck::vm::script_instance* inst);
        void             get_all_categories_on_city_life(i32 spawn_id, chuck::vm::script_dynamic_array<u32>* categories);
        void             get_spawn_point_point_data(chuck::vm::script_instance*                inst,
                                                    chuck::vm::script_dynamic_array<vector3d>* points);

        bool is_spidey_near_quest_giver(f32 radius);
        bool get_nearest_quest_giver_restart_position(vector3d* position, vector3d* facing);

    private:
        cross_neighborhood_spawn* find_cross_neighborhood_spawn(chuck::vm::script_instance* inst);
    };

    namespace references {
        // every active's spawn id comes from here
        inline util::memory_reference<i32> next_spawn_id { 0x010FB2F4 };

        // null until the first debris zone is added
        inline util::memory_reference<mash::vector<no_spawn_base>*> debris_no_spawn_zones { 0x010F9BDC };
    } // references

    ASSERT_SIZEOF  (spawn_table_manager_unk_04c,                                  0x198);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, quest_giver_spawns,              0x14);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, chosen_quest_giver_spawn,        0x24);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, pending_active_spawn_items,      0x28);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, active_spawn_items,              0x38);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, recent_despawns,                 0x48);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, saved_persistence_data,          0x58);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, active_persistence_data,         0x64);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, table_interface,                 0x70);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, no_spawn_zones,                  0x74);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, arbitrary_regions,               0x78);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, no_spawn_zone_set,               0x7C);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, spawn_items,                     0x80);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, friendly_spawn_items,            0x90);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, stars_mode_spawn_items,          0xA0);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, always_on_spawn_items,           0xB0);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, random_spawn_items,              0xC0);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, cross_neighborhood_spawns,       0xD0);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, quest_spawn_items,               0xE0);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, quest_always_on_spawn_items,     0xF0);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, quest_cross_neighborhood_spawns, 0x100);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, pacing,                          0x110);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, mission_pacing_override,         0x114);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, city_life_categories,            0x118);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, quest_spawn_uncapped,            0x11C);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, quest_spawn_cap,                 0x120);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, quest_spawn_share,               0x12C);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, spawn_count,                     0x134);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, pacing_phase,                    0x13C);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, stars_mode,                      0x144);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, stars_level,                     0x148);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, stars_mode_effects,              0x14C);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, unk_154,                         0x154);
    ASSERT_OFFSETOF(spawn_table_manager_unk_04c, fixed_entities,                  0x15C);

    ASSERT_SIZEOF  (spawn_table_manager,                          0x54);
    ASSERT_OFFSETOF(spawn_table_manager, unk_004,                 0x04);
    ASSERT_OFFSETOF(spawn_table_manager, spawn_placement_figures, 0x08);
    ASSERT_OFFSETOF(spawn_table_manager, street_percent_override, 0x18);
    ASSERT_OFFSETOF(spawn_table_manager, street_percent,          0x1C);
    ASSERT_OFFSETOF(spawn_table_manager, owned_spawn_records,     0x24);
    ASSERT_OFFSETOF(spawn_table_manager, borrowed_spawn_records,  0x34);
    ASSERT_OFFSETOF(spawn_table_manager, unk_044,                 0x44);
    ASSERT_OFFSETOF(spawn_table_manager, unk_049,                 0x49);
    ASSERT_OFFSETOF(spawn_table_manager, collectibles_disabled,   0x4A);
    ASSERT_OFFSETOF(spawn_table_manager, unk_04c,                 0x4C);
    ASSERT_OFFSETOF(spawn_table_manager, unk_050,                 0x50);
} // treyarch
