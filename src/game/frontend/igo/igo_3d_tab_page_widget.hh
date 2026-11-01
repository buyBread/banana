#pragma once

#include "game/frontend/igo/igo_3d_slideable_widget.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_tab_page_widget : public igo_3d_slideable_widget {

    public:
        u8  reserved_150[0x48];
        i32 first_visible_tab; // set_current_tab scrolls it to keep the current tab in view
        u8  reserved_19c[0x54];

        virtual void next_tab() = 0;
        virtual void previous_tab() = 0;
        virtual void slide_in() = 0;  // a timed move to the in position, then shown
        virtual void slide_out() = 0; // a timed move to the out position
        virtual void set_current_tab(i32 index) = 0; // wraps around either end
        virtual void unk_059() = 0;
    };

    // the scrapbook's "ui_shell_tab_bar"
    class igo_3d_scrapbook_tab_pages : public igo_3d_tab_page_widget {};

    ASSERT_OFFSETOF(igo_3d_tab_page_widget, first_visible_tab, 0x198);

    ASSERT_SIZEOF(igo_3d_scrapbook_tab_pages, 0x1F0);
} // treyarch
