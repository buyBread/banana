#pragma once

#include "vc8/vector.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "treyarch/mash/virtual_base.hh"
#include "game/spawn_point_list_collection.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace chuck { namespace vm {
        class script_executable;
        class script_object;
    }} // chuck::vm

    class active_spawn_item;
    class finger_of_god;

    // the base of every spawn item; mashed ones get their retail vtable from the type registry
    class static_spawn_item : public mash::mash_virtual_base {

    public:
        mash::string                  name;
        mash::vector
            <spawn_data_base>         data;
        bool                          unk_24;
        u8                            pad_25[0x03];
        mash::vector
            <mash::string>            categories;        // its city-life categories
        i32                           unk_3c;            // what spawn placement looks for
        u32                           active_count;
        bool                          unk_44;
        u8                            pad_45[0x03];
        chuck::vm::script_executable* script_executable;
        chuck::vm::script_object*     script_object;

        // sub_A90D90
        virtual active_spawn_item* create_active(u32) {
            return nullptr;
        }

        // sub_5BD2C0
        virtual bool unk_028(active_spawn_item*, u32) {
            return false;
        }

        virtual void unk_02c(active_spawn_item* active);

        // sub_71F870
        virtual bool is_scripted() { return false; }

        // sub_71F870
        virtual bool is_random() { return false; }

        // sub_71F870
        virtual bool is_always_on() { return false; }

        // sub_667F80
        virtual bool unk_03c() { return true; }

        // sub_71F870
        virtual bool unk_040() { return false; }

        // sub_71F870
        virtual bool unk_044() { return false; }

        // sub_71F870
        virtual bool is_cross_neighborhood() { return false; }

        // sub_71F870
        virtual bool is_friendly() { return false; }

        // sub_667F80
        virtual bool unk_050() { return true; }

        // sub_71FF10
        virtual bool tick(f32) { return false; }

        // sub_71F870
        virtual bool is_meta() { return false; }

        virtual bool find_spawn_record(spawn_point** record);
    };

    class scripted_spawn_item : public static_spawn_item {

    public:
        mash::string       script_name; // also the pack it needs
        mash::vector
            <mash::string> unk_5c;
        u32                spawn_limit; // how many may run at once, 0 for any number
    };

    class spawn_pointed_scripted_spawn_item : public scripted_spawn_item {

    public:
        mash::vector<mash::string> spawn_point_lists;
    };

    class extra_pack_optional_spawn_item : public spawn_pointed_scripted_spawn_item {

    public:
        mash::string pack;
    };

    // the run-time state of a cross-neighborhood spawn
    struct cross_neighborhood_spawn_unk_d8 {
        active_spawn_item*          active;
        vc8::vector<u32>            unk_04;
        vc8::vector<finger_of_god*> radar_markers;
        vc8::vector<u32>            zoom_map_markers;
        vc8::vector<u32>            unk_34;
        bool                        quest_event_raised;
        u8                          pad_45[0x03];
        i32                         safe_zone_wall_index;
    };

    class cross_neighborhood_spawn : public extra_pack_optional_spawn_item {

    public:
        u32                              unk_94; // an event callback id
        u8                               unk_98[0x0C];
        mash::vector
            <mash::vector<mash::string>> unk_a4;
        mash::string                     quest_event;
        mash::string                     unk_c4;
        u8                               unk_d0[0x05];
        bool                             unk_d5;
        u8                               pad_d6[0x02];
        cross_neighborhood_spawn_unk_d8* unk_d8;
    };

    class static_spawn_item_collection {

    public:
        void* self; // the resource handler points this at the collection itself

        mash::vector
            <static_spawn_item> spawn_items;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (static_spawn_item,                    0x50);
    ASSERT_OFFSETOF(static_spawn_item, data,              0x10);
    ASSERT_OFFSETOF(static_spawn_item, unk_24,            0x24);
    ASSERT_OFFSETOF(static_spawn_item, categories,        0x28);
    ASSERT_OFFSETOF(static_spawn_item, unk_3c,            0x3C);
    ASSERT_OFFSETOF(static_spawn_item, active_count,      0x40);
    ASSERT_OFFSETOF(static_spawn_item, unk_44,            0x44);
    ASSERT_OFFSETOF(static_spawn_item, script_executable, 0x48);
    ASSERT_OFFSETOF(static_spawn_item, script_object,     0x4C);

    ASSERT_SIZEOF  (scripted_spawn_item,              0x74);
    ASSERT_OFFSETOF(scripted_spawn_item, script_name, 0x50);
    ASSERT_OFFSETOF(scripted_spawn_item, unk_5c,      0x5C);
    ASSERT_OFFSETOF(scripted_spawn_item, spawn_limit, 0x70);

    ASSERT_SIZEOF  (spawn_pointed_scripted_spawn_item,                    0x88);
    ASSERT_OFFSETOF(spawn_pointed_scripted_spawn_item, spawn_point_lists, 0x74);

    ASSERT_SIZEOF  (extra_pack_optional_spawn_item,       0x94);
    ASSERT_OFFSETOF(extra_pack_optional_spawn_item, pack, 0x88);

    ASSERT_SIZEOF  (cross_neighborhood_spawn_unk_d8,                       0x4C);
    ASSERT_OFFSETOF(cross_neighborhood_spawn_unk_d8, unk_04,               0x04);
    ASSERT_OFFSETOF(cross_neighborhood_spawn_unk_d8, radar_markers,        0x14);
    ASSERT_OFFSETOF(cross_neighborhood_spawn_unk_d8, zoom_map_markers,     0x24);
    ASSERT_OFFSETOF(cross_neighborhood_spawn_unk_d8, unk_34,               0x34);
    ASSERT_OFFSETOF(cross_neighborhood_spawn_unk_d8, quest_event_raised,   0x44);
    ASSERT_OFFSETOF(cross_neighborhood_spawn_unk_d8, safe_zone_wall_index, 0x48);

    ASSERT_SIZEOF  (cross_neighborhood_spawn,              0xDC);
    ASSERT_OFFSETOF(cross_neighborhood_spawn, unk_94,      0x94);
    ASSERT_OFFSETOF(cross_neighborhood_spawn, unk_a4,      0xA4);
    ASSERT_OFFSETOF(cross_neighborhood_spawn, quest_event, 0xB8);
    ASSERT_OFFSETOF(cross_neighborhood_spawn, unk_c4,      0xC4);
    ASSERT_OFFSETOF(cross_neighborhood_spawn, unk_d5,      0xD5);
    ASSERT_OFFSETOF(cross_neighborhood_spawn, unk_d8,      0xD8);

    ASSERT_SIZEOF  (static_spawn_item_collection,              0x18);
    ASSERT_OFFSETOF(static_spawn_item_collection, spawn_items, 0x04);
} // treyarch
