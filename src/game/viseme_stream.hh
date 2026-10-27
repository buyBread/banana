#pragma once

#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector_basic.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // lip sync weights; RTTI's VisemeStream
    class viseme_stream {

    public:
        void* self; // the resource handler points this at the stream itself

        u32                     viseme_count; // weights per frame
        u32                     unk_08;       // further columns per frame, skipped by the milestone's text loader
        u32                     frame_count;
        i32                     frame_rate;
        f32                     time_offset;  // minus the first frame's time
        mash::vector_basic<u16> weights;      // frame-major; 0..5 scaled by 13107

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (viseme_stream,              0x28);
    ASSERT_OFFSETOF(viseme_stream, frame_count, 0x0C);
    ASSERT_OFFSETOF(viseme_stream, time_offset, 0x14);
    ASSERT_OFFSETOF(viseme_stream, weights,     0x18);
} // treyarch
