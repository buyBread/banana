#include "retail.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_text.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/quest_descriptions.hh"
#include "game/quest_list.hh"
#include "game/quest_manager.hh"

using namespace treyarch;

// sub_7EA9D0
void quest_manager::draw_text() {
    igo_3d_text* text = references::quest_text.read();

    if (!text)
        return;

    if (!references::frontend.get().igo->conversation_menu_system->is_conversation_active())
        text->render();
}

// sub_7EAA10
void quest_manager::update_progression() {
    progression_update_pending = true;
}

// sub_7EB310
void quest_manager::set_completed_quest_timers() {
    for (i32 index = 0; index < n_quests; ++index) {
        quest_state &state = quest_states[index];

        if (state.status == 8 && state.timer != 0.0f)
            state.timer = 300.0f;
    }
}

// sub_689610
void quest_manager::give_quest(const mash::string&) {}

// sub_7EACE0
i32 quest_manager::available_quest_count() {
    i32 count = 0;

    for (i32 index = 0; index < n_quests; ++index) {
        if (quest_states[index].status & 0x22)
            ++count;
    }

    return count;
}

// sub_7EAD10
mash::string quest_manager::get_available_quest(i32 n) { // a negative n gives the first quest
    for (i32 index = 0; index < n_quests; ++index) {
        if (quest_states[index].status & 0x22)
            --n;

        if (n < 0)
            return quests[index]->name;
    }

    return mash::string("");
}

// sub_7F0D30
bool quest_manager::check_completion(i32 index) {
    if (references::complete_active_quests.read())
        return true;

    for (i32 element = 0; element < 4; ++element) {
        quest_element_class_title* quest_element = quests[index]->quest_elements[element];

        if (!quest_element->type)
            continue;

        i32 progress = quest_states[index].progress[element];

        // a mission element is done at 3
        if (quest_element->type == 2 ? progress != 3 : progress < quest_element->count)
            return false;
    }

    return true;
}

// sub_7F1470
i32 quest_manager::completed_quest_count() {
    i32 count = 0;

    for (i32 index = 0; index < n_quests; ++index) {
        if (check_completion(index))
            ++count;
    }

    return count;
}

// sub_7F14A0
mash::string quest_manager::get_completed_quest(i32 n) { // a negative n gives the first quest
    for (i32 index = 0; index < n_quests; ++index) {
        if (check_completion(index))
            --n;

        if (n < 0)
            return quests[index]->name;
    }

    return mash::string("");
}

// sub_7F0CE0
i32 quest_manager::get_quest_index(const mash::string &name) { // -1 when no quest has that name
    for (i32 index = 0; index < n_quests; ++index) {
        if (quests[index]->name.is_equal(name.c_str()))
            return index;
    }

    return -1;
}

// sub_7F1610
mash::string quest_manager::get_localized_giver_name(const mash::string &giver) {
    for (i32 index = 0; index < giver_count; ++index) {
        if (givers[index]->name.is_equal(giver.c_str()))
            return givers[index]->localized_name;
    }

    // built but never printed
    mash::string message("Couldnt find localized name for giver: ");
    message += giver;

    return mash::string("");
}

// sub_7F7D80
mash::string quest_manager::get_questgiver_name(const mash::string &name) { // the turn-in giver once the quest is completed
    i32 index = get_quest_index(name);

    if (index <= -1)
        return mash::string("");

    mash::string giver;

    if (quest_states[index].status == 8)
        giver = quests[index]->turn_in;
    else
        giver = quests[index]->giver;

    return get_localized_giver_name(giver);
}

// sub_7F1510
vector3d quest_manager::get_questgiver_loc(const mash::string &name) { // where the quest's turn-in giver is
    i32 index = get_quest_index(name);

    if (index > -1) {
        mash::string turn_in(quests[index]->turn_in);

        for (i32 giver = 0; giver < giver_count; ++giver) {
            if (givers[giver]->name.is_equal(turn_in.c_str())) {
                vector3d position;
                retail::sub_90F780((i32)givers[giver]->unk_98, (i32)&position);

                return position;
            }
        }
    }

    return vector3d(0.0f, 0.0f, 0.0f);
}
