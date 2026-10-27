#pragma once

#include "treyarch/mash/string.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    struct quest_element_class_title {
        i32          type;
        mash::string thing;
        i32          count;
        mash::string text;

        // inlined @ sub_7FE0B0
        void construct_mashed_class() {
            thing.construct_mashed_class();
            text .construct_mashed_class();
        }

        // inlined @ sub_7FAE70
        void destruct_mashed_class() {
            thing.destruct_mashed_class();
            text .destruct_mashed_class();
        }

        // inlined @ sub_800830
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            thing.unmash(mash_info, this, buffer);
            text .unmash(mash_info, this, buffer);
        }
    };

    struct quest_condition {
        u32              unk_00[3];
        mash::string     term;
        quest_condition* unk_18;
        quest_condition* unk_1c;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    struct quest_description {
        mash::string               name;
        mash::string               giver;
        mash::string               turn_in;
        mash::string               title;
        mash::string               unk_30;
        mash::vector<mash::string> ask_about_conversation;
        mash::string               accept;
        mash::string               decline;
        mash::vector<mash::string> turn_in_conversation;
        mash::string               stale;
        f32                        unk_88;
        i32                        unk_8c;
        bool                       unk_90;
        bool                       unk_91;
        u8                         pad_92[2];
        i32                        unk_94;
        mash::string               unk_98;
        i32                        unk_a4; // maybe act?
        mash::vector<mash::string> spawn_tables;
        f32                        time_limit;
        quest_element_class_title* quest_elements[4];
        f32                        unk_d0;
        f32                        unk_d4;
        f32                        awarded_xp;
        f32                        unk_dc;
        i32                        unk_e0;
        bool                       elements_are_sequential;
        bool                       unk_e5;
        bool                       unk_e6;
        bool                       unk_e7;
        bool                       unk_e8;
        u8                         pad_e9[3];
        mash::string               alternate_giver;
        mash::vector<mash::string> reminders;
        bool                       unk_10c;
        u8                         pad_10d[3];
        quest_condition*           unk_110;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    class quest_descriptions {

    public:
        void* self; // the resource handler points this at the descriptions themselves

        u32 unk_04;

        mash::vector
            <quest_description> quests;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (quest_element_class_title,         0x20);
    ASSERT_OFFSETOF(quest_element_class_title, count,  0x10);
    ASSERT_OFFSETOF(quest_element_class_title, text,   0x14);

    ASSERT_SIZEOF  (quest_condition,         0x20);
    ASSERT_OFFSETOF(quest_condition, term,   0x0C);
    ASSERT_OFFSETOF(quest_condition, unk_18, 0x18);

    ASSERT_SIZEOF  (quest_description,                          0x114);
    ASSERT_OFFSETOF(quest_description, ask_about_conversation,  0x03C);
    ASSERT_OFFSETOF(quest_description, turn_in_conversation,    0x068);
    ASSERT_OFFSETOF(quest_description, stale,                   0x07C);
    ASSERT_OFFSETOF(quest_description, unk_88,                  0x088);
    ASSERT_OFFSETOF(quest_description, unk_94,                  0x094);
    ASSERT_OFFSETOF(quest_description, unk_98,                  0x098);
    ASSERT_OFFSETOF(quest_description, spawn_tables,            0x0A8);
    ASSERT_OFFSETOF(quest_description, time_limit,              0x0BC);
    ASSERT_OFFSETOF(quest_description, quest_elements,          0x0C0);
    ASSERT_OFFSETOF(quest_description, awarded_xp,              0x0D8);
    ASSERT_OFFSETOF(quest_description, elements_are_sequential, 0x0E4);
    ASSERT_OFFSETOF(quest_description, alternate_giver,         0x0EC);
    ASSERT_OFFSETOF(quest_description, reminders,               0x0F8);
    ASSERT_OFFSETOF(quest_description, unk_10c,                 0x10C);
    ASSERT_OFFSETOF(quest_description, unk_110,                 0x110);

    ASSERT_SIZEOF  (quest_descriptions,         0x1C);
    ASSERT_OFFSETOF(quest_descriptions, quests, 0x08);
} // treyarch
