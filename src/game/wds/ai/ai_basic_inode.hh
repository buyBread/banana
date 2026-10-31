#pragma once

#include "game/wds/ai/ai_root_inode.hh"
#include "game/wds/ai/ai_simple_target.hh"
#include "game/arch_base_vhandle.hh"
#include "game/hash/algo.hh"
#include "game/math/algebra/vector3d.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace slim {
        class slim_machine;
    } // slim

    class arch_base;

    inline const string_hash actual_car_seat_key      = string_hash(hash::djb2("actual_car_seat"));
    inline const string_hash spread_factor_key        = string_hash(hash::djb2("spread_factor"));
    inline const string_hash target_offset_vector_key = string_hash(hash::djb2("target_offset_vector"));
    inline const string_hash base_damage_scalar_key   = string_hash(hash::djb2("base_damage_scalar"));

    namespace references {
        // made on first use from the world's hero
        inline util::memory_reference<ai_simple_target_entity*> player_target { 0x00FC5EC0 };
    } // references

    // every actor's; its patrol and tethers stay native
    class ai_basic_inode : public ai_root_inode {

    public:
        enum : u32 {
            target_count       = 24,
            collection_slot    = 24,         // the target collection answers to it
            self_target_slot   = 0xFFFFFFFE,
            player_target_slot = 0xFFFFFFFF,
            machine_count      = 27
        };

        u8                           reserved_068[0x6C];
        slim::slim_machine*          machines[machine_count];
        u8                           machine_changed[machine_count]; // bit 0 is set by every store
        u8                           pad_15b;
        ai_simple_target*            targets[target_count];
        ai_simple_target_collection* target_collection;
        ai_simple_target*            self_target;                    // made on first use, dropped once the actor dies
        u8                           reserved_1c4[0x44];

        e_simple_target_type get_target_type(u32 slot); // drops a none target first
        void                 clear_target(i32 slot);
        ai_simple_target*    get_target(u32 slot);
        arch_base*           get_target_entity(u32 slot);
        bool                 is_target_valid(u32 slot); // an entity target that's an actor
        void                 set_target_entity(u32 slot, arch_base* target);
        void                 set_target_location(u32 slot, vector3d location);
        ai_simple_target*    get_self_target();

        static ai_simple_target_entity* get_player_target();

        void set_machine(u32 slot, slim::slim_machine* machine);
        void set_machine(u32 slot, const char* name);

        f32      get_actual_car_seat() const;
        f32      get_spread_factor() const;
        vector3d get_target_offset_vector() const;
        f32      get_base_damage_scalar() const;
    };

    ASSERT_SIZEOF  (ai_basic_inode,                    0x208);
    ASSERT_OFFSETOF(ai_basic_inode, reserved_068,      0x068);
    ASSERT_OFFSETOF(ai_basic_inode, machines,          0x0D4);
    ASSERT_OFFSETOF(ai_basic_inode, machine_changed,   0x140);
    ASSERT_OFFSETOF(ai_basic_inode, targets,           0x15C);
    ASSERT_OFFSETOF(ai_basic_inode, target_collection, 0x1BC);
    ASSERT_OFFSETOF(ai_basic_inode, self_target,       0x1C0);

    ai_basic_inode* get_ai_basic_inode(arch_base* the_entity);
    ai_basic_inode* get_ai_basic_inode_if_slim_actor(arch_base* the_entity);

    // what the entity rvb natives run on an actor's ai_basic_inode; each does nothing without one
    void                 rvb_reset_behavior(arch_base* the_entity);
    void                 rvb_set_var       (arch_base* the_entity, const string_hash* name, f32 value);
    f32                  rvb_get_var       (arch_base* the_entity, const string_hash* name);
    void                 rvb_set_flag      (arch_base* the_entity, const string_hash* name, bool value);
    bool                 rvb_get_flag      (arch_base* the_entity, const string_hash* name);
    void                 rvb_set_target    (arch_base* the_entity, u32 slot, arch_base* target);
    void                 rvb_set_target    (arch_base* the_entity, u32 slot, vector3d location); // slots 2 and 16 take no location
    void                 rvb_clear_target  (arch_base* the_entity, i32 slot);
    e_simple_target_type rvb_has_target    (arch_base* the_entity, u32 slot);
    void                 rvb_set_machine   (arch_base* the_entity, u32 slot, const char* name);  // slim actors only
    void                 rvb_clear_machine (arch_base* the_entity, u32 slot);                    // slim actors only

    void rvb_set_needs_repathfind(arch_base* the_entity); // a swift layer in use repaths

    i32 get_car_seat(arch_base* the_entity); // -1 outside a car

    void              rvb_set_speed_modifier       (arch_base* the_entity, f32 modifier); // vehicles only
    bool              rvb_is_crawling              (arch_base* the_entity);
    arch_base_vhandle rvb_get_animation_coordinator(arch_base* the_entity);              // a null handle without one
} // treyarch
