#pragma once

#include "treyarch/ngl/texture/texture.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    // planes follow BINKFRAMEBUFFERS: luma, alpha, chroma red, chroma blue
    struct movie_textures {
        ngl::texture* upload_y;
        ngl::texture* upload_a;
        ngl::texture* upload_cr;
        ngl::texture* upload_cb;
        ngl::texture* frame_y[2];
        ngl::texture* frame_a[2];
        ngl::texture* frame_cr[2];
        ngl::texture* frame_cb[2];
        u8            frame_decoded[2];
    };

    class movie_manager {

    public:
        void* vtable;
        i32   state;
        i32   unk_008;
        void* bink;

        bool is_playing() const {
            return state == 1;
        }

        void release_device_resources();
        void restore_device_resources();

        static void release_textures();
    };

    ASSERT_OFFSETOF(movie_manager, state, 0x04);
    ASSERT_OFFSETOF(movie_manager, bink,  0x0C);

    ASSERT_OFFSETOF(movie_textures, upload_y,      0x00);
    ASSERT_OFFSETOF(movie_textures, upload_cb,     0x0C);
    ASSERT_OFFSETOF(movie_textures, frame_y,       0x10);
    ASSERT_OFFSETOF(movie_textures, frame_a,       0x18);
    ASSERT_OFFSETOF(movie_textures, frame_cr,      0x20);
    ASSERT_OFFSETOF(movie_textures, frame_cb,      0x28);
    ASSERT_OFFSETOF(movie_textures, frame_decoded, 0x30);

    namespace references {
        inline util::memory_reference<movie_textures> movie_textures { 0x0102CDDC };
        inline util::memory_reference<movie_manager*> movie_manager  { 0x0102F2DC };
    } // references
} // treyarch
