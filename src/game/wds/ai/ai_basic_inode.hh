#pragma once

#include "game/wds/ai/ai_root_inode.hh"
#include "game/hash/algo.hh"
#include "game/math/algebra/vector3d.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class arch_base;

    inline const string_hash actual_car_seat_key      = string_hash(hash::djb2("actual_car_seat"));
    inline const string_hash spread_factor_key        = string_hash(hash::djb2("spread_factor"));
    inline const string_hash target_offset_vector_key = string_hash(hash::djb2("target_offset_vector"));
    inline const string_hash base_damage_scalar_key   = string_hash(hash::djb2("base_damage_scalar"));

    // every actor's; its targets, patrol, tethers and machine overrides stay native
    class ai_basic_inode : public ai_root_inode {

    public:
        f32      get_actual_car_seat() const;
        f32      get_spread_factor() const;
        vector3d get_target_offset_vector() const;
        f32      get_base_damage_scalar() const;
    };

    ai_basic_inode* get_ai_basic_inode(arch_base* the_entity);
    ai_basic_inode* get_ai_basic_inode_if_slim_actor(arch_base* the_entity);

    // what the entity rvb natives run on an actor's ai_basic_inode; each does nothing without one
    void rvb_reset_behavior(arch_base* the_entity);
    void rvb_set_var       (arch_base* the_entity, const string_hash* name, f32 value);
    f32  rvb_get_var       (arch_base* the_entity, const string_hash* name);
    void rvb_set_flag      (arch_base* the_entity, const string_hash* name, bool value);
    bool rvb_get_flag      (arch_base* the_entity, const string_hash* name);

    void rvb_set_needs_repathfind(arch_base* the_entity); // a swift layer in use repaths
} // treyarch
