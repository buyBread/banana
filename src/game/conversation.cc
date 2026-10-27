#include "game/app.hh"
#include "game/conversation.hh"
#include "game/localized_string_table.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/mash_info.hh"

using namespace treyarch;

// inlined @ sub_6F59E0
void conversation_state::construct_mashed_class() {
    name     .construct_mashed_class();
    text     .construct_mashed_class();
    quest    .construct_mashed_class();
    condition.construct_mashed_class();

    localize_strings(true);
}

// inlined @ sub_6CC660
void conversation_state::destruct_mashed_class() {
    name     .destruct_mashed_class();
    text     .destruct_mashed_class();
    quest    .destruct_mashed_class();
    condition.destruct_mashed_class();
}

// inlined @ sub_6E6EE0
void conversation_state::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    name     .unmash(mash_info, this, buffer);
    text     .unmash(mash_info, this, buffer);
    quest    .unmash(mash_info, this, buffer);
    condition.unmash(mash_info, this, buffer);
}

// sub_6B3F60
void conversation_state::localize_strings(bool enabled) { // swaps global text ids for their localized text
    if (!enabled)
        return;

    if (!name.is_equal("")) {
        string_hash id;
        id.initialize(mash::ALLOCATED, name.c_str());

        i32 index = find_global_text(id);

        if (index >= 0)
            name = references::game.read()->string_localizer->lookup_localized_string(index);
    }

    if (!text.is_equal("")) {
        string_hash id;
        id.initialize(mash::ALLOCATED, text.c_str());

        i32 index = find_global_text(id);

        if (index >= 0)
            text = references::game.read()->string_localizer->lookup_localized_string(index);
    }
}

// sub_6F9010
void conversation::construct_mashed_class() {
    self = nullptr;

    speakers  .construct_mashed_class();
    conditions.construct_mashed_class();
    states    .construct_mashed_class();
}

// sub_6DE160
void conversation::destruct_mashed_class() {
    speakers  .destruct_mashed_class();
    conditions.destruct_mashed_class();
    states    .destruct_mashed_class();
}

// sub_6F9070
void conversation::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    speakers  .unmash(mash_info, this, buffer);
    conditions.unmash(mash_info, this, buffer);
    states    .unmash(mash_info, this, buffer);
}
