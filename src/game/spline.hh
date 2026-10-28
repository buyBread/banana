#pragma once

#include "game/boolx.hh"
#include "game/math/algebra.hh"
#include "treyarch/mm/mm.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"
#include "vc8/vector.hh"

namespace treyarch {
    // SM3's eSplineType
    enum e_spline_type : i32 {
        spline_default,
        spline_direct,
        spline_catmull_rom,
        spline_b_spline,
        spline_bezier       // one spline per three points after the first
    };

    // a curve through control points
    class spline {

    public:
        vc8::vector<vector3d> control_pts;
        vc8::vector<f32>      control_pts_pct;         // one per control point, rebuilt with them
        i32                   sub_divisions;
        i32                   num_splines;
        e_spline_type         spline_type;
        boolx                 force_start;
        boolx                 need_rebuild;            // by a new type or control points
        boolx                 rebuild_control_pts_pct; // set whenever the control points change
        u8                    pad_02f;
        f32                   radius;
        vector3d              center;

        // sub_57FE90
        spline() : sub_divisions(5), spline_type(spline_b_spline), force_start(true), need_rebuild(true), rebuild_control_pts_pct(true) {}

        // inlined @ sub_5F6560
        void set_type(e_spline_type new_type) {
            need_rebuild = spline_type != new_type;
            spline_type  = new_type;
        }

        // inlined @ sub_5F6560
        // inlined @ sub_87E4F0
        void clear_control_pts() {
            control_pts.clear();

            need_rebuild = true;
        }

        // inlined @ sub_5F6560
        // inlined @ sub_661B20
        void add_control_pt(const vector3d &point) {
            control_pts.push_back(point);

            need_rebuild            = true;
            rebuild_control_pts_pct = true;
        }

        void* operator new(std::size_t size) noexcept {
            return mm::allocate((u32)size);
        }

        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
        }
    };

    ASSERT_SIZEOF  (spline,                          0x40);
    ASSERT_OFFSETOF(spline, control_pts,             0x00);
    ASSERT_OFFSETOF(spline, control_pts_pct,         0x10);
    ASSERT_OFFSETOF(spline, sub_divisions,           0x20);
    ASSERT_OFFSETOF(spline, spline_type,             0x28);
    ASSERT_OFFSETOF(spline, need_rebuild,            0x2D);
    ASSERT_OFFSETOF(spline, rebuild_control_pts_pct, 0x2E);
    ASSERT_OFFSETOF(spline, radius,                  0x30);
    ASSERT_OFFSETOF(spline, center,                  0x34);
} // treyarch
