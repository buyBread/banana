#pragma once

#include "treyarch/game/frontend/igo/igo_3d_widget.hh"
#include "treyarch/shared/dinkumware/list.hh"
#include "treyarch/shared/mash/string.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class igo_3d_script_widget_child;

    struct igo_3d_script_widget_child_vtable {
        void*         reserved_000[2];
        void          (__thiscall* method_008)(igo_3d_script_widget_child* self, u32 value);
        void*         reserved_00c[43];
        mash::string* (__thiscall* get_label)(igo_3d_script_widget_child* self, mash::string* result); // by value
    };

    class igo_3d_script_widget_child {
    public:
        igo_3d_script_widget_child_vtable* vtable;
    };

    struct igo_3d_script_widget_vtable {
        void*                reserved_000[3];
        igo_3d_widget_method draw;
        igo_3d_widget_test   is_visible;
        void                 (__thiscall* set_visible)(igo_3d_drawable* self, bool visible); // `flags` bit 0
        void*                reserved_018[54];
        igo_3d_widget_test   unk_0f0;                                                        // returns unk_168
    };

    class igo_3d_script_widget : public igo_3d_widget {
    public:
        u8                                reserved_150[0x0C];
        dinkumware::list
            <igo_3d_script_widget_child*> children;
        u8                                unk_168;
        u8                                reserved_169[0x14];
        u8                                unk_17d; // drawn while the interface is disabled
        u8                                reserved_17e[0x12];

        igo_3d_script_widget_vtable* script_vtable() {
            return (igo_3d_script_widget_vtable*)vtable;
        }

        void set_visible(bool visible) {
            script_vtable()->set_visible(this, visible);
        }

        u8 unk_0f0() {
            return script_vtable()->unk_0f0(this);
        }
    };

    ASSERT_OFFSETOF(igo_3d_script_widget_child_vtable, get_label, 0x0B8);

    ASSERT_OFFSETOF(igo_3d_script_widget_vtable, set_visible, 0x014);
    ASSERT_OFFSETOF(igo_3d_script_widget_vtable, unk_0f0,     0x0F0);

    ASSERT_SIZEOF  (igo_3d_script_widget,           0x190);
    ASSERT_OFFSETOF(igo_3d_script_widget, children, 0x15C);
    ASSERT_OFFSETOF(igo_3d_script_widget, unk_168,  0x168);
    ASSERT_OFFSETOF(igo_3d_script_widget, unk_17d,  0x17D);
} // treyarch
