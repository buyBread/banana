#pragma once

#include "game/math/algebra/vector3d.hh"
#include "vc8/vector.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // the particle rope a dangling tentacle engine simulates
    class dangler {

    public:
        struct particle {
            u8   reserved_00[0x34];
            bool unk_34; // follow's setup sets it on the particle past the tentacle's last position
            u8   pad_35[0x03];
        };

        vc8::vector<particle>* particles;
        i32                    particle_count;
        f32                    air_resistance;
        f32                    elasticity;
        f32                    segment_length;
        vector3d               gravity;
        u8                     reserved_20[0x04];
        bool                   allow_collisions;
        bool                   unk_25;
    };

    ASSERT_SIZEOF(dangler::particle, 0x38);

    ASSERT_OFFSETOF(dangler, air_resistance,   0x08);
    ASSERT_OFFSETOF(dangler, segment_length,   0x10);
    ASSERT_OFFSETOF(dangler, gravity,          0x14);
    ASSERT_OFFSETOF(dangler, allow_collisions, 0x24);
} // treyarch
