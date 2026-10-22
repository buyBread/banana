#pragma once

#include <cstddef>

#include "treyarch/nal/anim.hh"
#include "treyarch/shared/fixed_string.hh"
#include "treyarch/shared/four_cc.hh"
#include "treyarch/shared/memory/heap.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace nal {
    constexpr u32 scene_anim_type    = four_cc('S', 'A', 'N', 'M');
    constexpr u32 scene_anim_version = 0x20104;

    struct anim_instance;
    struct scene_anim_instance;

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

        nal::scene_anim_instance* create_instance();
    };

    // one playing scene anim: an anim instance per participant
    struct scene_anim_instance {
        nal::scene_anim*    scene_anim;
        nal::anim_instance* instances;  // chained through next, last participant first
        u32                 unk_08;
        u32                 unk_0c;

        scene_anim_instance() : scene_anim(nullptr), instances(nullptr) {}

        void destroy();

        void* operator new(std::size_t size) noexcept {
            return memory::heap::allocate(size);
        }

        void operator delete(void* allocation) noexcept {
            memory::heap::free(allocation);
        }
    };

    ASSERT_SIZEOF  (scene_anim_instance,             0x10);
    ASSERT_OFFSETOF(scene_anim_instance, scene_anim, 0x00);
    ASSERT_OFFSETOF(scene_anim_instance, instances,  0x04);
    ASSERT_OFFSETOF(scene_anim_instance, unk_08,     0x08);
    ASSERT_OFFSETOF(scene_anim_instance, unk_0c,     0x0C);

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
