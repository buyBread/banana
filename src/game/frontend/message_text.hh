#pragma once

#include "treyarch/mash/string.hh"
#include "treyarch/mm/mm.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // time runs to end_time: the alpha rises until fade_in_end and falls from fade_out_start, a negative fade_out_start holds it
    class message_text {

    public:
        union {
            mash::string text; // the destructor finalizes it
        };
        f32 end_time;
        f32 time;
        u8  unk_18;            // cleared once time passes end_time
        u8  unk_19;
        u8  pad_1a[0x02];
        f32 alpha;
        f32 fade_in_end;
        f32 fade_out_start;
        i32 unk_28;
        u8  unk_2c;
        u8  pad_2d[0x03];

        message_text();

        virtual ~message_text();

        // inlined into the deleting destructor
        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
        }

        virtual void update(f32 time_inc);

        void          set_text(mash::string value);
        mash::string* get_text(mash::string* result); // a copy of the text

        void clear();

        void replace_pad_tokens();

        static void substitute_pad_tokens(mash::string* value);
    };

    ASSERT_SIZEOF  (message_text,                 0x30);
    ASSERT_OFFSETOF(message_text, text,           0x04);
    ASSERT_OFFSETOF(message_text, end_time,       0x10);
    ASSERT_OFFSETOF(message_text, alpha,          0x1C);
    ASSERT_OFFSETOF(message_text, fade_out_start, 0x24);
} // treyarch
