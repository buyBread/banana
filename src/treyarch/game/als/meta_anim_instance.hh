#pragma once

#include "treyarch/game/als/meta_anim.hh"
#include "treyarch/game/als/param.hh"
#include "treyarch/nal/anim_instance.hh"
#include "treyarch/nal/pose.hh"
#include "treyarch/nal/transport_fk.hh"
#include "treyarch/shared/hash/algo.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/memory/fixed_pool.hh"
#include "treyarch/shared/memory/heap.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace als {
    class als_meta_anim_swing;
    class als_meta_linear_blend;

    namespace references {
        inline util::memory_reference<memory::fixed_pool> linear_blend_instance_pool { 0x00E684C0 };
        inline util::memory_reference<memory::fixed_pool> swing_instance_pool        { 0x00E68508 };
    } // references

    // what a linear blend's layer can say about it
    inline const string_hash meta_linear_param_index_id       = string_hash(hash::djb2("meta_linear_param_index"));
    inline const string_hash meta_linear_param_blend_speed_id = string_hash(hash::djb2("meta_linear_param_blend_speed"));

    // alsAnimInstance: what an als meta anim plays as
    class als_anim_instance {

    public:
        virtual ~als_anim_instance() = default;

        virtual f32        get_duration() = 0;
        virtual void       get_pose(f32 t, f32 t_prev, nal::pose* pose, nal::transport_fk* fk) = 0;
        virtual bool       is_looping();
        virtual nal::anim* get_anim();
        virtual void       on_blend_out(); // the controller starts fading it out

        // inlined into the deleting destructor
        void operator delete(void* allocation) noexcept {
            memory::heap::free(allocation);
        }
    };

    // alsMetaLinearBlendInstance
    class als_meta_linear_blend_instance : public als_anim_instance {

    public:
        nal::anim_instance**    instances;    // one per key anim, null where the key has none
        animation_logic_system* the_als;
        state_machine*          the_layer;
        als_meta_linear_blend*  meta_anim;
        bool                    follows_param; // the blend amount param is only followed until the fade out
        u8                      pad_015[0x03];

        als_meta_linear_blend_instance(als_meta_linear_blend*  the_meta_anim,
                                       nal::transport_list*    the_skel,
                                       u32                     flags,
                                       nal::skeleton_map*      skel_map,
                                       animation_logic_system* als,
                                       state_machine*          layer);
        ~als_meta_linear_blend_instance() override;

        // from a pool
        void* operator new   (std::size_t size) noexcept;
        void  operator delete(void* allocation) noexcept;

        f32        get_duration() override;
        void       get_pose(f32 t, f32 t_prev, nal::pose* pose, nal::transport_fk* fk) override;
        bool       is_looping() override;
        nal::anim* get_anim() override;
        void       on_blend_out() override;

        // samples both into the pose with the weight; the trajectory takes its own weight
        void blend_two_anims(f32                 t,
                             f32                 t_prev,
                             nal::pose*          pose,
                             f32                 weight,
                             nal::anim_instance* first,
                             nal::anim_instance* second,
                             nal::transport_fk*  fk);

        // which of the five blend amount params the layer's index picks
        static param_id get_blend_amount_param(i32 index);
    };

    // alsMetaAnimSwingInstance
    class als_meta_anim_swing_instance : public als_anim_instance {

    public:
        nal_als_instance_bridge keys[8];
        animation_logic_system* the_als;
        state_machine*          the_layer;
        als_meta_anim_swing*    meta_anim;
        f32                     unk_50;
        f32                     unk_54;
        f32                     unk_58;

        als_meta_anim_swing_instance(als_meta_anim_swing*    the_meta_anim,
                                     nal::transport_list*    the_skel,
                                     u32                     flags,
                                     nal::skeleton_map*      skel_map,
                                     animation_logic_system* als,
                                     state_machine*          layer);
        ~als_meta_anim_swing_instance() override;

        // from a pool
        void* operator new   (std::size_t size) noexcept;
        void  operator delete(void* allocation) noexcept;

        f32  get_duration() override;
        void get_pose(f32 t, f32 t_prev, nal::pose* pose, nal::transport_fk* fk) override;
    };

    ASSERT_SIZEOF  (als_anim_instance, 0x04);

    ASSERT_SIZEOF  (als_meta_linear_blend_instance,                0x18);
    ASSERT_OFFSETOF(als_meta_linear_blend_instance, instances,     0x04);
    ASSERT_OFFSETOF(als_meta_linear_blend_instance, meta_anim,     0x10);
    ASSERT_OFFSETOF(als_meta_linear_blend_instance, follows_param, 0x14);

    ASSERT_SIZEOF  (als_meta_anim_swing_instance,            0x5C);
    ASSERT_OFFSETOF(als_meta_anim_swing_instance, keys,      0x04);
    ASSERT_OFFSETOF(als_meta_anim_swing_instance, the_als,   0x44);
    ASSERT_OFFSETOF(als_meta_anim_swing_instance, meta_anim, 0x4C);
    ASSERT_OFFSETOF(als_meta_anim_swing_instance, unk_50,    0x50);
}} // treyarch::als
