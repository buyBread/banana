#pragma once

#include "vc8/list.hh"
#include "game/arch_base_vhandle.hh"
#include "game/math/algebra/vector3d.hh"
#include "game/mutex.hh"
#include "game/timing/simple_timer.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class cpu_combat_pie;
    class info_node;

    struct cpu_combat_pie_ring {
        u8  reserved_00[0x08];
        i32 num_slices;
    };

    // a fighter's place in the pie around its target: one slice of one ring
    struct cpu_combat_pie_slice {
        cpu_combat_pie* pie;             // null while unassigned
        info_node*      owner;           // its actor stands in for the pie while there's none
        i32             slice;
        i32             ring;
        simple_timer    unk_10;          // stale once it runs out
        f32             ring_0_min_dist;
        f32             ring_0_max_dist;
        f32             ring_gap;
        f32             ring_width;
        f32             buffer_arc_pct;
        i32             orientation;     // which of the pie's per-orientation arrays it's in

        bool is_valid() const;
        bool is_stale() const;

        arch_base_vhandle get_pie_target() const;

        vector3d get_pie_center() const;
        vector3d get_pie_normal() const;
        vector3d get_pie_facing() const;

        f32 get_pie_min_dist() const;
        f32 get_pie_max_dist() const;

        // inlined @ sub_4E7670
        // inlined @ sub_4EBBD0
        // inlined @ sub_4EBDB0
        f32 get_ring_min_dist() const {
            if (!ring)
                return ring_0_min_dist;

            return (f32)((f64)(f32)(ring - 1) * (f64)ring_width + (f64)ring_0_max_dist + (f64)ring_gap);
        }

        // inlined @ sub_4E76E0
        // inlined @ sub_4EBBD0
        // inlined @ sub_4EBDB0
        f32 get_ring_max_dist() const {
            if (!ring)
                return ring_0_max_dist;

            return (f32)((f64)ring_width - (f64)ring_gap + (f64)get_ring_min_dist());
        }

        i32 get_pie_num_rings()  const;
        i32 get_pie_num_slices() const;

        bool     pos_inside_slice(const vector3d* pos);
        vector3d get_new_pie_pos();
    };

    // the rings of slices the fighters around one target share; each array is per orientation
    class cpu_combat_pie {

    public:
        arch_base_vhandle               target;
        vector3d                        center[2];
        vector3d                        facing[2];
        vector3d                        normal[2];
        vc8::list<cpu_combat_pie_ring*> pie_rings[2];
        f32                             unk_64;       // the pie update lowers it by dt and resets it to 1 once spent

        // inlined @ sub_4EED80
        // inlined @ sub_4EBBD0
        // inlined @ sub_4EBDB0
        cpu_combat_pie_ring* get_ring(u32 ring_number, i32 orientation) const { // null past the last ring
            if (ring_number >= pie_rings[orientation].size())
                return nullptr;

            auto* position = pie_rings[orientation].begin();

            for (i32 i = (i32)ring_number; i > 0; --i)
                position = position->next;

            return position->value;
        }

        // pos defaults to the slice owner's actor position; any_ring takes every distance from 0 to 9999
        bool pos_in_slice(const cpu_combat_pie_slice* slice, const vector3d* pos, bool any_ring);

        vector3d slot_position(const cpu_combat_pie_slice* slice);
    };

    namespace references {
        // also guards the live pie list
        inline util::memory_reference<engine_recursive_lock> cpu_combat_pie_lock { 0x00FC5F60 };
    } // references

    ASSERT_SIZEOF  (cpu_combat_pie_slice,                  0x2C);
    ASSERT_OFFSETOF(cpu_combat_pie_slice, unk_10,          0x10);
    ASSERT_OFFSETOF(cpu_combat_pie_slice, buffer_arc_pct,  0x24);
    ASSERT_OFFSETOF(cpu_combat_pie_slice, orientation,     0x28);

    ASSERT_OFFSETOF(cpu_combat_pie, facing,    0x1C);
    ASSERT_OFFSETOF(cpu_combat_pie, normal,    0x34);
    ASSERT_OFFSETOF(cpu_combat_pie, pie_rings, 0x4C);
    ASSERT_OFFSETOF(cpu_combat_pie, unk_64,    0x64);

    ASSERT_OFFSETOF(cpu_combat_pie_ring, num_slices, 0x08);
} // treyarch
