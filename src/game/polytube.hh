#pragma once

#include "game/boolx.hh"
#include "game/color.hh"
#include "game/math/algebra/matrix4x4.hh"
#include "game/math/algebra/vector3d.hh"
#include "game/mutex.hh"
#include "game/spline.hh"
#include "game/wds/entity/entity.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"
#include "vc8/vector.hh"

namespace treyarch {
    namespace ngl {
        struct mesh;
    } // ngl

    class polytube_widths;

    // SM3's ai_tentacle engine types
    enum e_tentacle_engine_type : i32 {
        tentacle_engine_strike       = 1,
        tentacle_engine_swirl_strike = 2,
        tentacle_engine_dangle       = 5, // ai_tentacle_dangle and ai_tentacle_dangle_old
        tentacle_engine_follow       = 6,
        tentacle_engine_slack        = 7
    };

    // one way a tentacle moves; the tentacle info runs and blends them
    class ai_tentacle_engine {

    public:
        void** vtable;

        // slot 1
        e_tentacle_engine_type get_type() {
            return ((e_tentacle_engine_type (__thiscall*)(ai_tentacle_engine*))vtable[1])(this);
        }

        // slot 6; dangles and slacks
        void set_length(f32 length) {
            ((void (__thiscall*)(ai_tentacle_engine*, f32))vtable[6])(this, length);
        }

        // slot 7; dangles and slacks
        f32 get_length() {
            return ((f32 (__thiscall*)(ai_tentacle_engine*))vtable[7])(this);
        }

        // slot 9; follows
        f32 get_slack_percent() {
            return ((f32 (__thiscall*)(ai_tentacle_engine*))vtable[9])(this);
        }

        // slot 10; follows
        void set_slack_percent(f32 percent) {
            ((void (__thiscall*)(ai_tentacle_engine*, f32))vtable[10])(this, percent);
        }
    };

    // ai_tentacle_strike and ai_tentacle_swirl_strike both keep their target here
    class ai_tentacle_strike : public ai_tentacle_engine {

    public:
        u8       reserved_004[0x18];
        vector3d target;
    };

    class ai_tentacle_follow : public ai_tentacle_engine {

    public:
        u8  reserved_004[0x20];
        f32 follow_drag;
    };

    class polytube;

    // SM3's ai_tentacle_info: the simulated tentacle a polytube draws
    class ai_tentacle_info {

    public:
        enum e_flags : u32 {
            check_collisions = 0x04
        };

        u8          reserved_000[0x0C];
        string_hash id; // SM3's ai_generic_id
        u8          reserved_010[0x04];
        polytube*   tentacle;
        u8          reserved_018[0xC4];
        u32         flags;

        static ai_tentacle_info* find_tentacle(const string_hash &search_id, ai_tentacle_info** array, i32 size);
    };

    // SM3's polytube_pt_anim
    struct polytube_pt_anim {
        u32      flags;    // 1 while it runs
        vector3d base;
        vector3d delta;
        f32      dist;
        f32      duration;
        f32      time;
        f32      speed;
    };

    // a tube drawn along a spline, optionally a tentacle's
    class polytube : public entity {

    public:
        u8                            reserved_074[0x2C];
        polytube*                     next_polytube;          // on active_polytubes or inactive_polytubes
        polytube*                     prev_polytube;
        boolx                         reversed;
        boolx                         widths_active;          // a widths record is animating
        u8                            pad_0aa[0x02];
        f32                           it_offset;
        boolx                         offsets_valid;
        boolx                         use_spline;
        boolx                         additive;
        boolx                         pseudo_3d;
        boolx                         external_tentacle_info;
        u8                            pad_0b5[0x03];
        matrix4x4*                    pseudo_3d_po_array;
        spline                        the_spline;
        u8                            reserved_0fc[0x04];
        f32                           tube_radius;
        f32                           unk_104;
        i32                           num_sides;
        f32                           tiles_per_meter;
        f32                           max_length;
        f32                           v_anim;                 // anything but 0 keeps a visible tube active
        f32                           cur_v;
        f32                           pseudo_3d_offset;
        vc8::vector
            <polytube_pt_anim>        pt_anims;
        u32                           tether_a;
        u32                           tether_b;
        f32                           tether_length;
        color32                       render_color;
        u8                            reserved_140[0x04];
        ai_tentacle_info*             tentacle_info;
        u8                            misc_renders[0x0C];
        u32                           tracked_entity;
        vc8::vector<u32>              tracked_control_pts;
        polytube_widths*              widths[4];
        vc8::vector<vector3d>         normals;
        u32                           widths_allocated;
        i32                           widths_count;
        boolx                         unk_190;
        u8                            pad_191[0x03];
        u32                           mesh_column_index;
        u32                           mesh_row_index;
        boolx                         distance_lod;
        u8                            pad_19d[0x03];
        ngl::mesh*                    mesh;
        f32                           width_scale;

        // SM3's set_tube_radius and set_num_sides
        void set_tube_radius(f32 radius) {
            if (tube_radius != radius) {
                tube_radius   = radius;
                offsets_valid = false;
            }
        }

        void set_num_sides(i32 sides) {
            if (num_sides != sides) {
                num_sides     = sides;
                offsets_valid = false;
            }
        }

        // sub_492C90
        i32 get_num_control_pts() const {
            return (i32)the_spline.control_pts.size();
        }

        void remove_from_list();
        bool has_active_anim() const;
        void v_animate();
        void add_control_pt(const vector3d &point);
    };

    namespace references {
        inline util::memory_reference<polytube*> inactive_polytubes { 0x00FFEBA4 };
        inline util::memory_reference<polytube*> active_polytubes   { 0x00FFEBA8 };

        inline util::memory_reference<engine_recursive_lock> polytube_lock { 0x00FFEDD8 };
    } // references

    ASSERT_OFFSETOF(ai_tentacle_strike, target,      0x1C);
    ASSERT_OFFSETOF(ai_tentacle_follow, follow_drag, 0x24);

    ASSERT_OFFSETOF(ai_tentacle_info, id,       0x0C);
    ASSERT_OFFSETOF(ai_tentacle_info, tentacle, 0x14);
    ASSERT_OFFSETOF(ai_tentacle_info, flags,    0xDC);

    ASSERT_SIZEOF(polytube_pt_anim, 0x2C);

    ASSERT_SIZEOF  (polytube,                      0x1A8);
    ASSERT_OFFSETOF(polytube, next_polytube,       0x0A0);
    ASSERT_OFFSETOF(polytube, offsets_valid,       0x0B0);
    ASSERT_OFFSETOF(polytube, pseudo_3d_po_array,  0x0B8);
    ASSERT_OFFSETOF(polytube, the_spline,          0x0BC);
    ASSERT_OFFSETOF(polytube, tube_radius,         0x100);
    ASSERT_OFFSETOF(polytube, num_sides,           0x108);
    ASSERT_OFFSETOF(polytube, v_anim,              0x114);
    ASSERT_OFFSETOF(polytube, pt_anims,            0x120);
    ASSERT_OFFSETOF(polytube, render_color,        0x13C);
    ASSERT_OFFSETOF(polytube, tentacle_info,       0x144);
    ASSERT_OFFSETOF(polytube, tracked_control_pts, 0x158);
    ASSERT_OFFSETOF(polytube, widths,              0x168);
    ASSERT_OFFSETOF(polytube, normals,             0x178);
    ASSERT_OFFSETOF(polytube, unk_190,             0x190);
    ASSERT_OFFSETOF(polytube, mesh,                0x1A0);
} // treyarch
