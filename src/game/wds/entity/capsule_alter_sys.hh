#pragma once

#include "game/math/algebra/vector3d.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mm/mm.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class actor;

    // SM3's eFlagType, the ones read so far
    enum e_capsule_alter_flags : u32 {
        capsule_alter_flag_from_mash = 0x00000008
    };

    // reshapes an actor's collision capsule; mode 2 is a capsule its als state asks for
    class capsule_alter_sys {

    public:
        actor*   owner;
        u32      mode;
        u32      prev_mode;
        u8       reserved_00c[0x3C];
        vector3d custom_base;
        vector3d custom_end;
        f32      custom_radius;
        u8       reserved_064[0x48];
        u32      flags;

        ~capsule_alter_sys() {
            finalize(mash::ALLOCATED);
        }

        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
        }

        // sub_689610
        void finalize(mash::allocation_scope) {}

        void construct_mashed_class();

        // sub_5B4A70
        void destruct_mashed_class() {}

        void set_mode(u32 new_mode);
        void set_custom_capsule(const vector3d &base, const vector3d &end, f32 radius);
        void update(bool unk);
    };

    ASSERT_OFFSETOF(capsule_alter_sys, mode,          0x04);
    ASSERT_OFFSETOF(capsule_alter_sys, prev_mode,     0x08);
    ASSERT_OFFSETOF(capsule_alter_sys, custom_base,   0x48);
    ASSERT_OFFSETOF(capsule_alter_sys, custom_end,    0x54);
    ASSERT_OFFSETOF(capsule_alter_sys, custom_radius, 0x60);
    ASSERT_OFFSETOF(capsule_alter_sys, flags,         0xAC);
} // treyarch
