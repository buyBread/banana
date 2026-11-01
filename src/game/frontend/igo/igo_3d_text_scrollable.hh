#pragma once

#include "game/frontend/igo/igo_3d_text.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // IGO3DText_Scrollable; the conversation menu's line
    class igo_3d_text_scrollable : public igo_3d_text {

    public:
        // slot 96
        i32 get_scroll_line() { // the first line shown
            return ((i32 (__thiscall*)(igo_3d_text_scrollable*))(*(void***)this)[96])(this);
        }

        // slot 97
        void set_scroll_line(i32 line) {
            ((void (__thiscall*)(igo_3d_text_scrollable*, i32))(*(void***)this)[97])(this, line);
        }

        // slot 98
        void scroll(i32 lines, bool clamp) {
            ((void (__thiscall*)(igo_3d_text_scrollable*, i32, bool))(*(void***)this)[98])(this, lines, clamp);
        }

        // slot 101
        i32 get_visible_line_count() {
            return ((i32 (__thiscall*)(igo_3d_text_scrollable*))(*(void***)this)[101])(this);
        }
    };
} // treyarch
