#pragma once

#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    struct anim_event_param {
        u32 type; // 1 a hash or handle, 2 a float, 4 an integer
        u32 value;
    };

    // an event subclass; the native constructor and destructor build and free it with retail's vtable
    class anim_event {

    public:
        void**            vtable;
        string_hash       event_type_id;
        u32               raised_frame;
        bool              autokill;
        bool              from_mash;
        u8                pad_00e[0x02];
        string_hash       bone;
        anim_event_param* params;
        u32               num_params;
        f32               unk_1c;

        anim_event(string_hash event_type_id, string_hash bone, u32 num_params, f32 unk_1c);
        ~anim_event();
    };

    ASSERT_SIZEOF  (anim_event,             0x20);
    ASSERT_OFFSETOF(anim_event, bone,       0x10);
    ASSERT_OFFSETOF(anim_event, params,     0x14);
    ASSERT_OFFSETOF(anim_event, num_params, 0x18);
    ASSERT_OFFSETOF(anim_event, unk_1c,     0x1C);
} // treyarch
