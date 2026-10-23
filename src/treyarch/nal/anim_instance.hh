#pragma once

#include <cstddef>

#include "treyarch/nal/anim.hh"
#include "treyarch/nal/nal2_decoder_state.hh"
#include "treyarch/nal/pose.hh"
#include "treyarch/nal/transport_fk.hh"
#include "treyarch/shared/memory/heap.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace nal {
    struct anim_instance;
    struct skeleton_map;

    // the most encoded values a q-pose holds
    constexpr u32 max_pose_tracks = 1024;

    enum e_instance_flags : u32 {
        instance_flag_mirror = 0x01
    };

    struct instance_observer_link {
        instance_observer_link* next;
        instance_observer_link* prev;
    };

    // told about every instance's creation and destruction
    struct instance_observer : instance_observer_link {
        virtual void on_create(anim_instance* instance)  = 0;
        virtual void on_destroy(anim_instance* instance) = 0;
    };

    // a ring that nothing ever joins
    inline instance_observer_link instance_observers = { &instance_observers, &instance_observers };

    // counted by get_pose at a time, never read
    inline u32 single_frame_poses;
    inline u32 blended_poses;

#pragma pack(push, 1)
    // nalAnimInstance
    struct anim_instance {
        nal::po                  last_trajectory_po;
        bool                     last_trajectory_po_initialized;
        u32                      flags;
        nal::skeleton_map*       skeleton_map;  // may be null
        nal::anim*               anim;
        nal::nal2_decoder_state* decoder;
        anim_instance*           next;          // a scene anim's chain
        bool                     force_looping;
        u8                       pad_36[0x0A];

        anim_instance(nal::anim* animation, u32 instance_flags, nal::skeleton_map* rig_map);
        ~anim_instance();

        void get_pose(i32 frame, nal::pose* pose);
        void get_pose(f32 t, f32 t_prev, nal::pose* pose, nal::transport_fk* fk);
        void get_q_pose(u32 t, i32* q_pose);
        void unshuffle_q_pose(i32* q_pose);
        void remap_pose(nal::pose* source, nal::pose* target);

        void* operator new(std::size_t size) noexcept {
            return memory::heap::allocate(size);
        }

        void operator delete(void* allocation) noexcept {
            memory::heap::free(allocation);
        }
    };
#pragma pack(pop)

    void mirror_pose(nal::anim_instance* instance, nal::pose* pose);

    ASSERT_SIZEOF  (anim_instance,                                 0x40);
    ASSERT_OFFSETOF(anim_instance, last_trajectory_po,             0x00);
    ASSERT_OFFSETOF(anim_instance, last_trajectory_po_initialized, 0x20);
    ASSERT_OFFSETOF(anim_instance, flags,                          0x21);
    ASSERT_OFFSETOF(anim_instance, skeleton_map,                   0x25);
    ASSERT_OFFSETOF(anim_instance, anim,                           0x29);
    ASSERT_OFFSETOF(anim_instance, decoder,                        0x2D);
    ASSERT_OFFSETOF(anim_instance, next,                           0x31);
    ASSERT_OFFSETOF(anim_instance, force_looping,                  0x35);
}} // treyarch::nal
