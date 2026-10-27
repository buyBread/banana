#pragma once

#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace speedtree {
    // filled by CSpeedTreeRT (sub_A3CE30); -1 lods and -1.0 alphas until then (sub_A3C7D0).
    // the two alphas always come out equal
    struct lod_values {
        i32 branch_lod;
        f32 branch_alpha;
        i32 frond_lod;
        f32 frond_alpha;
        i32 leaf_lods[2];
        f32 leaf_alphas[2];
        f32 billboard_fade;
    };

    // only the leaf angle tables; the wind matrices go through sub_A43580
    struct wind {
        u8   reserved_000[0x60];
        f32* leaf_angles_060;
        u8   reserved_064[0x20];
        f32* leaf_angles_084;
    };

    ASSERT_SIZEOF  (lod_values,                 0x24);
    ASSERT_OFFSETOF(lod_values, frond_lod,      0x08);
    ASSERT_OFFSETOF(lod_values, leaf_lods,      0x10);
    ASSERT_OFFSETOF(lod_values, leaf_alphas,    0x18);
    ASSERT_OFFSETOF(lod_values, billboard_fade, 0x20);

    ASSERT_OFFSETOF(wind, leaf_angles_060, 0x60);
    ASSERT_OFFSETOF(wind, leaf_angles_084, 0x84);
} // speedtree
