#include <new>

#include "retail.hh"
#include "game/app.hh"
#include "game/event/event_callback.hh"
#include "game/frontend/conversation_menu_system.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_face_button_system.hh"
#include "game/frontend/igo/igo_3d_loading_screen.hh"
#include "game/frontend/igo/igo_3d_text.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/game.hh"
#include "game/mission/mission_manager.hh"
#include "game/quest_descriptions.hh"
#include "game/quest_list.hh"
#include "game/quest_manager.hh"
#include "game/region_pack_manager.hh"
#include "game/spawn/quest_giver_spawn.hh"
#include "game/trigger.hh"
#include "game/wds/ai/ai_basic_inode.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/interface/physical_interface.hh"
#include "game/wds/entity/interface/standing_interface.hh"
#include "game/wds/references.hh"
#include "treyarch/mm/mm.hh"

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

        for (i32 giver = 0; giver < giver_count; ++giver)
            if (givers[giver]->name.is_equal(turn_in.c_str()))
                return givers[giver]->spawn->get_position();
    }

    return vector3d(0.0f, 0.0f, 0.0f);
}

// sub_801790
void quest_manager::frame_advance(f32 time_inc) {
    mission_manager* missions = mission_manager::inst();

    if (wait_for_idle_mission && !missions->is_idle())
        return;

    wait_for_idle_mission = false;

    if (missions->is_act_transitioning())
        return;

    wait_for_idle_mission = false;

    for (i32 index = 0; index < n_quests; ++index) {
        quest_state &state = quest_states[index];

        if (state.status == 4) {
            state.timer = (f32)((f64)state.timer + (f64)time_inc);

            if (check_completion(index)) {
                retail::sub_7FD960((i32)this, index); // the quest's elements are met

                if (references::complete_active_quests.read())
                    retail::sub_7EB230((u32*)this, index);
            } else if (retail::sub_7F0DA0((u32*)this, index)) // failed
                retail::sub_7FAA20((i16*)this, index);
            else if (!retail::sub_7EB2E0((u32*)this, index) && !(quests[index]->time_limit > 0.0f)) {
                if (missions->is_mission_running())
                    state.timer = 0.0f;

                f32 reminder_time = quests[index]->unk_88;

                if (reminder_time != -1.0f && (f64)state.timer > (f64)reminder_time) {
                    if (!retail::sub_7DF520((u32*)quests[index]))
                        retail::sub_7F1C10((u32*)this, index, false); // the reminder

                    state.timer = 0.0f;
                }
            }
        } else if (state.status == 8) {
            if (missions->is_mission_running())
                state.timer = 0.0f;

            state.timer = (f32)((f64)state.timer + (f64)time_inc);

            if (state.timer > 300.0f) {
                i32 kind = quests[index]->unk_8c;

                if (kind != 3 && kind != 4 && kind != 6 && kind != 7)
                    retail::sub_7F1C10((u32*)this, index, true); // the turn-in reminder

                state.timer = 0.0f;
            }
        }
    }

    if (references::complete_active_quests.read())
        references::complete_active_quests.write(0);

    if (references::pick_up_available_quests.read())
        references::pick_up_available_quests.write(0);

    retail::sub_7F7BB0((u8*)this);
    retail::sub_7FFD90((i32)this); // re-checks the unlock rules

    if (references::quest_text.read())
        references::quest_text.read()->set_text_no_localize("");

    retail::sub_7F8DD0((u32*)this);
    retail::sub_7EAB70((u32*)this);

    if (unk_080 > -1) {
        u32 status = quest_states[unk_080].status;

        if ((status == 8 || status == 16) && missions->is_idle())
            retail::sub_9860C0((u32*)missions);
    }

    if (deferred_missions.size() && !missions->is_mission_running()) {
        for (auto* position = deferred_missions.begin(); position != deferred_missions.end(); position = position->next)
            retail::sub_985CD0((u32*)missions, (i32)position->value.c_str());

        deferred_missions.clear();
    }

    if (!hold_blackscreen)
        return;

    if (!retail::sub_77BF80() || !retail::sub_90F610((i32)region_pack_manager::inst())) // the terrain and the region packs are idle
        return;

    hold_blackscreen = false;

    if (missions->is_mission_ready() ||
        missions->is_mission_running() ||
        missions->has_auto_launch_instances())

        return;

    game_data* data = references::game.read()->data;

    if (!data || !retail::sub_77C9F0((u32*)data))
        references::frontend.get().igo->loading_screen->blackscreen_off(0.25f, false);
}

