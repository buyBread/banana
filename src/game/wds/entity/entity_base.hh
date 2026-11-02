#pragma once

#include <array>

#include "game/arch_base.hh"
#include "game/global.hh"
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
#include "vc8/vector.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class actor;
    class collision_geometry;
    class conglomerate;
    class entity;
    class info_node;

    namespace amalga {
        struct resource_pack_slot;
    } // amalga

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
        entity_flag_in_limbo                             = 0x00000001,
        entity_flag_needs_collision_proximity_map_update = 0x00000004,
        entity_flag_physics_collide_characters           = 0x00000010,
        entity_flag_physics_collide_terrain              = 0x00000020,
        entity_flag_moving                               = 0x00000040,
        entity_flag_walkable                             = 0x00000080,
        entity_flag_graphics                             = 0x00000100,
        entity_flag_graphics_visible                     = 0x00000200,
        entity_flag_special_target                       = 0x00001000,
        entity_flag_active                               = 0x00002000,
        entity_flag_collisions_active                    = 0x00004000,
        entity_flag_member                               = 0x00008000,
        entity_flag_nontarget                            = 0x00020000,
        entity_flag_beamable                             = 0x00080000,
        entity_flag_unk_00200000                         = 0x00200000,
        entity_flag_externally_controlled                = 0x00400000,
        entity_flag_detectable                           = 0x00800000, // SM3's GLOWABLE
        entity_flag_update_regions_every_frame           = 0x02000000,
        entity_flag_bone_connector                       = 0x08000000,
        entity_flag_region_forced                        = 0x10000000,
        entity_flag_allow_lockon                         = 0x20000000, // SM3's COLGEOM_INSTANCED
        entity_flag_suspended                            = 0x40000000,
        entity_flag_member_hide                          = 0x80000000
    };

    // SM3's EFLAG_EXT_*, but 0x10000
    enum e_entity_ext_flags : u32 {
        entity_ext_flag_bounding_sphere_dirty           = 0x00000040,
        entity_ext_flag_spawned                         = 0x00000100,
        entity_ext_flag_is_hero                         = 0x00000200,
        entity_ext_flag_is_auto_activate_prop           = 0x00000800,
        entity_ext_flag_unk_00001000                    = 0x00001000,
        entity_ext_flag_invulnerable                    = 0x00004000,
        entity_ext_flag_noncrawlable                    = 0x00008000,
        entity_ext_flag_shadow_casting                  = 0x00010000, // SM3's AI_COVER
        entity_ext_flag_sync_anim                       = 0x00020000,
        entity_ext_flag_ai_los_see_through              = 0x00100000,
        entity_ext_flag_unk_00200000                    = 0x00200000,
        entity_ext_flag_added_to_list_of_moved_entities = 0x00800000,
        entity_ext_flag_alloc_adv_ptrs                  = 0x02000000,
        entity_ext_flag_attack_damage_unlocked          = 0x20000000,
        entity_ext_flag_does_not_manage_abs_po          = 0x40000000
    };

    struct fade_range_t {
        f32 fade_radius_threshold;
        f32 fade_min_dist;
        f32 fade_max_dist;
        f32 fade_min_dist_squared;
        f32 fade_max_dist_squared;
        f32 inverse_length;
    };

    ASSERT_SIZEOF(fade_range_t, 0x18);

    namespace references {
        inline util::memory_reference<std::array<fade_range_t, 16>> enx_ranges { 0x00E7B670 };
    } // references

    class entity_base : public arch_base {

    public:
        string_hash                id;
        po*                        my_rel_po;
        po*                        my_abs_po;
        mutable u32                flags;
        mutable u32                ext_flags;
        interface_storage*         my_ifc_storage;
        entity_base*               my_parent;
        entity_base*               my_child;
        entity_base*               my_brother;
        vc8::vector<entity_base*>* adopted_children;
        arch_base_vhandle          my_trigger;
        u8                         reserved_034[0x06];
        u8                         fade_timer;
        u8                         clone_member_idx;
        u8                         reserved_03c[0x04];
        conglomerate*              my_conglom_root;
        u8                         bone_idx;
        u8                         reserved_045[0x07];
        string_hash                sound_type;

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
        virtual void                post_unmash_fixup(amalga::resource_pack_slot* pack_slot);
        virtual u32                 get_entity_size() const;

        // inlined; the milestone's has_<name>_ifc()
        bool has_ifc(e_entity_ifc index) const {
            return my_ifc_storage && my_ifc_storage->has_ifc(index);
        }

        // sub_4024A0
        // inlined @ sub_614E90
        bool has_damage_ifc() const {
            return has_ifc(entity_ifc_damage);
        }

        // sub_4024C0
        // inlined @ sub_614190
        bool has_physical_ifc() const {
            return has_ifc(entity_ifc_physical);
        }

        // sub_4024E0
        // inlined @ sub_57D110
        // inlined @ sub_614190
        bool has_standing_ifc() const {
            return has_ifc(entity_ifc_standing);
        }

        // sub_408CF0
        bool has_script_data_ifc() const {
            return has_ifc(entity_ifc_script_data);
        }

        // sub_408D10
        bool has_manip_obj_ifc() const {
            return has_ifc(entity_ifc_manip_obj);
        }

        // sub_44E680
        bool has_switch_obj_ifc() const {
            return has_ifc(entity_ifc_switch_obj);
        }

        // sub_4E1CE0
        bool has_thrown_weapon_ifc() const {
            return has_ifc(entity_ifc_thrown_weapon);
        }

        // sub_4E1D00
        bool has_gun_weapon_ifc() const {
            return has_ifc(entity_ifc_gun_weapon);
        }

        // sub_4E1D20
        bool has_melee_weapon_ifc() const {
            return has_ifc(entity_ifc_melee_weapon);
        }

        // sub_402500
        bool has_player_ifc() const {
            return has_ifc(entity_ifc_player);
        }

        // sub_5FCB20
        bool has_variant_ifc() const {
            return has_ifc(entity_ifc_variant);
        }

        // sub_5837A0
        bool has_simple_variant_ifc() const {
            return has_ifc(entity_ifc_simple_variant);
        }

        // sub_48E6C0
        bool has_card_ifc() const {
            return has_ifc(entity_ifc_card);
        }

        // sub_48E6E0
        bool has_occupant_ifc() const {
            return has_ifc(entity_ifc_occupant);
        }

        // sub_5A7FC0
        bool has_morph_ifc() const {
            return has_ifc(entity_ifc_morph);
        }

        // sub_5FCB40
        bool has_rpg_ifc() const {
            return has_ifc(entity_ifc_rpg);
        }

        // sub_402520
        bool has_combo_ifc() const {
            return has_ifc(entity_ifc_combo);
        }

        // sub_423D00
        bool has_ise_ifc() const {
            return has_ifc(entity_ifc_ise);
        }

        // sub_423D20
        bool has_tentacle_ifc() const {
            return has_ifc(entity_ifc_tentacle);
        }

        // sub_402540
        bool has_sound_ifc() const {
            return has_ifc(entity_ifc_sound);
        }

        // inlined; the milestone's entity_base::<name>_ifc()
        damage_interface* damage_ifc() const {
            return (damage_interface*)my_ifc_storage->get_ifc(entity_ifc_damage);
        }

        physical_interface* physical_ifc() const {
            return (physical_interface*)my_ifc_storage->get_ifc(entity_ifc_physical);
        }

        // sub_5B76B0
        physical_interface* find_physical_ifc() const {
            if (!my_ifc_storage)
                return nullptr;

            return (physical_interface*)my_ifc_storage->find_ifc(entity_ifc_physical);
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
            return (flags & entity_flag_graphics_visible) && !(ext_flags & entity_ext_flag_unk_00001000);
        }

        // sub_42AC00
        string_hash get_sound_type() const {
            return sound_type;
        }

        bool is_hero() const;
        void set_hero(bool hero);
        void set_invulnerable(bool invulnerable);

        void set_parent(entity_base* new_parent); // null clears
        void clear_parent();                      // a child missing from its parent's chain keeps its links

        conglomerate* get_conglom_owner() const; // a conglomerate is its own, a member asks its parent
        entity_base*  get_highest_parent() const;
        bool          is_a_parent(const entity_base* par) const;

        void update_abs_po_from_model_po();
        void update_child_abs_po();
        void update_attached_abs_pos();

        void set_rel_po(const po &the_po);
        void set_rel_position(const vector3d &p); // w is forced to 1
        void look_at(const vector3d &abs_pos);

        void set_fader_override_preset_closest_to_distance(f32 fade_distance, only_if_you_know_what_you_are_doing magic);
        void on_fade_distance_changed(i32 preset_index, only_if_you_know_what_you_are_doing magic);

        // sub_6075F0
        bool has_adopted_children() const {
            return adopted_children && !adopted_children->empty();
        }

        void add_adopted_child(entity_base* child);
        void remove_adopted_child(entity_base* child);
        void remove_child(entity_base* bad_kid);
        void clear_adopted_children();
        void dirty_adopted_children();

        bool set_flag_in_the_moved_list(); // true when this call set it

        static void stop_owned_effects(entity_base* ent);

        // the milestone's entity_base::create_<name>_ifc(); an existing one is answered as is
        script_data_interface* create_script_data_ifc();
        physical_interface*    create_physical_ifc();
        damage_interface*      create_damage_ifc();
        time_interface*        create_time_ifc();

        generic_interface* find_ifc_by_name(string_hash name);

    private:
        void finalize(mash::allocation_scope scope);

        template <typename ask_t>
        bool ask_ifcs(ask_t ask);
    };

    ASSERT_SIZEOF  (entity_base,                   0x50);
    ASSERT_OFFSETOF(entity_base, id,               0x08);
    ASSERT_OFFSETOF(entity_base, my_rel_po,        0x0C);
    ASSERT_OFFSETOF(entity_base, my_abs_po,        0x10);
    ASSERT_OFFSETOF(entity_base, flags,            0x14);
    ASSERT_OFFSETOF(entity_base, ext_flags,        0x18);
    ASSERT_OFFSETOF(entity_base, my_ifc_storage,   0x1C);
    ASSERT_OFFSETOF(entity_base, my_parent,        0x20);
    ASSERT_OFFSETOF(entity_base, my_child,         0x24);
    ASSERT_OFFSETOF(entity_base, my_brother,       0x28);
    ASSERT_OFFSETOF(entity_base, adopted_children, 0x2C);
    ASSERT_OFFSETOF(entity_base, my_trigger,       0x30);
    ASSERT_OFFSETOF(entity_base, fade_timer,       0x3A);
    ASSERT_OFFSETOF(entity_base, clone_member_idx, 0x3B);
    ASSERT_OFFSETOF(entity_base, my_conglom_root,  0x40);
    ASSERT_OFFSETOF(entity_base, bone_idx,         0x44);
    ASSERT_OFFSETOF(entity_base, sound_type,       0x4C);

    void entity_set_abs_po(entity_base* ent, const po &the_po);
    void entity_set_abs_position(entity_base* ent, const vector3d &pos);
    void entity_set_abs_parent(entity_base* me, entity_base* parent); // keeps the world po
    void entity_set_facing_and_up(actor* ent, const vector3d &facing, const vector3d &up, bool force);

    i32 compare_entity_base_ptr_entity_id(const void* x1, const void* x2); // for qsort
} // treyarch
