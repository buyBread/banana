#pragma once

#include "game/arch_base.hh"
#include "game/wds/entity/interface/combo_interface.hh"
#include "game/wds/entity/interface/damage_interface.hh"
#include "game/wds/entity/interface/physical_interface.hh"
#include "game/wds/entity/interface/player_interface.hh"
#include "game/wds/entity/interface/switch_obj_interface.hh"
#include "game/wds/entity/interface/time_interface.hh"
#include "game/wds/entity/interface/towing_interface.hh"
#include "game/wds/entity/interface/variant_interface.hh"
#include "game/wds/entity/interface_storage.hh"
#include "game/container/fixed_vector.hh"
#include "treyarch/mash/string_hash.hh"
#include "game/math/po.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    struct region;

    // SM3's entflavors.h order
    enum e_entity_flavor : i32 {
        entity_flavor_actor,
        entity_flavor_camera,
        entity_flavor_base,
        entity_flavor_entity,
        entity_flavor_marker,
        entity_flavor_mic,
        entity_flavor_light_source,
        entity_flavor_particle_generator,
        entity_flavor_item,
        entity_flavor_conglomerate,
        entity_flavor_conglomerate_clone
    };

    // SM3's EFLAG_*
    enum e_entity_flags : u32 {
        entity_flag_graphics_visible  = 0x00000200,
        entity_flag_collisions_active = 0x00004000,
        entity_flag_nontarget         = 0x00020000,
        entity_flag_beamable          = 0x00080000,
        entity_flag_suspended         = 0x40000000
    };

    // SM3's EFLAG_EXT_*, but 0x10000
    enum e_entity_ext_flags : u32 {
        entity_ext_flag_is_auto_activate_prop = 0x00000800,
        entity_ext_flag_invulnerable          = 0x00004000,
        entity_ext_flag_shadow_casting        = 0x00010000, // SM3's AI_COVER
        entity_ext_flag_sync_anim             = 0x00020000,
        entity_ext_flag_ai_los_see_through    = 0x00100000
    };

    class entity : public arch_base {

    public:
        string_hash                         id;
        po*                                 my_rel_po;
        po*                                 my_abs_po;
        u32                                 flags;
        u32                                 ext_flags;
        interface_storage*                  my_ifc_storage;
        entity*                             my_parent;
        u8                                  reserved_024[0x16];
        u8                                  unk_03a; // cleared as scene entities are loaded
        u8                                  reserved_03b[0x15];
        container::fixed_vector<region*, 8> regions;

        // inlined; the milestone's has_<name>_ifc()
        bool has_ifc(e_entity_ifc index) const {
            return my_ifc_storage && my_ifc_storage->has_ifc(index);
        }

        // inlined; the milestone's entity_base::<name>_ifc()
        damage_interface* damage_ifc() const {
            return (damage_interface*)my_ifc_storage->get_ifc(entity_ifc_damage);
        }

        physical_interface* physical_ifc() const {
            return (physical_interface*)my_ifc_storage->get_ifc(entity_ifc_physical);
        }

        time_interface* time_ifc() const {
            return (time_interface*)my_ifc_storage->get_ifc(entity_ifc_time);
        }

        player_interface* player_ifc() const {
            return (player_interface*)my_ifc_storage->get_ifc(entity_ifc_player);
        }

        combo_interface* combo_ifc() const {
            return (combo_interface*)my_ifc_storage->get_ifc(entity_ifc_combo);
        }

        towing_interface* towing_ifc() const {
            return (towing_interface*)my_ifc_storage->get_ifc(entity_ifc_towing);
        }

        switch_obj_interface* switch_obj_ifc() const {
            return (switch_obj_interface*)my_ifc_storage->get_ifc(entity_ifc_switch_obj);
        }

        variant_interface* variant_ifc() const {
            return (variant_interface*)my_ifc_storage->get_ifc(entity_ifc_variant);
        }

        simple_variant_interface* simple_variant_ifc() const {
            return (simple_variant_interface*)my_ifc_storage->get_ifc(entity_ifc_simple_variant);
        }

        // unless ext flag 0x1000 vetoes it
        bool is_visible() const {
            return (flags & entity_flag_graphics_visible) && !(ext_flags & 0x1000);
        }

        // slot 77
        e_entity_flavor get_flavor() const {
            return ((e_entity_flavor (__thiscall*)(const entity*))vtable[77])(this);
        }

        // slot 64; this and its members
        void set_flag_recursive(u32 flag, bool on) {
            ((void (__thiscall*)(entity*, u32, bool))vtable[64])(this, flag, on);
        }

        // slot 65
        void set_ext_flag_recursive(u32 flag, bool on) {
            ((void (__thiscall*)(entity*, u32, bool))vtable[65])(this, flag, on);
        }

        // slot 67
        void set_visible(bool visible, bool unk) {
            ((void (__thiscall*)(entity*, bool, bool))vtable[67])(this, visible, unk);
        }

        // slot 69; SM3's EFLAG_PHYSICS_COLLISIONS_ACTIVE (0x4000)
        void set_collisions_active(bool active, bool recursive) {
            ((void (__thiscall*)(entity*, bool, bool))vtable[69])(this, active, recursive);
        }

        // slot 75
        bool was_occluded_last_frame() {
            return ((bool (__thiscall*)(entity*))vtable[75])(this);
        }

        // slot 116
        void suspend(bool unk) {
            ((void (__thiscall*)(entity*, bool))vtable[116])(this, unk);
        }

        // slot 117
        void unsuspend(bool unk) {
            ((void (__thiscall*)(entity*, bool))vtable[117])(this, unk);
        }

        void invoke_render_phase();

        generic_interface* find_ifc_by_name(string_hash name);

        region* get_primary_region();
    };

    ASSERT_OFFSETOF(entity, id,             0x08);
    ASSERT_OFFSETOF(entity, my_rel_po,      0x0C);
    ASSERT_OFFSETOF(entity, my_abs_po,      0x10);
    ASSERT_OFFSETOF(entity, flags,          0x14);
    ASSERT_OFFSETOF(entity, ext_flags,      0x18);
    ASSERT_OFFSETOF(entity, my_ifc_storage, 0x1C);
    ASSERT_OFFSETOF(entity, my_parent,      0x20);
    ASSERT_OFFSETOF(entity, unk_03a,        0x3A);
    ASSERT_OFFSETOF(entity, regions,        0x50);
} // treyarch
