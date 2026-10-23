#pragma once

#include <cstddef>

#include "vc8/vector.hh"
#include "treyarch/nal/anim_instance.hh"
#include "treyarch/nal/scene_anim.hh"
#include "treyarch/shared/arch_base_vhandle.hh"
#include "treyarch/shared/hash/algo.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/math/algebra.hh"
#include "treyarch/shared/memory/heap.hh"
#include "treyarch/shared/tl_fixed_string.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class entity;

    using scene_anim_handle_t = u32;

    // raised on every bound entity when its scene anim goes away
    inline const string_hash scene_anim_finished = string_hash(hash::djb2("SCENE_ANIM_FINISHED"));
    inline const string_hash scene_anim_ending   = string_hash(hash::djb2("SCENE_ANIM_ENDING"));

    // one bound entity (or the camera)
    class scene_anims_info {

    public:
        arch_base_vhandle   ent;
        vector3d            entity_up_vec;
        scene_anim_handle_t handle;
        nal::anim_instance* instance;
        mash::string        name;
        u32                 unk_24;
        u32                 status;   // bit 0: the entity was hidden when it was bound
    };

    // a light spawned for a participant whose anim carries light data
    struct scene_anim_light {
        entity*             light;
        nal::anim_instance* instance;
        i32                 index; // the participant's
    };

    class scene_anim {

    public:
        nal::scene_anim*          the_scene_anim;
        nal::scene_anim_instance* scene_anim_inst;
        vc8::vector
            <scene_anims_info>    my_anim_info_list;
        scene_anim_handle_t       the_handle;
        mash::string              my_name;
        u16                       my_flags;
        bool                      finished;
        bool                      dead;
        bool                      paused;
        vc8::vector
            <scene_anim_light>    lights;

        scene_anim();
        ~scene_anim();

        void load(const mash::string &filename);
        void play(scene_anim_handle_t handle, u16 flags, f32 offset);

        void find_actor_and_create_controller_for_subanim(      i32                 index,
                                                          const tl_fixed_string    &name,
                                                                nal::anim_instance* instance);

        void finish();

        void frame_advance(f32 delta_t);

        void* operator new(std::size_t size) noexcept {
            return memory::heap::allocate(size);
        }

        void operator delete(void* allocation) noexcept {
            memory::heap::free(allocation);
        }
    };

    ASSERT_SIZEOF  (scene_anims_info,                0x2C);
    ASSERT_OFFSETOF(scene_anims_info, entity_up_vec, 0x04);
    ASSERT_OFFSETOF(scene_anims_info, handle,        0x10);
    ASSERT_OFFSETOF(scene_anims_info, instance,      0x14);
    ASSERT_OFFSETOF(scene_anims_info, name,          0x18);
    ASSERT_OFFSETOF(scene_anims_info, unk_24,        0x24);
    ASSERT_OFFSETOF(scene_anims_info, status,        0x28);

    ASSERT_SIZEOF(scene_anim_light, 0x0C);

    ASSERT_SIZEOF  (scene_anim,                    0x40);
    ASSERT_OFFSETOF(scene_anim, scene_anim_inst,   0x04);
    ASSERT_OFFSETOF(scene_anim, my_anim_info_list, 0x08);
    ASSERT_OFFSETOF(scene_anim, the_handle,        0x18);
    ASSERT_OFFSETOF(scene_anim, my_name,           0x1C);
    ASSERT_OFFSETOF(scene_anim, my_flags,          0x28);
    ASSERT_OFFSETOF(scene_anim, finished,          0x2A);
    ASSERT_OFFSETOF(scene_anim, dead,              0x2B);
    ASSERT_OFFSETOF(scene_anim, paused,            0x2C);
    ASSERT_OFFSETOF(scene_anim, lights,            0x30);
} // treyarch