// sub_801B00
void quest_manager::giver_register_entity(actor* giver, bool unk) {
    if (!giver)
        return;

    void*         allocation = mm::allocate(sizeof(giver_entity));
    giver_entity* record     = allocation ? new (allocation) giver_entity : nullptr;

    record->entity                = giver;
    record->unk_18                = unk;
    record->attached_conversation = nullptr;

    for (i32 index = 0; index < giver_count; ++index) {
        string_hash name;
        name.initialize(mash::ALLOCATED, givers[index]->name.c_str());

        if (giver->id == name) {
            record->name = givers[index]->name;
            break;
        }
    }

    if (giver->get_ai_core()) {
        auto* aim_util = (aim_util_inode*)giver->get_ai_core()->get_info_node(info_node_type_aim_util);

        if (aim_util)
            aim_util->flags |= aim_util_flag_aim_head;
    }

    if (giver->has_ifc(entity_ifc_physical))
        giver->physical_ifc()->flags |= physical_interface_flag_immobile;

    string_hash trigger_name;
    void*       trigger_allocation = mm::allocate(0x58);

    record->sphere_trigger = trigger_allocation ?
        (trigger*)retail::sub_75D900((i32)trigger_allocation,
                                     retail::sub_64CD40(&trigger_name.source_hash_code, (void*)"sphere_trigger"), // the next free instance name
                                     (i32)giver->my_handle.value,
                                     2.0f) :
        nullptr;

    record->sphere_trigger->add_callback(references::enter.read(), giver_trigger_entered, record, false);
    record->sphere_trigger->add_callback(references::leave.read(), (code_event_callback_function)retail::sub_7EAAE0, record, false); // takes the square button down

    record->square_pressed_callback_id = 0;
    record->face_button                = 0;

    giver_entities->push_back(record);
}

// sub_800760
void quest_manager::giver_trigger_entered(event*, arch_base_vhandle, void* parameters) {
    entity* hero = references::g_world_ptr.read()->get_hero_ptr();

    string_hash command;
    command.initialize(mash::ALLOCATED, "ENABLE_PLR_CMD_COMBAT_LIGHT_MELEE");

    hero->set_ifc_num(command, 0.0f, false);

    auto* record = (giver_entity*)parameters;

    if (record->entity->is_an_actor() && record->entity->get_ai_core()) {
        string_hash flag;
        flag.initialize(mash::ALLOCATED, nullptr, 0x1215CFD7); // an ai flag whose name is not in the binary

        if (rvb_get_flag(record->entity, &flag))
            return;
    }

    record->face_button                = retail::sub_70C7E0((u32*)references::frontend.get().igo->face_button_system, 2, -1.0f, 0.0f); // the square button
    record->square_pressed_callback_id = references::script_controller.read()->add_callback(references::square_pressed.read(), giver_square_pressed, record, false);
}

// sub_800660
void quest_manager::giver_square_pressed(event*, arch_base_vhandle, void* parameters) {
    entity* hero = references::g_world_ptr.read()->get_hero_ptr();

    bool standing = hero->has_ifc(entity_ifc_standing) &&
                    hero->standing_ifc()->is_enabled() &&
                    hero->standing_ifc()->is_effectively_standing() &&
                    (hero->standing_ifc()->flags & standing_interface_flag_was_effectively_standing);

    auto*            record   = (giver_entity*)parameters;
    mission_manager* missions = mission_manager::inst();

    if (record->attached_conversation &&
        standing &&
        (missions->is_idle() || missions->is_mission_running())) {

        bool met_before = retail::sub_7F0FD0((i32*)quest_manager::inst(), (i32)&record->name); // and marks the giver met

        references::frontend.get().igo->conversation_menu_system->begin_conversation(record->attached_conversation, met_before, true);
        references::frontend.get().igo->face_button_system->remove_all_buttons();

        retail::sub_7EAA70((u32*)quest_manager::inst(), (i32)record->entity); // suspends the giver
    } else if (record->unk_18)
        retail::sub_7FF3D0((i32*)quest_manager::inst(), (i32)&record->name); // offers the giver's available quests
}
