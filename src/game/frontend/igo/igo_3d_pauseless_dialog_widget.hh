#pragma once

#include "game/frontend/igo/igo_3d_dialog_widget.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_pauseless_dialog_widget : public igo_3d_dialog_widget {

    public:
        u8 reserved_230[0x20];
    };

    ASSERT_SIZEOF(igo_3d_pauseless_dialog_widget, 0x250);
} // treyarch
