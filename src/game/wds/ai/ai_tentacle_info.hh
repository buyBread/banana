#pragma once

#include "game/container/simple_list.hh"
#include "game/arch_base_vhandle.hh"
#include "game/color.hh"
#include "game/math/algebra/quaternion.hh"
#include "game/math/algebra/vector3d.hh"
#include "game/math/po.hh"
#include "game/mutex.hh"
#include "game/resource_key.hh"
#include "game/spline.hh"
#include "game/wds/entity/conglomerate.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "treyarch/mash/vector_basic.hh"
#include "treyarch/mm/mm.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class ai_tentacle_dangle;
    class ai_tentacle_engine;
    class entity;
    class polytube;
    class polytube_misc_render_object;
    class polytube_render_info;
    class polytube_widths_params;

    // the simulated rope a polytube draws as a tentacle
    class ai_tentacle_info {

    public:
        enum e_ai_tentacle_flags : u32 {
            in_use                 = 0x00000001,
            reverse_spline         = 0x00000002,
            check_collisions       = 0x00000004,
            base_node_is_member    = 0x00000008,
            end_node_is_member     = 0x00000010,
            use_polytube           = 0x00000020,
            pseudo_3d              = 0x00000040,
            update_node_po         = 0x00000080,
            need_po_vector         = 0x00000080,
            external_polytube      = 0x00000100,
            external_base_position = 0x00000200,
            distance_lod           = 0x00000400, // handed to the polytube
            manual_update_spline   = 0x20000000,
            manual_frame_advance   = 0x40000000,
            dynamic                = 0x80000000
        };

        container::simple_list_variables
            <ai_tentacle_info*>            simple_list_vars;              // on all_tentacles
        string_hash                        id;                            // SM3's ai_generic_id
        arch_base_vhandle                  my_entity;
        polytube*                          tentacle;
        polytube_render_info*              tentacle_info;
        f32                                code_blend;                    // how much code, not animation, places the positions
        f32                                desired_code_blend;
        f32                                code_blend_speed;
        f32                                tween_timer;
        f32                                tween_duration;
        f32                                tween_amount;
        vector3d                           tween_end_pos;
        quaternion                         tween_end_quat;
        mash::vector_basic
            <vector3d>                     tween_positions;
        string_hash                        base_node_id;
        string_hash                        end_node_id;
        entity*                            base_node;
        entity*                            end_node;
        mash::vector_basic
            <e_conglom_member_index>       node_indices;
        vector3d                           base_pos;
        vector3d                           end_pos;
        quaternion                         end_quat;
        mash::vector_basic
            <vector3d>                     positions;                     // the points between the base and the end
        mash::vector_basic
            <po>                           node_po_storage;
        f32                                length_percent;
        f32                                cur_length;
        f32                                default_length;
        f32                                tpm_delta;
        f32                                counter_window;
        u32                                flags;
        container::simple_list
            <ai_tentacle_engine*>          engines;                       // only the first one runs
        u32                                misc_engine_id;                // the dangle engine while dangling
        resource_key                       misc_ent_resource;
        arch_base_vhandle                  misc_render_ent;
        container::simple_list
            <polytube_misc_render_object*> misc_render_pool;
        f32                                misc_render_ent_dist;
        i32                                max_misc_render_ents;
        color32                            default_misc_render_ent_color;
        mash::vector
            <polytube_widths_params>       widths_params;                 // handed to the polytube's widths
        u32                                mesh_column_index;
        u32                                mesh_row_index;

        ~ai_tentacle_info();

        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
        }

        void destruct_mashed_class();

        static ai_tentacle_info* find_tentacle(const string_hash &search_id, ai_tentacle_info** array, i32 size);

        static void frame_advance_all_tentacles(f32 delta_t);
        static void update_spline_on_all_tentacles();

        // sub_48E720
        // inlined @ sub_4B77A0
        static vector3d compute_blend_pos(const vector3d &a_pt, const vector3d &b_pt, f32 blend) {
            if (blend < 0.0001f)
                return a_pt;

            if (blend > 1.0f - 0.0001f)
                return b_pt;

            return vector3d((b_pt.x - a_pt.x) * blend + a_pt.x,
                            (b_pt.y - a_pt.y) * blend + a_pt.y,
                            (b_pt.z - a_pt.z) * blend + a_pt.z);
        }

        void    set_my_entity(entity_base* own);
        entity* get_my_entity() const;
        entity_base* get_tentacle_parent() const;

        void create_tentacle(polytube* owner);
        void destroy_tentacle(bool keep_render_info);
        void init_num_positions(i32 num_pos, entity* _base_node, entity* _end_node);
        void apply_render_info();

        void set_visible(bool vis);
        bool is_visible() const;

        void set_spline_type(e_spline_type type);

        void frame_advance(f32 delta_t);
        void update_spline(bool ignore_limbo);

        // sub_48E840
        bool under_code_control() const {
            return code_blend >= 0.0001f;
        }

        bool is_tweening() const {
            return tween_timer < tween_duration;
        }

        void set_code_blend(f32 blend, f32 time);
        void init_full_code_control();

        vector3d get_base_position() const;
        void     set_base_position(const vector3d &pos);

        i32             get_num_positions() const;
        const vector3d &get_position(i32 index) const;
        void            set_position(i32 index, const vector3d &pos);

        void            set_end_position(const vector3d &pos);
        const vector3d &get_end_position() const;
        vector3d        get_abs_end_position() const;

        void set_end_po(po &the_po); // the po's fourth column is rewritten
        po   get_end_po() const;
        po   get_abs_end_po() const;

        void create_line(const vector3d &end, const vector3d* end_face);

        void start_dangle(f32 desired_length, const vector3d &initial_vel);
        void stop_dangle(f32 time);

        bool                is_dangling();
        ai_tentacle_dangle* get_dangle_engine();

        u32                 push_engine(ai_tentacle_engine* engine); // answers the engine's id
        ai_tentacle_engine* get_engine(u32 engine_id);
        bool                kill_engine(u32 engine_id);

    protected:
        void initialize(mash::allocation_scope scope);

        void init_code_tween(f32 time);
    };

    void update_node_facing(po* node, const po &from, const vector3d &at);

    namespace references {
        inline util::memory_reference<container::simple_list<ai_tentacle_info*>> all_tentacles { 0x00FC508C };

        inline util::memory_reference<engine_recursive_lock> all_tentacles_lock { 0x00FC4FF0 };
    } // references

    ASSERT_SIZEOF  (ai_tentacle_info,                                0x130);
    ASSERT_OFFSETOF(ai_tentacle_info, id,                            0x00C);
    ASSERT_OFFSETOF(ai_tentacle_info, tentacle,                      0x014);
    ASSERT_OFFSETOF(ai_tentacle_info, code_blend,                    0x01C);
    ASSERT_OFFSETOF(ai_tentacle_info, tween_end_pos,                 0x034);
    ASSERT_OFFSETOF(ai_tentacle_info, tween_end_quat,                0x040);
    ASSERT_OFFSETOF(ai_tentacle_info, tween_positions,               0x050);
    ASSERT_OFFSETOF(ai_tentacle_info, base_node,                     0x068);
    ASSERT_OFFSETOF(ai_tentacle_info, node_indices,                  0x070);
    ASSERT_OFFSETOF(ai_tentacle_info, base_pos,                      0x080);
    ASSERT_OFFSETOF(ai_tentacle_info, end_pos,                       0x08C);
    ASSERT_OFFSETOF(ai_tentacle_info, end_quat,                      0x098);
    ASSERT_OFFSETOF(ai_tentacle_info, positions,                     0x0A8);
    ASSERT_OFFSETOF(ai_tentacle_info, node_po_storage,               0x0B8);
    ASSERT_OFFSETOF(ai_tentacle_info, length_percent,                0x0C8);
    ASSERT_OFFSETOF(ai_tentacle_info, flags,                         0x0DC);
    ASSERT_OFFSETOF(ai_tentacle_info, engines,                       0x0E0);
    ASSERT_OFFSETOF(ai_tentacle_info, misc_engine_id,                0x0EC);
    ASSERT_OFFSETOF(ai_tentacle_info, misc_ent_resource,             0x0F0);
    ASSERT_OFFSETOF(ai_tentacle_info, misc_render_ent,               0x0F8);
    ASSERT_OFFSETOF(ai_tentacle_info, misc_render_pool,              0x0FC);
    ASSERT_OFFSETOF(ai_tentacle_info, default_misc_render_ent_color, 0x110);
    ASSERT_OFFSETOF(ai_tentacle_info, widths_params,                 0x114);
    ASSERT_OFFSETOF(ai_tentacle_info, mesh_column_index,             0x128);
} // treyarch
