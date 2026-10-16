#pragma once

#include "treyarch/game/gradient.hh"
#include "treyarch/game/reference.hh"
#include "treyarch/game/sm3_light.hh"
#include "treyarch/shared/boolx.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/mash/vector_basic.hh"
#include "treyarch/shared/math/types/color.hh"
#include "treyarch/shared/math/types/vector3.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    // one time of day's ambient light
    struct sphere {
        color up[8];
        color down[8];

        void construct_mashed_class() {}
        void destruct_mashed_class() {}
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}
    };

    struct ambient {
        mash::vector<sphere> times_of_day;
        u32                  unk_14[4];

        // inlined @ sub_7DE310
        void construct_mashed_class() {
            times_of_day.construct_mashed_class();
        }

        // inlined @ sub_7DCF10
        void destruct_mashed_class() {
            times_of_day.destruct_mashed_class();
        }

        // inlined @ sub_7DD630
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            times_of_day.unmash(mash_info, this, buffer);
        }
    };

    struct environment_adjustment {
        u32 unk_00[12];
    };

    struct blur {
        mash::vector_basic<f32> brightness_multiplier;
        f32                     duration;

        // inlined @ sub_7DE310
        void construct_mashed_class() {
            brightness_multiplier.construct_mashed_class();

            duration = 1.0f;
        }

        // inlined @ sub_7DCF10
        void destruct_mashed_class() {
            brightness_multiplier.destruct_mashed_class();
        }

        // inlined @ sub_7DD630
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            brightness_multiplier.unmash(mash_info, this, buffer);
        }
    };

    class fog {

    public:
        mash::vector_basic<vector3> position0;
        mash::vector_basic<vector3> position1;
        mash::vector_basic<f32>     fogs_per_meter;
        mash::vector_basic<f32>     maximum_fogs;
        gradient                    color;
        mash::vector_basic<f32>     depth_start;
        mash::vector_basic<f32>     depth_end;
        mash::vector_basic<f32>     depth_power;
        mash::vector_basic<f32>     depth_opacity;
        mash::vector_basic<f32>     depth_overide;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    class environment_unk_148 {

    public:
        u32                     unk_000[4];
        mash::vector_basic<f32> unk_010[16];
        gradient                unk_110[3];
        mash::vector_basic<f32> unk_140[4];

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    class environment {

    public:
        void* self; // the resource handler points this at the environment itself

        treyarch::ambient ambient;

        mash::vector_basic
            <environment_adjustment> adjustment;
        mash::vector_basic
            <environment_adjustment> material_adjustment;
        mash::vector_basic
            <environment_adjustment> character_adjustment;
        mash::vector_basic
            <environment_adjustment> character_ambient_light_adjustment;
        mash::vector_basic
            <environment_adjustment> character_diffuse_light_adjustment;

        reference<treyarch::outdoors> outdoors;
        mash::vector<sm3_light>       light;
        treyarch::blur                blur;
        treyarch::fog                 fog;
        environment_unk_148           unk_148[3];
        boolx                         auto_exposure;
        u8                            pad_5c9[3];
        mash::vector
            <reference<environment>>  alternates;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (sphere, 0x100);

    ASSERT_SIZEOF  (ambient,         0x24);
    ASSERT_OFFSETOF(ambient, unk_14, 0x14);

    ASSERT_SIZEOF  (environment_adjustment, 0x30);

    ASSERT_SIZEOF  (blur,           0x14);
    ASSERT_OFFSETOF(blur, duration, 0x10);

    ASSERT_SIZEOF  (fog,                0xA0);
    ASSERT_OFFSETOF(fog, color,         0x40);
    ASSERT_OFFSETOF(fog, depth_start,   0x50);
    ASSERT_OFFSETOF(fog, depth_overide, 0x90);

    ASSERT_SIZEOF  (environment_unk_148,          0x180);
    ASSERT_OFFSETOF(environment_unk_148, unk_010, 0x010);
    ASSERT_OFFSETOF(environment_unk_148, unk_110, 0x110);
    ASSERT_OFFSETOF(environment_unk_148, unk_140, 0x140);

    ASSERT_SIZEOF  (environment,                                     0x5E0);
    ASSERT_OFFSETOF(environment, ambient,                            0x004);
    ASSERT_OFFSETOF(environment, adjustment,                         0x028);
    ASSERT_OFFSETOF(environment, character_diffuse_light_adjustment, 0x068);
    ASSERT_OFFSETOF(environment, outdoors,                           0x078);
    ASSERT_OFFSETOF(environment, light,                              0x080);
    ASSERT_OFFSETOF(environment, blur,                               0x094);
    ASSERT_OFFSETOF(environment, fog,                                0x0A8);
    ASSERT_OFFSETOF(environment, unk_148,                            0x148);
    ASSERT_OFFSETOF(environment, auto_exposure,                      0x5C8);
    ASSERT_OFFSETOF(environment, alternates,                         0x5CC);
} // treyarch
