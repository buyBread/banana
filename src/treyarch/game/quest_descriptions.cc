#include "treyarch/game/quest_descriptions.hh"
#include "treyarch/shared/mash/mash_info.hh"

using namespace treyarch;

// sub_7EB3B0
// inlined @ sub_7EB910
// inlined @ sub_7FE0B0
void quest_condition::construct_mashed_class() {
    term.construct_mashed_class();

    if (unk_18)
        mash::mash_info_struct::construct_class(unk_18);

    if (unk_1c)
        mash::mash_info_struct::construct_class(unk_1c);
}

// sub_7EB3D0
void quest_condition::destruct_mashed_class() {
    term.destruct_mashed_class();

    if (unk_18) {
        unk_18->destruct_mashed_class();
        unk_18 = nullptr;
    }

    if (unk_1c) {
        unk_1c->destruct_mashed_class();
        unk_1c = nullptr;
    }
}

// sub_7F93C0
void quest_condition::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    term.unmash(mash_info, this, buffer);

    u8 class_mashed;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(unk_18, this, buffer);
    else
        unk_18 = nullptr;

    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(unk_1c, this, buffer);
    else
        unk_1c = nullptr;
}

// sub_7FE0B0
void quest_description::construct_mashed_class() {
    name                  .construct_mashed_class();
    giver                 .construct_mashed_class();
    turn_in               .construct_mashed_class();
    title                 .construct_mashed_class();
    unk_30                .construct_mashed_class();
    ask_about_conversation.construct_mashed_class();
    accept                .construct_mashed_class();
    decline               .construct_mashed_class();
    turn_in_conversation  .construct_mashed_class();
    stale                 .construct_mashed_class();
    unk_98                .construct_mashed_class();
    spawn_tables          .construct_mashed_class();
    alternate_giver       .construct_mashed_class();
    reminders             .construct_mashed_class();

    for (quest_element_class_title* element : quest_elements) {
        if (element)
            element->construct_mashed_class();
    }

    if (unk_110)
        unk_110->construct_mashed_class();
}

// sub_7FAE70
void quest_description::destruct_mashed_class() {
    name                  .destruct_mashed_class();
    giver                 .destruct_mashed_class();
    turn_in               .destruct_mashed_class();
    title                 .destruct_mashed_class();
    unk_30                .destruct_mashed_class();
    ask_about_conversation.destruct_mashed_class();
    accept                .destruct_mashed_class();
    decline               .destruct_mashed_class();
    turn_in_conversation  .destruct_mashed_class();
    stale                 .destruct_mashed_class();
    unk_98                .destruct_mashed_class();
    spawn_tables          .destruct_mashed_class();
    alternate_giver       .destruct_mashed_class();
    reminders             .destruct_mashed_class();

    for (quest_element_class_title* &element : quest_elements) {
        if (element) {
            element->destruct_mashed_class();
            element = nullptr;
        }
    }

    if (unk_110) {
        unk_110->destruct_mashed_class();
        unk_110 = nullptr;
    }
}

// sub_800830
void quest_description::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    name                  .unmash(mash_info, this, buffer);
    giver                 .unmash(mash_info, this, buffer);
    turn_in               .unmash(mash_info, this, buffer);
    title                 .unmash(mash_info, this, buffer);
    unk_30                .unmash(mash_info, this, buffer);
    ask_about_conversation.unmash(mash_info, this, buffer);
    accept                .unmash(mash_info, this, buffer);
    decline               .unmash(mash_info, this, buffer);
    turn_in_conversation  .unmash(mash_info, this, buffer);
    stale                 .unmash(mash_info, this, buffer);
    unk_98                .unmash(mash_info, this, buffer);
    spawn_tables          .unmash(mash_info, this, buffer);
    alternate_giver       .unmash(mash_info, this, buffer);
    reminders             .unmash(mash_info, this, buffer);

    u8 class_mashed;

    for (quest_element_class_title* &element : quest_elements) {
        mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

        if (class_mashed == mash::member_class_mashed)
            mash_info->unmash_class(element, this, buffer);
        else
            element = nullptr;
    }

    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(unk_110, this, buffer);
    else
        unk_110 = nullptr;
}

// sub_8023C0
void quest_descriptions::construct_mashed_class() {
    self = nullptr;

    quests.construct_mashed_class();
}

// sub_802150
void quest_descriptions::destruct_mashed_class() {
    quests.destruct_mashed_class();
}

// sub_802880
void quest_descriptions::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    quests.unmash(mash_info, this, buffer);
}
