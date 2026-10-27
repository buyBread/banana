#pragma once

#include "game/gradient.hh"
#include "game/reference.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "treyarch/mash/vector_basic.hh"
#include "game/math/algebra/vector2d.hh"
#include "treyarch/math/vector.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class outdoors_unk_004 {

    public:
        gradient                      unk_00;
        mash::vector_basic<f32>       unk_10;
        mash::vector_basic
            <math::position3::packed> unk_20;
        gradient                      unk_30[4];

        // inlined @ sub_7DEC30
        void construct_mashed_class() {
            unk_00.construct_mashed_class();
            unk_10.construct_mashed_class();
            unk_20.construct_mashed_class();

            for (gradient &part : unk_30)
                part.construct_mashed_class();
        }

        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    class celestial_body {

    public:
        gradient                      color;
        mash::vector_basic<f32>       intensity;
        reference<ngl::mesh>          mesh;
        f32                           distance;
        f32                           scale;
        mash::vector_basic<f32>       scale_by_time_of_day;
        mash::vector_basic
            <math::position3::packed> position;

        // inlined @ sub_7DEC30
        void construct_mashed_class() {
            color               .construct_mashed_class();
            intensity           .construct_mashed_class();
            mesh                .construct_mashed_class();
            scale_by_time_of_day.construct_mashed_class();
            position            .construct_mashed_class();
        }

        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    class cloud_colors {

    public:
        gradient lit_colors;
        gradient shadow_colors;

        // inlined @ sub_7DEC30
        void construct_mashed_class() {
            lit_colors   .construct_mashed_class();
            shadow_colors.construct_mashed_class();
        }

        void destruct_mashed_class();

        // inlined @ sub_7DE190
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            lit_colors   .unmash(mash_info, this, buffer);
            shadow_colors.unmash(mash_info, this, buffer);
        }
    };

    class sky_prop {

    public:
        reference<ngl::mesh>          mesh;
        f32                           scale;
        f32                           duration;
        f32                           distance;
        f32                           seconds_to_look_ahead;
        math::position3::packed       minimum_world_position;
        math::position3::packed       maximum_world_position;
        mash::vector_basic
            <math::position3::packed> position;

        // inlined @ sub_7DE950
        void construct_mashed_class() {
            mesh    .construct_mashed_class();
            position.construct_mashed_class();
        }

        void destruct_mashed_class();

        // inlined @ sub_7DC580
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            mesh    .unmash(mash_info, this, buffer);
            position.unmash(mash_info, this, buffer);
        }
    };

    class outdoors {

    public:
        void*            self; // the resource handler points this at the outdoors itself
        outdoors_unk_004 unk_004[3];
        celestial_body   sun;
        celestial_body   moon;

        treyarch::cloud_colors cloud_colors;

        gradient ground_color;
        gradient skylight_color;
        gradient zenith_color;
        gradient horizon_color;
        gradient streetlight_color;

        mash::vector_basic
            <vector2d>             horizon_mad;
        mash::vector
            <sky_prop>             skyprops;

        f32 sky_distance;
        f32 sun_size;
        u32 unk_290;
        f32 cloud_min_height;
        f32 cloud_max_height;

        reference<day_sky_color>   current_day_sky_color;
        reference<night_sky_color> current_night_sky_color;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (outdoors_unk_004,        0x70);
    ASSERT_OFFSETOF(outdoors_unk_004, unk_30, 0x30);

    ASSERT_SIZEOF  (celestial_body,                       0x50);
    ASSERT_OFFSETOF(celestial_body, mesh,                 0x20);
    ASSERT_OFFSETOF(celestial_body, scale_by_time_of_day, 0x30);
    ASSERT_OFFSETOF(celestial_body, position,             0x40);

    ASSERT_SIZEOF  (cloud_colors, 0x20);

    ASSERT_SIZEOF  (sky_prop,                         0x40);
    ASSERT_OFFSETOF(sky_prop, minimum_world_position, 0x18);
    ASSERT_OFFSETOF(sky_prop, position,               0x30);

    ASSERT_SIZEOF  (outdoors,                          0x2AC);
    ASSERT_OFFSETOF(outdoors, sun,                     0x154);
    ASSERT_OFFSETOF(outdoors, moon,                    0x1A4);
    ASSERT_OFFSETOF(outdoors, cloud_colors,            0x1F4);
    ASSERT_OFFSETOF(outdoors, ground_color,            0x214);
    ASSERT_OFFSETOF(outdoors, horizon_mad,             0x264);
    ASSERT_OFFSETOF(outdoors, skyprops,                0x274);
    ASSERT_OFFSETOF(outdoors, sky_distance,            0x288);
    ASSERT_OFFSETOF(outdoors, cloud_max_height,        0x298);
    ASSERT_OFFSETOF(outdoors, current_day_sky_color,   0x29C);
    ASSERT_OFFSETOF(outdoors, current_night_sky_color, 0x2A4);
} // treyarch
