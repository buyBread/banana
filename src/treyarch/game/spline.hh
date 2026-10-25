#pragma once

#include "treyarch/shared/math/algebra.hh"
#include "treyarch/shared/memory/heap.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"
#include "vc8/vector.hh"

namespace treyarch {
    // a curve through control points
    class spline {

    public:
        vc8::vector<vector3d> control_pts;
        vc8::vector<f32>      unk_010;            // one per control point, rebuilt with them
        i32                   unk_020;
        u8                    reserved_024[0x04];
        i32                   type;               // 3 from the constructor
        bool                  unk_02c;
        bool                  changed;            // by a new type or control points
        bool                  rebuild;            // set whenever the control points change
        u8                    reserved_02f[0x11];

        // sub_57FE90
        spline() : unk_020(5), type(3), unk_02c(true), changed(true), rebuild(true) {}

        // inlined @ sub_5F6560
        void set_type(i32 new_type) {
            changed = type != new_type;
            type    = new_type;
        }

        // inlined @ sub_5F6560
        void clear_control_pts() {
            control_pts.clear();

            changed = true;
        }

        // inlined @ sub_5F6560
        void add_control_pt(const vector3d &point) {
            control_pts.push_back(point);

            changed = true;
            rebuild = true;
        }

        void* operator new(std::size_t size) noexcept {
            return memory::heap::allocate((u32)size);
        }

        void operator delete(void* allocation) noexcept {
            memory::heap::free(allocation);
        }
    };

    ASSERT_SIZEOF  (spline,              0x40);
    ASSERT_OFFSETOF(spline, control_pts, 0x00);
    ASSERT_OFFSETOF(spline, unk_010,     0x10);
    ASSERT_OFFSETOF(spline, unk_020,     0x20);
    ASSERT_OFFSETOF(spline, type,        0x28);
    ASSERT_OFFSETOF(spline, changed,     0x2D);
    ASSERT_OFFSETOF(spline, rebuild,     0x2E);
} // treyarch
