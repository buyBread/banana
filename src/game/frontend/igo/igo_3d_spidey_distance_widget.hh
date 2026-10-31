#pragma once

#include "vc8/vector.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/math/algebra/vector3d.hh"
#include "treyarch/mash/string.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // one of the gauge's parts, kept sorted by name
    struct distance_gauge_part {
        mash::string name;
        u8           unk_0c;
        u8           pad_0d[0x03];
        vector3d     tint;
        i32          unk_1c;
    };

    // the gauge's arrows, five lists of parts
    struct distance_gauge_arrows {
        bool unk_00;
        bool unk_01;        // set whenever the flip is
        bool hot_cold_flip;
        u8   pad_03;

        vc8::vector
            <distance_gauge_part*> unk_04;
        vc8::vector
            <distance_gauge_part*> unk_14;
        vc8::vector
            <distance_gauge_part*> unk_24;
        vc8::vector
            <distance_gauge_part*> unk_34;
        vc8::vector
            <distance_gauge_part*> unk_44;
    };

    // what the distance natives work on
    struct distance_gauge {
        // the tint the bars go back to; a constant in retail data
        inline static const vector3d default_tint = vector3d(1.0f, 1.0f, 1.0f);

        static constexpr const char* near_part_name = "DM_GAUGE_BG";
        static constexpr const char* mid_part_name  = "DM_GAUGE_MID";
        static constexpr const char* far_part_name  = "DM_GAUGE_END";

        i32                         mode;         // 1 follow meter, 2 stealth meter
        vector3d                    offset;       // the update takes its length as the distance
        u8                          reserved_10[0x04];
        f32                         min_distance;
        f32                         max_distance;
        u8                          reserved_1c[0x0C];
        bool                        unk_28;       // the update shows the up / down arrows while it's set
        u8                          reserved_29[0x47];
        vc8::vector
            <distance_gauge_part*>* parts;
        distance_gauge_arrows*      arrows;
        u8                          reserved_78[0x08];
    };

    class igo_3d_spidey_distance_widget : public igo_3d_widget {

    public:
        distance_gauge* gauge;
        u8              reserved_154[0x0C];

        void set_min_distance(f32 distance);
        void set_max_distance(f32 distance);
        void set_offset(const vector3d &new_offset);
        void set_mode(i32 new_mode);
        void set_hot_cold_flip(bool flip);

        void set_near_tint(const vector3d &tint);
        void set_mid_tint (const vector3d &tint);
        void set_far_tint (const vector3d &tint);
        void reset_tints();
    };

    ASSERT_SIZEOF  (distance_gauge_part,       0x20);
    ASSERT_OFFSETOF(distance_gauge_part, tint, 0x10);

    ASSERT_SIZEOF  (distance_gauge_arrows,                0x54);
    ASSERT_OFFSETOF(distance_gauge_arrows, hot_cold_flip, 0x02);
    ASSERT_OFFSETOF(distance_gauge_arrows, unk_14,        0x14);
    ASSERT_OFFSETOF(distance_gauge_arrows, unk_24,        0x24);

    ASSERT_SIZEOF  (distance_gauge,               0x80);
    ASSERT_OFFSETOF(distance_gauge, offset,       0x04);
    ASSERT_OFFSETOF(distance_gauge, min_distance, 0x14);
    ASSERT_OFFSETOF(distance_gauge, max_distance, 0x18);
    ASSERT_OFFSETOF(distance_gauge, unk_28,       0x28);
    ASSERT_OFFSETOF(distance_gauge, parts,        0x70);
    ASSERT_OFFSETOF(distance_gauge, arrows,       0x74);

    ASSERT_SIZEOF  (igo_3d_spidey_distance_widget,        0x160);
    ASSERT_OFFSETOF(igo_3d_spidey_distance_widget, gauge, 0x150);
} // treyarch
