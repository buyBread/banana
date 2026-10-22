#pragma once

#include "treyarch/game/reference.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/virtual_base.hh"
#include "treyarch/shared/math/vmath/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    // the base of every uv animation; mashed ones get their retail vtable from the type registry
    class uv_animation : public mash::mash_virtual_base {};

    class sm_decal_param_layer {

    public:
        enum e_wrap_mode : i32 {
            wrap_mode_wrap,
            wrap_mode_clamp,
            wrap_mode_mirror,
            wrap_mode_border
        };

        reference<ngl::texture> diffuse_map;
        reference<ngl::texture> normal_map;
        u32                     unk_18[20];
        treyarch::uv_animation* uv_animation;
        f32                     color_adjustments[20];
        e_wrap_mode             wrap_mode;

        // inlined @ sub_7D9A10
        void construct_mashed_class() {
            diffuse_map.construct_mashed_class();
            normal_map .construct_mashed_class();

            if (uv_animation)
                uv_animation = (treyarch::uv_animation*)mash::mash_virtual_base::construct_class_helper(uv_animation);
        }

        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    class sm_decal_param {

    public:
        enum e_shadow_mode : i32 {
            shadow_mode_receive_only,
            shadow_mode_cast_and_receive,
            shadow_mode_cast_only,
            shadow_mode_exempt
        };

        enum e_blend_mode : i32 {
            blend_mode_opaque,
            blend_mode_blend,
            blend_mode_add,
            blend_mode_subtract
        };

        void*                   self; // the resource handler points this at the parameters themselves
        e_shadow_mode           shadow_mode;
        e_blend_mode            blend_mode;
        sm_decal_param_layer    diffuse;
        sm_decal_param_layer    decal;
        reference<ngl::texture> environment_map;
        u32                     unk_198[4];
        math::vector4::packed   fresnel;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (uv_animation, 0x04);

    ASSERT_SIZEOF  (sm_decal_param_layer,                    0xC0);
    ASSERT_OFFSETOF(sm_decal_param_layer, normal_map,        0x0C);
    ASSERT_OFFSETOF(sm_decal_param_layer, uv_animation,      0x68);
    ASSERT_OFFSETOF(sm_decal_param_layer, color_adjustments, 0x6C);
    ASSERT_OFFSETOF(sm_decal_param_layer, wrap_mode,         0xBC);

    ASSERT_SIZEOF  (sm_decal_param,                  0x1B8);
    ASSERT_OFFSETOF(sm_decal_param, diffuse,         0x00C);
    ASSERT_OFFSETOF(sm_decal_param, decal,           0x0CC);
    ASSERT_OFFSETOF(sm_decal_param, environment_map, 0x18C);
    ASSERT_OFFSETOF(sm_decal_param, fresnel,         0x1A8);
} // treyarch
