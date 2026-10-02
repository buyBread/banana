#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    namespace ngl {
        struct scene;
    } // ngl

    struct igo_color_frame {
        u8   reserved_000[0x08];
        u32* colors;
    };

    // type 1 tracks keep a float (a blur strength) where type 0 tracks keep colors
    struct igo_color_track {
        u32               type;
        igo_color_frame** frames;
    };

    struct igo_color_channel {
        igo_color_track* track;
        u32              frame_index;
        u32              reserved_008;
        f32              interpolation;

        u32         sample_color(u32 color_index);
        ngl::scene* draw_fullscreen_quad();
    };

    // a null-terminated channel array
    struct igo_color_channel_set {
        igo_color_channel** channels;
    };

    // background-effect mode 11
    void draw_background_channels();

    namespace references {
        inline util::memory_reference<i32> unk_0102cdb8 { 0x0102CDB8 }; // nothing is drawn unless positive, then from background_channel_sets[1]

        inline util::memory_reference<igo_color_channel_set*> background_channel_sets { 0x00E6BFC8 }; // two
    } // references

    ASSERT_SIZEOF  (igo_color_frame,         0x0C);
    ASSERT_OFFSETOF(igo_color_frame, colors, 0x08);

    ASSERT_SIZEOF  (igo_color_track,         0x08);
    ASSERT_OFFSETOF(igo_color_track, frames, 0x04);
    
    ASSERT_SIZEOF  (igo_color_channel,                0x10);
    ASSERT_OFFSETOF(igo_color_channel, track,         0x00);
    ASSERT_OFFSETOF(igo_color_channel, frame_index,   0x04);
    ASSERT_OFFSETOF(igo_color_channel, interpolation, 0x0C);
} // treyarch
