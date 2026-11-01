#pragma once

#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // a handle on one posted sound event
    class sound_response {

    public:
        u32 unk_00;
        u32 unk_04;
        u32 unk_08; // the callback the release cancels
        u8  unk_0c;
        u8  pad_0d[0x03];

        // inlined @ sub_4D07D0
        // inlined @ sub_8663E0
        ~sound_response() {
            release(false);
        }

        void release(bool clear_event);
    };

    ASSERT_SIZEOF  (sound_response,         0x10);
    ASSERT_OFFSETOF(sound_response, unk_08, 0x08);
} // treyarch
