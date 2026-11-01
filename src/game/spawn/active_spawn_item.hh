#pragma once

#include "vc8/map.hh"
#include "vc8/vector.hh"
#include "treyarch/mash/string.hh"
#include "game/arch_base_vhandle.hh"
#include "game/math/algebra.hh"
#include "game/spawn_point_list_collection.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace chuck { namespace vm {
        class script_instance;
        class vm_thread;

        template<typename T>
        class script_dynamic_array;
    }} // chuck::vm

    class entity;
    class static_spawn_item;

    // a spawn item being run; only ever built by retail, so every call goes through the retail vtables
    class active_spawn_item {

    public:
        spawn_point*                              record;
        static_spawn_item*                        item;
        chuck::vm::vm_thread*                     constructor_thread;
        chuck::vm::script_instance*               instance;           // made from the item's CITY_LIFE_OBJECT
        vc8::map<mash::string, arch_base_vhandle> important_entities;
        i32                                       spawn_id;           // what the script natives call a spawn's number
        bool                                      unk_24;             // set once the instance is made
        bool                                      quest_item;         // drawn from the quest items
        bool                                      unk_26;
        bool                                      forced_despawn;
        bool                                      unk_28;
        u8                                        pad_29[0x03];
        vector3d*                                 neighborhood_center;
        vector3d*                                 meta_neighborhood_center;

        virtual ~active_spawn_item();

        virtual bool ready_to_start() = 0;
        virtual bool unk_008(bool unk) = 0;
        virtual void unk_00c(u32 unk) = 0;
        virtual void get_entities(i32                   number,
                                  vc8::vector<entity*>* found,
                                  mash::string*         name) = 0;

        // sub_90F9F0
        virtual bool has_instance(chuck::vm::script_instance* inst) {
            return inst && instance == inst;
        }

        virtual void set_instance(chuck::vm::script_instance* inst);

        // sub_9518B0
        virtual vector3d get_spawn_position() {
            return record->position;
        }

        // sub_9518D0
        virtual void unk_020(bool value) {
            unk_26 = value;
        }

        // sub_9518E0
        virtual bool unk_024() {
            return unk_26;
        }

        // sub_90FA10
        virtual bool is_out_of_range(vector3d point, f32 limit_squared, bool) {
            vector3d position = record->position;

            // the height counts from 0, not from the record
            f32 dx = (f32)((f64)point.x - (f64)position.x);
            f32 dy = (f32)((f64)point.y - 0.0);
            f32 dz = (f32)((f64)point.z - (f64)position.z);

            f32 distance_squared = (f32)((f64)dx * dx + (f64)dy * dy + (f64)dz * dz);

            return distance_squared > limit_squared || forced_despawn;
        }

        virtual void update_point_of_interest(mash::string name, vector3d position);
        virtual void get_current_flagsets(chuck::vm::script_dynamic_array<u32>* flagsets) = 0;
    };

    ASSERT_SIZEOF  (active_spawn_item,                           0x34);
    ASSERT_OFFSETOF(active_spawn_item, record,                   0x04);
    ASSERT_OFFSETOF(active_spawn_item, instance,                 0x10);
    ASSERT_OFFSETOF(active_spawn_item, important_entities,       0x14);
    ASSERT_OFFSETOF(active_spawn_item, spawn_id,                 0x20);
    ASSERT_OFFSETOF(active_spawn_item, unk_24,                   0x24);
    ASSERT_OFFSETOF(active_spawn_item, forced_despawn,           0x27);
    ASSERT_OFFSETOF(active_spawn_item, unk_28,                   0x28);
    ASSERT_OFFSETOF(active_spawn_item, neighborhood_center,      0x2C);
    ASSERT_OFFSETOF(active_spawn_item, meta_neighborhood_center, 0x30);
} // treyarch
