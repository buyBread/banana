#pragma once

#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/mash/vector_basic.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    struct conversation_condition {
        i32          type;
        mash::string quest;

        // inlined @ sub_6F59E0
        // inlined @ sub_6F9010
        void construct_mashed_class() {
            quest.construct_mashed_class();
        }

        // inlined @ sub_6CC5D0
        // inlined @ sub_6CC660
        void destruct_mashed_class() {
            quest.destruct_mashed_class();
        }

        // inlined @ sub_6E6E10
        // inlined @ sub_6E6EE0
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            quest.unmash(mash_info, this, buffer);
        }
    };

    struct conversation_state {
        mash::string           name;
        mash::string           text;
        mash::string           quest;
        i32                    unk_24;
        conversation_condition condition; // the state is only offered while this holds
        i32                    depth;     // children sit one level deeper than their parent
        i32                    unk_3c;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);

        void localize_strings(bool enabled);
    };

    class conversation {

    public:
        void* self; // the resource handler points this at the conversation itself

        i32                     hero_speaker; // the other side of every exchange, 10 (Spiderman Red) in every file
        mash::vector_basic<i32> speakers;     // indices into the ui character info list

        u8    unk_18;
        bool  unk_19; // set when the quest manager attaches the conversation
        u8    pad_1a[2];

        mash::vector
            <conversation_condition> conditions;
        mash::vector
            <conversation_state>     states;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (conversation_condition, 0x10);

    ASSERT_SIZEOF  (conversation_state,            0x40);
    ASSERT_OFFSETOF(conversation_state, text,      0x0C);
    ASSERT_OFFSETOF(conversation_state, quest,     0x18);
    ASSERT_OFFSETOF(conversation_state, unk_24,    0x24);
    ASSERT_OFFSETOF(conversation_state, condition, 0x28);
    ASSERT_OFFSETOF(conversation_state, depth,     0x38);
    ASSERT_OFFSETOF(conversation_state, unk_3c,    0x3C);

    ASSERT_SIZEOF  (conversation,             0x44);
    ASSERT_OFFSETOF(conversation, speakers,   0x08);
    ASSERT_OFFSETOF(conversation, unk_18,     0x18);
    ASSERT_OFFSETOF(conversation, unk_19,     0x19);
    ASSERT_OFFSETOF(conversation, conditions, 0x1C);
    ASSERT_OFFSETOF(conversation, states,     0x30);
} // treyarch
