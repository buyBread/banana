#pragma once

#include "game/arch_base.hh"
#include "game/wds/entity/interface/combo_interface.hh"
#include "game/wds/entity/interface/damage_interface.hh"
#include "game/wds/entity/interface/physical_interface.hh"
#include "game/wds/entity/interface/player_interface.hh"
#include "game/wds/entity/interface/script_data_interface.hh"
#include "game/wds/entity/interface/switch_obj_interface.hh"
#include "game/wds/entity/interface/time_interface.hh"
#include "game/wds/entity/interface/towing_interface.hh"
#include "game/wds/entity/interface/variant_interface.hh"
#include "game/wds/entity/interface_storage.hh"
#include "game/container/fixed_vector.hh"
#include "game/color.hh"
#include "treyarch/mash/string_hash.hh"
#include "game/math/po.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class collision_geometry;
    class terrain;
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

    // SM3's EFLAG_*, but 0x800000 and 0x20000000
    enum e_entity_flags : u32 {
        entity_flag_moving            = 0x00000040,
        entity_flag_walkable          = 0x00000080,
        entity_flag_graphics_visible  = 0x00000200,
        entity_flag_special_target    = 0x00001000,
        entity_flag_collisions_active = 0x00004000,
        entity_flag_member            = 0x00008000,
        entity_flag_nontarget         = 0x00020000,
        entity_flag_beamable          = 0x00080000,
        entity_flag_detectable        = 0x00800000, // SM3's GLOWABLE
        entity_flag_allow_lockon      = 0x20000000, // SM3's COLGEOM_INSTANCED
        entity_flag_suspended         = 0x40000000
    };

    // SM3's EFLAG_EXT_*, but 0x10000
    enum e_entity_ext_flags : u32 {
        entity_ext_flag_spawned                = 0x00000100,
        entity_ext_flag_is_auto_activate_prop  = 0x00000800,
        entity_ext_flag_invulnerable           = 0x00004000,
        entity_ext_flag_noncrawlable           = 0x00008000,
        entity_ext_flag_shadow_casting         = 0x00010000, // SM3's AI_COVER
        entity_ext_flag_sync_anim              = 0x00020000,
        entity_ext_flag_ai_los_see_through     = 0x00100000,
        entity_ext_flag_attack_damage_unlocked = 0x20000000
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

        script_data_interface* script_data_ifc() const {
            return (script_data_interface*)my_ifc_storage->get_ifc(entity_ifc_script_data);
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

        // slot 68
        collision_geometry* get_colgeom() const {
            return ((collision_geometry* (__thiscall*)(const entity*))vtable[68])(this);
        }

        // slot 69; SM3's EFLAG_PHYSICS_COLLISIONS_ACTIVE (0x4000)
        void set_collisions_active(bool active, bool recursive) {
            ((void (__thiscall*)(entity*, bool, bool))vtable[69])(this, active, recursive);
        }

        // slot 70
        bool are_character_collisions_active() const {
            return ((bool (__thiscall*)(const entity*))vtable[70])(this);
        }

        // slot 72
        void set_character_collisions_active(bool active) {
            ((void (__thiscall*)(entity*, bool))vtable[72])(this, active);
        }

        // slot 75
        bool was_occluded_last_frame() {
            return ((bool (__thiscall*)(entity*))vtable[75])(this);
        }

        // slot 76
        bool is_alive() const {
            return ((bool (__thiscall*)(const entity*))vtable[76])(this);
        }

        // slot 77
        e_entity_flavor get_flavor() const {
            return ((e_entity_flavor (__thiscall*)(const entity*))vtable[77])(this);
        }

        // slot 78
        bool is_a_placeholder_vehicle() const {
            return ((bool (__thiscall*)(const entity*))vtable[78])(this);
        }

        // slot 89
        void compute_sector(terrain* ter, bool use_high_res_intersect, entity* original) {
            ((void (__thiscall*)(entity*, terrain*, bool, entity*))vtable[89])(this, ter, use_high_res_intersect, original);
        }

        // slot 91
        void force_current_region() {
            ((void (__thiscall*)(entity*))vtable[91])(this);
        }

        // slot 92
        void unforce_regions() {
            ((void (__thiscall*)(entity*))vtable[92])(this);
        }

        // slot 93
        void force_regions(entity* e) {
            ((void (__thiscall*)(entity*, entity*))vtable[93])(this, e);
        }

        // slot 113
        f32 get_visual_radius() const {
            return ((f32 (__thiscall*)(const entity*))vtable[113])(this);
        }

        // slot 114
        vector3d get_visual_center() const {
            vector3d center;
            ((vector3d* (__thiscall*)(const entity*, vector3d*))vtable[114])(this, &center);

            return center;
        }

        // slot 116
        void suspend(bool unk) {
            ((void (__thiscall*)(entity*, bool))vtable[116])(this, unk);
        }

        // slot 117
        void unsuspend(bool unk) {
            ((void (__thiscall*)(entity*, bool))vtable[117])(this, unk);
        }

        // slot 118
        void set_render_color(color32 color) {
            ((void (__thiscall*)(entity*, color32))vtable[118])(this, color);
        }

        // slot 119
        color32 get_render_color() const {
            color32 color;
            ((color32* (__thiscall*)(const entity*, color32*))vtable[119])(this, &color);

            return color;
        }

        // slot 122
        void set_render_scale(const vector3d &scale) {
            ((void (__thiscall*)(entity*, const vector3d*))vtable[122])(this, &scale);
        }

        // slot 123
        vector3d get_render_scale() const {
            vector3d scale;
            ((vector3d* (__thiscall*)(const entity*, vector3d*))vtable[123])(this, &scale);

            return scale;
        }

        // slot 134
        bool hates_spiderman() {
            return ((bool (__thiscall*)(entity*))vtable[134])(this);
        }

        // the milestone's entity_base::create_<name>_ifc(); an existing one is answered as is
        script_data_interface* create_script_data_ifc();
        physical_interface*    create_physical_ifc();
        damage_interface*      create_damage_ifc();
        time_interface*        create_time_ifc();

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
