#pragma once

#include "treyarch/shared/math/algebra/vector3d.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    // reshapes an actor's collision capsule; mode 2 is a capsule its als state asks for
    class capsule_alter_sys {

    public:
        u32      unk_000;
        u32      mode;
        u32      prev_mode;
        u8       reserved_00c[0x3C];
        vector3d custom_base;
        vector3d custom_end;
        f32      custom_radius;

        void set_mode(u32 new_mode);
        void set_custom_capsule(const vector3d &base, const vector3d &end, f32 radius);
        void update(bool unk);
    };

    ASSERT_OFFSETOF(capsule_alter_sys, mode,          0x04);
    ASSERT_OFFSETOF(capsule_alter_sys, prev_mode,     0x08);
    ASSERT_OFFSETOF(capsule_alter_sys, custom_base,   0x48);
    ASSERT_OFFSETOF(capsule_alter_sys, custom_end,    0x54);
    ASSERT_OFFSETOF(capsule_alter_sys, custom_radius, 0x60);
} // treyarch
