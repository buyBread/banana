#pragma once

#include "game/boolx.hh"
#include "game/colgeom.hh"
#include "game/resource_key.hh"
#include "game/wds/ai/param_block.hh"
#include "game/wds/entity/entity.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mm/mm.hh"
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
    class rb_character_controller;
    struct tl_fixed_string;

    struct base_ai_data {
        resource_key core_ai_key; // what the ai_core is built from
        param_block  overrides;   // what its parameters are overridden with
        ai_core*     core;

        // sub_64B170
        ~base_ai_data() {
            finalize(mash::ALLOCATED);
        }

        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
        }

        void finalize(mash::allocation_scope scope); // core keeps pointing at what it freed

        void construct_mashed_class();
        void destruct_mashed_class();
    };

    class movement_info {

    public:
        po       frame_delta;
        f32      frame_time;
        vector3d last_position;
        f32      delta_length;
        boolx    frame_delta_valid;
        boolx    last_frame_delta_valid;
        u8       pad_056[0x0A];

        void add_frame_delta_velocity(vector3d* target) const;
    };

    class render_info {

    public:
        color32  render_color;
        vector3d render_scale;
        f32      render_zbias;
        f32      render_alpha_mod;

        // inlined @ sub_620F50
        // inlined @ sub_620FC0
        // inlined @ sub_621040
        // inlined @ sub_6653F0
        render_info() : render_color(255, 255, 255, 255),
                        render_scale(1.0f, 1.0f, 1.0f),
                        render_zbias(1.0f),
                        render_alpha_mod(1.0f) {}
    };

    struct last_collision_free_state_t {
        po      xform;
        capsule rel_cap;
        i32     last_disable_frame_number;

        // inlined @ sub_6150C0
        // inlined @ sub_615260
        last_collision_free_state_t() {
            xform.identity();

            last_disable_frame_number = -1;
        }
    };

    struct advanced_entity_ptrs {
        vc8::vector<arch_base_vhandle>* ignore_col_ents;
        render_info*                    m_render_info;
        movement_info*                  mi;
        chuck::vm::script_instance*     my_script;        // the actor's "__enx" instance
        amalga::resource_pack_slot*     script_pack_slot; // its pack name is the script object's name
        resource_key*                   script_exe_name;  // its hash is the script's filename
        string_hash                     script_obj_name;
        u8                              unk_01c;          // the instance isn't removed with the script while set
        u8                              pad_01d[0x03];
        als::als_res_data*              my_als_data;
        f32                             unk_024;
        i32                             unk_028;          // set_really_immobile makes it 10, or 0

        // inlined @ sub_620ED0
        advanced_entity_ptrs() : unk_01c(0) {
            initialize(mash::ALLOCATED);
        }

        void finalize(mash::allocation_scope scope);

        void construct_mashed_class();
        void destruct_mashed_class();

        void release_script();

    private:
        void initialize(mash::allocation_scope scope);
        void free_records();
    };

    namespace references {
        // the generator actor's unk_0e0 and the variant interface's random textures draw from
        inline util::memory_reference<u32> unk_00e6b258 { 0x00E6B258 };
    } // references

    class actor : public entity {

    public:
        mash::string*                m_skeleton_name;
        nal::transport_list*         m_skeleton;
        nal::skeleton_map*           skeleton_map;
        animation_controller*        anim_ctrl;
        advanced_entity_ptrs*        adv_ptrs;
        collision_geometry*          colgeom;
        ngl::mesh*                   mesh;
        last_collision_free_state_t* last_collision_free_state;
        amalga::resource_pack_slot*  pack_slot; // the one the actor came from; its script threads take it as their resource context
        mutable vector3d             cached_visual_bounding_sphere_center;
        base_ai_data*                my_base_ai_data;
        capsule_alter_sys*           capsule_alter;
        rb_character_controller*     rb_cc;
        u32                          unk_0dc;
        u32                          unk_0e0;   // the material's random seed
        u32                          unk_0e4;

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
        virtual bool             is_inside_car();
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

        bool has_entity_collision() const;
        bool has_camera_collision() const;
        void replace_colgeom(collision_geometry* new_colgeom);

        void set_frame_delta_no_update(const po &bob, f32 t);
        void save_last_collision_free_state();
        void save_last_collision_free_state(const po &matrix, const capsule &rel_cap);

        void create_adv_ptrs();

        void add_collision_ignorance(arch_base_vhandle ent);
        void remove_collision_ignorance(arch_base_vhandle ent);

        void get_velocity(vector3d* target) const;

        static string_hash my_type_key;

    protected:
        void release_base_ai_data(mash::allocation_scope scope);
        void destroy_als(bool allocated);

    private:
        void initialize(mash::allocation_scope scope);
        void finalize(mash::allocation_scope scope);

        render_info* create_render_info();
    };

    ASSERT_OFFSETOF(base_ai_data, overrides, 0x08);
    ASSERT_OFFSETOF(base_ai_data, core,      0x28);

    ASSERT_SIZEOF  (movement_info,                         0x60);
    ASSERT_OFFSETOF(movement_info, frame_time,             0x40);
    ASSERT_OFFSETOF(movement_info, last_position,          0x44);
    ASSERT_OFFSETOF(movement_info, delta_length,           0x50);
    ASSERT_OFFSETOF(movement_info, frame_delta_valid,      0x54);
    ASSERT_OFFSETOF(movement_info, last_frame_delta_valid, 0x55);

    ASSERT_SIZEOF  (render_info,                   0x18);
    ASSERT_OFFSETOF(render_info, render_scale,     0x04);
    ASSERT_OFFSETOF(render_info, render_zbias,     0x10);
    ASSERT_OFFSETOF(render_info, render_alpha_mod, 0x14);

    ASSERT_SIZEOF  (last_collision_free_state_t,                            0x60);
    ASSERT_OFFSETOF(last_collision_free_state_t, rel_cap,                   0x40);
    ASSERT_OFFSETOF(last_collision_free_state_t, last_disable_frame_number, 0x5C);

    ASSERT_SIZEOF  (advanced_entity_ptrs,                   0x2C);
    ASSERT_OFFSETOF(advanced_entity_ptrs, m_render_info,    0x04);
    ASSERT_OFFSETOF(advanced_entity_ptrs, mi,               0x08);
    ASSERT_OFFSETOF(advanced_entity_ptrs, my_script,        0x0C);
    ASSERT_OFFSETOF(advanced_entity_ptrs, script_pack_slot, 0x10);
    ASSERT_OFFSETOF(advanced_entity_ptrs, script_exe_name,  0x14);
    ASSERT_OFFSETOF(advanced_entity_ptrs, script_obj_name,  0x18);
    ASSERT_OFFSETOF(advanced_entity_ptrs, unk_01c,          0x1C);
    ASSERT_OFFSETOF(advanced_entity_ptrs, my_als_data,      0x20);
    ASSERT_OFFSETOF(advanced_entity_ptrs, unk_024,          0x24);
    ASSERT_OFFSETOF(advanced_entity_ptrs, unk_028,          0x28);

    ASSERT_SIZEOF  (actor,                                       0xE8);
    ASSERT_OFFSETOF(actor, m_skeleton_name,                      0xA0);
    ASSERT_OFFSETOF(actor, m_skeleton,                           0xA4);
    ASSERT_OFFSETOF(actor, skeleton_map,                         0xA8);
    ASSERT_OFFSETOF(actor, anim_ctrl,                            0xAC);
    ASSERT_OFFSETOF(actor, adv_ptrs,                             0xB0);
    ASSERT_OFFSETOF(actor, colgeom,                              0xB4);
    ASSERT_OFFSETOF(actor, mesh,                                 0xB8);
    ASSERT_OFFSETOF(actor, last_collision_free_state,            0xBC);
    ASSERT_OFFSETOF(actor, pack_slot,                            0xC0);
    ASSERT_OFFSETOF(actor, cached_visual_bounding_sphere_center, 0xC4);
    ASSERT_OFFSETOF(actor, my_base_ai_data,                      0xD0);
    ASSERT_OFFSETOF(actor, capsule_alter,                        0xD4);
    ASSERT_OFFSETOF(actor, rb_cc,                                0xD8);
    ASSERT_OFFSETOF(actor, unk_0e4,                              0xE4);
} // treyarch
