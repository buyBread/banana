#pragma once

#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/math/algebra.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // the scrapbook's "ui_shell_tab_bar_bottom"; no slots past the widget's
    class igo_3d_scrapbook_sub_tab_pages : public igo_3d_widget {

    public:
        u8             reserved_150[0x10];
        igo_3d_widget* button;           // "ui_shell_sub_tab_bar_lb_btn"
        vector3d       initial_position; // the parameters' position at construction
        u8             reserved_170[0x10];
    };

    ASSERT_SIZEOF  (igo_3d_scrapbook_sub_tab_pages,                   0x180);
    ASSERT_OFFSETOF(igo_3d_scrapbook_sub_tab_pages, button,           0x160);
    ASSERT_OFFSETOF(igo_3d_scrapbook_sub_tab_pages, initial_position, 0x164);
} // treyarch
