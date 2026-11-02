#pragma once

#include "game/arch_base_vhandle.hh"
#include "game/math/algebra/vector3d.hh"
#include "treyarch/mm/mm.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // the ring under a tracked entity
    class igo_3d_gta_widget {

    public:
        void**            vtable;
        arch_base_vhandle target_ring;
        f32               render_scale;
        vector3d          pos_offset;
        bool              active;
        u8                unk_19;
        u8                reserved_01a[0x02];
        f32               unk_1c;
        u8                unk_20;
        u8                unk_21;
        u8                reserved_022[0x02];

        igo_3d_gta_widget();

        void* operator new(std::size_t size) noexcept {
            return mm::allocate(size);
        }

        void set_color(vector3d color);
    };

    ASSERT_SIZEOF  (igo_3d_gta_widget,               0x24);
    ASSERT_OFFSETOF(igo_3d_gta_widget, target_ring,  0x04);
    ASSERT_OFFSETOF(igo_3d_gta_widget, render_scale, 0x08);
    ASSERT_OFFSETOF(igo_3d_gta_widget, pos_offset,   0x0C);
    ASSERT_OFFSETOF(igo_3d_gta_widget, active,       0x18);
    ASSERT_OFFSETOF(igo_3d_gta_widget, unk_1c,       0x1C);
    ASSERT_OFFSETOF(igo_3d_gta_widget, unk_20,       0x20);

    namespace references {
        inline util::memory_reference<void*> igo_3d_gta_widget_vtable { 0x00BBA4B0 };
    } // references
} // treyarch
