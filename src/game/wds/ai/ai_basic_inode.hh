#pragma once

#include "game/wds/ai/ai_root_inode.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class arch_base;

    // every actor's; its targets, patrol, tethers and machine overrides stay native
    class ai_basic_inode : public ai_root_inode {};

    ai_basic_inode* get_ai_basic_inode(arch_base* the_entity);

    // what the entity rvb natives run on an actor's ai_basic_inode; each does nothing without one
    void rvb_reset_behavior(arch_base* the_entity);
    void rvb_set_var       (arch_base* the_entity, const string_hash* name, f32 value);
    f32  rvb_get_var       (arch_base* the_entity, const string_hash* name);
    void rvb_set_flag      (arch_base* the_entity, const string_hash* name, bool value);
    bool rvb_get_flag      (arch_base* the_entity, const string_hash* name);
} // treyarch
