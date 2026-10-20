#pragma once

#include "treyarch/shared/fixed_string.hh"
#include "treyarch/shared/four_cc.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace nal {
    constexpr u32 anim_type    = four_cc('A', 'N', 'I', 'M');
    constexpr u32 anim_version = 0x20203;

    enum e_anim_flags : u32 {
        anim_flag_looping = 0x01,
        anim_flag_unk_04  = 0x04  // set when the entry loads
    };

    struct transport_data;
    struct transport_list;

    struct rescale {
        f32 scale;
        u32 offset;  // bytes into the pose data
    };

    // ANIM
    struct anim {
        fixed_string         name;
        anim*                next;
        fixed_string         transport_list_name;
        u32                  version;
        u32                  transport_list_revision;
        nal::transport_list* transport_list;
        u32                  flags;
        f32                  duration;
        f32                  frame_time;
        u32                  ref_count;
        u32                  frame_count;
        nal::transport_data* transport_data;
        u32                  total_field_values_encoded;
        u32                  rescale_count;
        rescale*             rescales;
        u32*                 bitstream;
        u16*                 lane_streams;
        u32                  lane_starts[8];
        u32                  initial_bit_offset;
        u16*                 takeout_tracks;
        u16                  takeout_count;
        u32                  present_mask[13];
        u32                  constant_mask[13];

        void link_transport_list();
    };

    ASSERT_SIZEOF  (rescale,         0x08);
    ASSERT_OFFSETOF(rescale, scale,  0x00);
    ASSERT_OFFSETOF(rescale, offset, 0x04);

    ASSERT_SIZEOF  (anim,                             0xE0);
    ASSERT_OFFSETOF(anim, name,                       0x00);
    ASSERT_OFFSETOF(anim, next,                       0x08);
    ASSERT_OFFSETOF(anim, transport_list_name,        0x0C);
    ASSERT_OFFSETOF(anim, version,                    0x14);
    ASSERT_OFFSETOF(anim, transport_list_revision,    0x18);
    ASSERT_OFFSETOF(anim, transport_list,             0x1C);
    ASSERT_OFFSETOF(anim, flags,                      0x20);
    ASSERT_OFFSETOF(anim, duration,                   0x24);
    ASSERT_OFFSETOF(anim, frame_time,                 0x28);
    ASSERT_OFFSETOF(anim, ref_count,                  0x2C);
    ASSERT_OFFSETOF(anim, frame_count,                0x30);
    ASSERT_OFFSETOF(anim, transport_data,             0x34);
    ASSERT_OFFSETOF(anim, total_field_values_encoded, 0x38);
    ASSERT_OFFSETOF(anim, rescale_count,              0x3C);
    ASSERT_OFFSETOF(anim, rescales,                   0x40);
    ASSERT_OFFSETOF(anim, bitstream,                  0x44);
    ASSERT_OFFSETOF(anim, lane_streams,               0x48);
    ASSERT_OFFSETOF(anim, lane_starts,                0x4C);
    ASSERT_OFFSETOF(anim, initial_bit_offset,         0x6C);
    ASSERT_OFFSETOF(anim, takeout_tracks,             0x70);
    ASSERT_OFFSETOF(anim, takeout_count,              0x74);
    ASSERT_OFFSETOF(anim, present_mask,               0x78);
    ASSERT_OFFSETOF(anim, constant_mask,              0xAC);
}} // treyarch::nal
