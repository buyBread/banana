#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class igo_3d_drawable;

    enum igo_widget_suppression_condition : u32 {
        suppress_when_paused             = 0x01,
        suppress_when_interface_disabled = 0x02,
        suppress_when_zoom_map_open      = 0x04,
        suppress_when_loading_screen     = 0x08,
        suppress_when_letterbox_visible  = 0x10,
        suppress_when_pauseless_dialog   = 0x20,
    };

    using igo_3d_widget_test = u8 (__thiscall*)(igo_3d_drawable* self);
    using igo_3d_widget_method = void (__thiscall*)(igo_3d_drawable* self);

    struct igo_3d_widget_vtable {
        void*                reserved_000[3];
        igo_3d_widget_method draw;
        igo_3d_widget_test   is_visible;
        igo_3d_widget_method method_014;
        void*                reserved_018[20];
        igo_3d_widget_method method_068;
        igo_3d_widget_method method_06c;
        void*                reserved_070;
        igo_3d_widget_test   test_074;
        void*                reserved_078[2];
        u32 (__thiscall* suppression_mask)(igo_3d_drawable* self);
    };

    class igo_3d_drawable {
    public:
        igo_3d_widget_vtable* vtable;

        void draw() {
            vtable->draw(this);
        }

    };

    class igo_3d_widget : public igo_3d_drawable {
    public:
        u8    reserved_004[0x0C];
        void* element;
        u32   flags;
        u8    reserved_018[0xC0];
        u8    unk_0d8;             // the base constructor sets it; while the pauseless dialog has it set, no mission or hint text is drawn
        u8    reserved_0d9[0x77];

        u8 is_visible() {
            return vtable->is_visible(this);
        }

        u32 suppression_mask() {
            return vtable->suppression_mask(this);
        }
    };

    ASSERT_SIZEOF(igo_3d_drawable, 0x004);

    ASSERT_SIZEOF  (igo_3d_widget,                            0x150);
    ASSERT_OFFSETOF(igo_3d_widget,        element,            0x010);
    ASSERT_OFFSETOF(igo_3d_widget,        flags,              0x014);
    ASSERT_OFFSETOF(igo_3d_widget,        unk_0d8,            0x0D8);
    ASSERT_OFFSETOF(igo_3d_widget_vtable, draw,               0x00C);
    ASSERT_OFFSETOF(igo_3d_widget_vtable, is_visible,         0x010);
    ASSERT_OFFSETOF(igo_3d_widget_vtable, method_014,         0x014);
    ASSERT_OFFSETOF(igo_3d_widget_vtable, method_068,         0x068);
    ASSERT_OFFSETOF(igo_3d_widget_vtable, method_06c,         0x06C);
    ASSERT_OFFSETOF(igo_3d_widget_vtable, test_074,           0x074);
    ASSERT_OFFSETOF(igo_3d_widget_vtable, suppression_mask,   0x080);
} // treyarch
