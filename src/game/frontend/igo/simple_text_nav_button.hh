#pragma once

#include "banana/util/types.hh"

namespace treyarch {
    class simple_nav_button {

    public:
        virtual ~simple_nav_button();

        virtual void unk_001(i32 unk) = 0;
        virtual void unk_002(i32 unk) = 0;
    };

    // a nav button with an igo_3d_text label
    class simple_text_nav_button : public simple_nav_button {

    public:
        virtual void unk_003(i32 unk) = 0;
        virtual void set_shown(bool shown) = 0; // shows or hides the label
    };
} // treyarch
