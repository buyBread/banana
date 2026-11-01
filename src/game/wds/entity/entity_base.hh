#pragma once

#include "game/arch_base.hh"
#include "game/wds/entity/interface/card_interface.hh"
#include "game/wds/entity/interface/combo_interface.hh"
#include "game/wds/entity/interface/damage_interface.hh"
#include "game/wds/entity/interface/ise_interface.hh"
#include "game/wds/entity/interface/manip_obj_interface.hh"
#include "game/wds/entity/interface/occupant_interface.hh"
#include "game/wds/entity/interface/physical_interface.hh"
#include "game/wds/entity/interface/player_interface.hh"
#include "game/wds/entity/interface/script_data_interface.hh"
#include "game/wds/entity/interface/standing_interface.hh"
#include "game/wds/entity/interface/switch_obj_interface.hh"
#include "game/wds/entity/interface/tentacle_interface.hh"
#include "game/wds/entity/interface/time_interface.hh"
#include "game/wds/entity/interface/towing_interface.hh"
#include "game/wds/entity/interface/variant_interface.hh"
#include "game/wds/entity/interface_storage.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "game/math/po.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class collision_geometry;
    class entity;
    class info_node;

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
        entity_flag_in_limbo              = 0x00000001,
        entity_flag_moving                = 0x00000040,
        entity_flag_walkable              = 0x00000080,
        entity_flag_graphics_visible      = 0x00000200,
        entity_flag_special_target        = 0x00001000,
        entity_flag_collisions_active     = 0x00004000,
        entity_flag_member                = 0x00008000,
        entity_flag_nontarget             = 0x00020000,
        entity_flag_beamable              = 0x00080000,
        entity_flag_externally_controlled = 0x00400000,
        entity_flag_detectable            = 0x00800000, // SM3's GLOWABLE
        entity_flag_bone_connector        = 0x08000000,
        entity_flag_allow_lockon          = 0x20000000, // SM3's COLGEOM_INSTANCED
        entity_flag_suspended             = 0x40000000
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

    class entity_base : public arch_base {

    public:
        string_hash        id;
        po*                my_rel_po;
        po*                my_abs_po;
        u32                flags;
        u32                ext_flags;
        interface_storage* my_ifc_storage;
        entity*            my_parent;
        u8                 reserved_024[0x0C];
        u32                unk_030; // the transform waits report a move (add_moved) only while it's set
        u8                 reserved_034[0x06];
        u8                 unk_03a; // cleared as scene entities are loaded
        u8                 reserved_03b[0x15];

        explicit entity_base(mash::generating_vtable_version so_do_nothing) : arch_base(so_do_nothing) {}

        ~entity_base() override;

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool is_an_entity_base() const override;

        virtual vector3d            get_last_position() const;
        virtual i32                 po_changed();
        virtual i32                 unk_055(i32 unk, i32 unk_2, i32 unk_3, i32 unk_4, bool unk_5, bool unk_6, bool unk_7);
        virtual u32                 spawn_effect_at_entity_position(string_hash name, const vector4d &position, i32 unk); // the position is in the entity's space
        virtual u32                 spawn_effect(string_hash name, i32 unk);
        virtual u32                 spawn_effect_at_world_position(string_hash name, const vector4d &position, i32 unk);
        virtual u32                 unk_059(string_hash name, i32 unk);                                                   // slot 58's flags without a position
        virtual u32                 spawn_looping_effect_at_entity_position(string_hash name, const vector4d &position, i32 unk);
        virtual u32                 spawn_looping_effect(string_hash name, i32 unk);                                      // the id is what stop_looping_effect takes
        virtual u32                 unk_062(string_hash name, const vector4d &position, i32 unk);
        virtual void                stop_looping_effect(u32 effect_id);                                                   // only with an ise interface
        virtual void                set_flag_recursive(u32 flag, bool on);                                                // this and its members
        virtual void                set_ext_flag_recursive(u32 flag, bool on);
        virtual void                set_active(bool active);
        virtual void                set_visible(bool visible, bool from_conglom);
        virtual collision_geometry* get_colgeom() const;
        virtual void                set_collisions_active(bool active, bool recursive);                                   // SM3's EFLAG_PHYSICS_COLLISIONS_ACTIVE (0x4000)
        virtual bool                are_character_collisions_active() const;
        virtual bool                are_moving_terrain_only_collisions_active() const;
        virtual void                set_character_collisions_active(bool active);
        virtual void                set_terrain_collisions_active(bool active);
        virtual void                set_moving_terrain_only_collisions_active(bool active);
        virtual bool                was_occluded_last_frame();
        virtual bool                is_alive() const;
        virtual e_entity_flavor     get_flavor() const;
        virtual bool                is_a_placeholder_vehicle() const;
        virtual bool                get_ifc_num(const string_hash &name, f32* value, bool document);
        virtual bool                set_ifc_num(const string_hash &name, f32 value, bool document);
        virtual bool                get_ifc_vec(const string_hash &name, vector3d* value, bool document);
        virtual bool                set_ifc_vec(const string_hash &name, const vector3d &value, bool document);
        virtual bool                get_ifc_str(const string_hash &name, const char** value, bool document);
        virtual bool                set_ifc_str(const string_hash &name, const char* value, bool document);
        virtual info_node*          get_voice();
        virtual void                post_unmash_fixup(i32 unk);
        virtual u32                 get_entity_size() const;

        // inlined; the milestone's has_<name>_ifc()
        bool has_ifc(e_entity_ifc index) const {
            return my_ifc_storage && my_ifc_storage->has_ifc(index);
        }

        // sub_4024C0
        bool has_physical_ifc() const {
            return has_ifc(entity_ifc_physical);
        }

        // sub_4024E0
        bool has_standing_ifc() const {
            return has_ifc(entity_ifc_standing);
        }

        // sub_48E6E0
        bool has_occupant_ifc() const {
            return has_ifc(entity_ifc_occupant);
        }

        // sub_402540
        bool has_sound_ifc() const {
            return has_ifc(entity_ifc_sound);
        }

        // sub_48E6C0
        bool has_card_ifc() const {
            return has_ifc(entity_ifc_card);
        }

        // inlined; the milestone's entity_base::<name>_ifc()
        damage_interface* damage_ifc() const {
            return (damage_interface*)my_ifc_storage->get_ifc(entity_ifc_damage);
        }

        physical_interface* physical_ifc() const {
            return (physical_interface*)my_ifc_storage->get_ifc(entity_ifc_physical);
        }

        standing_interface* standing_ifc() const {
            return (standing_interface*)my_ifc_storage->get_ifc(entity_ifc_standing);
        }

        script_data_interface* script_data_ifc() const {
            return (script_data_interface*)my_ifc_storage->get_ifc(entity_ifc_script_data);
        }

        manip_obj_interface* manip_obj_ifc() const {
            return (manip_obj_interface*)my_ifc_storage->get_ifc(entity_ifc_manip_obj);
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

        ise_interface* ise_ifc() const {
            return (ise_interface*)my_ifc_storage->get_ifc(entity_ifc_ise);
        }

        tentacle_interface* tentacle_ifc() const {
            return (tentacle_interface*)my_ifc_storage->get_ifc(entity_ifc_tentacle);
        }

        occupant_interface* occupant_ifc() const {
            return (occupant_interface*)my_ifc_storage->get_ifc(entity_ifc_occupant);
        }

        card_interface* card_ifc() const {
            return (card_interface*)my_ifc_storage->get_ifc(entity_ifc_card);
        }

        // unless ext flag 0x1000 vetoes it
        bool is_visible() const {
            return (flags & entity_flag_graphics_visible) && !(ext_flags & 0x1000);
        }

        // the milestone's entity_base::create_<name>_ifc(); an existing one is answered as is
        script_data_interface* create_script_data_ifc();
        physical_interface*    create_physical_ifc();
        damage_interface*      create_damage_ifc();
        time_interface*        create_time_ifc();

        generic_interface* find_ifc_by_name(string_hash name);

    private:
        void finalize(mash::allocation_scope scope);
    };

    ASSERT_SIZEOF  (entity_base,                 0x50);
    ASSERT_OFFSETOF(entity_base, id,             0x08);
    ASSERT_OFFSETOF(entity_base, my_rel_po,      0x0C);
    ASSERT_OFFSETOF(entity_base, my_abs_po,      0x10);
    ASSERT_OFFSETOF(entity_base, flags,          0x14);
    ASSERT_OFFSETOF(entity_base, ext_flags,      0x18);
    ASSERT_OFFSETOF(entity_base, my_ifc_storage, 0x1C);
    ASSERT_OFFSETOF(entity_base, my_parent,      0x20);
    ASSERT_OFFSETOF(entity_base, unk_030,        0x30);
    ASSERT_OFFSETOF(entity_base, unk_03a,        0x3A);
} // treyarch
