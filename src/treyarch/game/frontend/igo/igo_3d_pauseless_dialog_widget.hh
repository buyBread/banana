#pragma once

#include "treyarch/game/frontend/igo/igo_3d_widget.hh"
#include "treyarch/shared/mash/string.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class igo_3d_pauseless_dialog_widget : public igo_3d_widget {

    public:
        u8 reserved_150[0xDC];
        u8 unk_22c;

        // slot 54; takes a string table index
        void method_0d8(i32 text_id) {
            ((void (__thiscall*)(igo_3d_pauseless_dialog_widget*, i32))((void**)vtable)[0xD8 / 4])(this, text_id);
        }

        // slot 57; the string is passed by value, so callers build it straight into the call
        using method_0e4_function = void (__thiscall*)(igo_3d_pauseless_dialog_widget* self, mash::string text);

        method_0e4_function method_0e4() const {
            return (method_0e4_function)((void**)vtable)[0xE4 / 4];
        }

        // slot 58
        void method_0e8() {
            ((void (__thiscall*)(igo_3d_pauseless_dialog_widget*))((void**)vtable)[0xE8 / 4])(this);
        }

        // slot 72; the chosen button
        i32 method_120() {
            return ((i32 (__thiscall*)(igo_3d_pauseless_dialog_widget*))((void**)vtable)[0x120 / 4])(this);
        }
    };

    ASSERT_OFFSETOF(igo_3d_pauseless_dialog_widget, unk_22c, 0x22C);
} // treyarch
