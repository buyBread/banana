#pragma once

#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"
#include "game/arch_base_vhandle.hh"

namespace treyarch {
    struct far_away_render_list_entry {
        far_away_render_list_entry* next;
        arch_base_vhandle           vhandle;

        void activate(f32 amount);
    };

    ASSERT_SIZEOF(far_away_render_list_entry, 0x08);
} // treyarch
