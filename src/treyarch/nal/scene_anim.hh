#pragma once

#include "treyarch/nal/anim.hh"
#include "treyarch/shared/fixed_string.hh"
#include "treyarch/shared/four_cc.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace nal {
    constexpr u32 scene_anim_type    = four_cc('S', 'A', 'N', 'M');
    constexpr u32 scene_anim_version = 0x20104;

    enum e_scene_anim_flags : u32 {
        scene_anim_flag_looping = 0x02
    };

    // SANM, the embedded anims and participant names follow
    struct scene_anim {
        fixed_string name;
        void*        unk_08;
        u32          version;
        u32          flags;
        u32          num_anims;
        anim*        first_anim;
        u32          ref_count;
        f32          duration;
        void*        unk_24;

        void link_transport_lists();
    };

    ASSERT_OFFSETOF(scene_anim, name,       0x00);
    ASSERT_OFFSETOF(scene_anim, unk_08,     0x08);
    ASSERT_OFFSETOF(scene_anim, version,    0x0C);
    ASSERT_OFFSETOF(scene_anim, flags,      0x10);
    ASSERT_OFFSETOF(scene_anim, num_anims,  0x14);
    ASSERT_OFFSETOF(scene_anim, first_anim, 0x18);
    ASSERT_OFFSETOF(scene_anim, ref_count,  0x1C);
    ASSERT_OFFSETOF(scene_anim, duration,   0x20);
    ASSERT_OFFSETOF(scene_anim, unk_24,     0x24);
}} // treyarch::nal
