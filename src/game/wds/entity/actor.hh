#pragma once

#include "game/wds/entity/entity.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace chuck { namespace vm {
        class script_instance;
    }} // chuck::vm

    namespace nal {
        struct scene_anim_instance;
        struct skeleton_map;
        struct transport_list;
    } // nal

    namespace ngl {
        struct morph_set;
    } // ngl

    namespace amalga {
        struct resource_pack_slot;
    } // amalga

    namespace als {
        class animation_logic_system_interface;
        struct als_res_data;
    } // als

    class ai_core;
    class animation_controller;
    class capsule_alter_sys;
    struct resource_key;
    struct tl_fixed_string;

    struct base_ai_data {
        u8       reserved_000[0x28];
        ai_core* core;
    };

    struct advanced_entity_ptrs {
        u8                          reserved_000[0x0C];
        chuck::vm::script_instance* my_script;       // the actor's "__enx" instance
        u8*                         unk_010;         // its script object's name hash sits at +4
        resource_key*               script_exe_name; // its hash is the script's filename
        string_hash                 script_obj_name;
        u8                          reserved_01c[0x04];
        als::als_res_data*          my_als_data;
        u8                          reserved_024[0x04];
        i32                         unk_028;         // set_really_immobile makes it 10, or 0
    };

    class actor : public entity {

    public:
        u8                          reserved_0a0[0x04];
        nal::transport_list*        m_skeleton;
        nal::skeleton_map*          skeleton_map;
        animation_controller*       anim_ctrl;
        advanced_entity_ptrs*       adv_ptrs;
        collision_geometry*         colgeom;
        u8                          reserved_0b8[0x08];
        amalga::resource_pack_slot* pack_slot; // the one the actor came from; its script threads take it as their resource context
        u8                          reserved_0c4[0x0C];
        base_ai_data*               my_base_ai_data;
        capsule_alter_sys*          capsule_alter;
        u8                          reserved_0d8[0x10];

        explicit actor(mash::generating_vtable_version so_do_nothing) : entity(so_do_nothing) {}

        ~actor() override;

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool                is_an_actor() const override;
        vector3d            get_last_position() const override;
        collision_geometry* get_colgeom() const override;
        void                set_collisions_active(bool active, bool recursive) override;
        bool                are_character_collisions_active() const override;
        bool                is_alive() const override;
        e_entity_flavor     get_flavor() const override;
        bool                is_a_placeholder_vehicle() const override;
        void                post_unmash_fixup(amalga::resource_pack_slot* pack_slot) override;
        u32                 get_entity_size() const override;
        void                unk_108(f32 unk) override;
        void                render(f32 camera_distance, f32 entity_translucency_pct) override;
        ngl::mesh*          get_mesh() const override;
        ngl::mesh*          unk_111() const override;
        bool                has_mesh() const override;
        f32                 get_visual_radius() const override;
        vector3d            get_visual_center() const override;
        void                suspend(bool update_region) override; // every actor class shares this body
        void                unsuspend(bool update_region) override;
        void                set_render_color(color32 color) override;
        color32             get_render_color() const override;
        void                set_render_alpha_mod(f32 alpha) override;
        f32                 get_render_alpha_mod() const override;
        void                set_render_scale(const vector3d &scale) override;
        vector3d            get_render_scale() const override;
        void                set_render_zbias(f32 zbias) override;
        f32                 get_render_zbias() const override;
        bool                hates_spiderman() override;

        virtual bool             is_a_rocket() const;
        virtual void             allocate_anim_controller();
        virtual f32              get_floor_offset();
        virtual void             set_floor_offset(f32 offset);
        virtual void*            bind_to_scene_anim(nal::scene_anim_instance* instance, i32 index);
        virtual void             unbind_from_scene_anim();
        virtual bool             unk_142();
        virtual void             set_colgeom_flag_recursive(u32 flag, bool set);
        virtual void             update_colgeom(po* replacement_po = nullptr);              // null takes the abs po
        virtual void             invalidate_colgeom();
        virtual f32              get_inter_capsule_radius_scale();
        virtual bool             test_combat_target(const vector3d  &p0,
                                                    const vector3d  &p1,
                                                          vector3d*  impact_pos,
                                                          vector3d*  impact_normal,
                                                          f32        default_radius,
                                                          bool       rear_cull) const;
        virtual f32              get_colgeom_radius() const;
        virtual vector3d         get_colgeom_center() const;
        virtual void             radius_changed(bool force);
        virtual ngl::morph_set*  get_morph(const tl_fixed_string &morph_name, bool warn_if_missing);
        virtual bool             is_frame_delta_valid() const;
        virtual bool             is_last_frame_delta_valid() const;
        virtual const po        &get_frame_delta() const;
        virtual void             set_frame_delta(const po &bob, f32 t);
        virtual void             set_frame_delta_trans(const vector3d &delta, f32 delta_t); // adds to the movement the actor takes this frame
        virtual void             invalidate_frame_delta();

        ai_core*                               get_ai_core() const;
        als::animation_logic_system_interface* get_als() const;

        // sub_602850
        bool has_base_ai_data() const {
            return my_base_ai_data != nullptr;
        }

        void set_frame_delta_no_update(const po &bob, f32 t);
        void save_last_collision_free_state();

        static string_hash my_type_key;

    private:
        void finalize(mash::allocation_scope scope);
    };

    ASSERT_OFFSETOF(base_ai_data, core, 0x28);

    ASSERT_OFFSETOF(advanced_entity_ptrs, my_script,       0x0C);
    ASSERT_OFFSETOF(advanced_entity_ptrs, script_exe_name, 0x14);
    ASSERT_OFFSETOF(advanced_entity_ptrs, script_obj_name, 0x18);
    ASSERT_OFFSETOF(advanced_entity_ptrs, my_als_data,     0x20);
    ASSERT_OFFSETOF(advanced_entity_ptrs, unk_028,         0x28);

    ASSERT_SIZEOF  (actor,                  0xE8);
    ASSERT_OFFSETOF(actor, m_skeleton,      0xA4);
    ASSERT_OFFSETOF(actor, skeleton_map,    0xA8);
    ASSERT_OFFSETOF(actor, anim_ctrl,       0xAC);
    ASSERT_OFFSETOF(actor, adv_ptrs,        0xB0);
    ASSERT_OFFSETOF(actor, colgeom,         0xB4);
    ASSERT_OFFSETOF(actor, pack_slot,       0xC0);
    ASSERT_OFFSETOF(actor, my_base_ai_data, 0xD0);
    ASSERT_OFFSETOF(actor, capsule_alter,   0xD4);
} // treyarch
