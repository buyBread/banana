#pragma once

#include "game/boolx.hh"
#include "game/math/algebra/vector3d.hh"
#include "game/wds/entity/actor.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // a web spun strand by strand between anchors
    class web_wall : public actor {

    public:
        // a strand per step
        inline static constexpr i32 built = 0x40;

        u8       reserved_0d8[0x1B4];
        vector3d build_point;
        boolx    wall_active;
        u8       pad_299[0x07];
        i32      build_progress;  // building until it reaches built
        u8       reserved_2a4[0x04];
        char     material[0x100];
    };

    ASSERT_OFFSETOF(web_wall, build_point,    0x28C);
    ASSERT_OFFSETOF(web_wall, wall_active,    0x298);
    ASSERT_OFFSETOF(web_wall, build_progress, 0x2A0);
    ASSERT_OFFSETOF(web_wall, material,       0x2A8);
} // treyarch
