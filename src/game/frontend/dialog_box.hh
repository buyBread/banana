#pragma once

#include "game/frontend/igo/igo_3d_widget.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // the ui_hud_rvb_choice two-choice dialog; an input listener with its widget at +0x10
    class dialog_box {

    public:
        void**        vtable;
        u8            reserved_004[0x0C];
        igo_3d_widget widget;
        bool          shown;
        bool          confirmed;
        u8            choice;     // 0 or 1
        u8            reserved_163[0x5D];

        // slot 0
        void destroy() {
            ((void (__thiscall*)(dialog_box*, u32))vtable[0])(this, 1);
        }

        // slot 24; resets the choice and the timers
        void show(u32 unk) {
            ((void (__thiscall*)(dialog_box*, u32))vtable[0x60 / 4])(this, unk);
        }

        // slot 25
        void hide() {
            ((void (__thiscall*)(dialog_box*))vtable[0x64 / 4])(this);
        }
    };

    ASSERT_SIZEOF  (dialog_box,            0x1C0);
    ASSERT_OFFSETOF(dialog_box, widget,    0x010);
    ASSERT_OFFSETOF(dialog_box, shown,     0x160);
    ASSERT_OFFSETOF(dialog_box, confirmed, 0x161);
    ASSERT_OFFSETOF(dialog_box, choice,    0x162);
} // treyarch
