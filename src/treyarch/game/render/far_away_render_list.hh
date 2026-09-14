#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    struct far_away_render_list_entry {
        far_away_render_list_entry* next;
        u32                         vhandle;
    };

    ASSERT_SIZEOF(far_away_render_list_entry, 0x08);
} // treyarch
