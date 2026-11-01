#pragma once

#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_nav_button_bar {

    public:
        virtual ~igo_3d_nav_button_bar();

        virtual bool unk_001(bool unk) = 0;
        virtual bool unk_002() = 0;
        virtual void unk_003() = 0;
        virtual void unk_004(f32 unk) = 0;
        virtual void draw_buttons() = 0;
        virtual i32  unk_006(i32 unk_0, bool unk_1, i32 unk_2, i32 unk_3) = 0;
        virtual void unk_007() = 0;
        virtual void unk_008() = 0;
    };
} // treyarch
