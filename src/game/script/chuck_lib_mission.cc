#include <cstdio>

#include "retail.hh"
#include "treyarch/chuck/vm/script_executable.hh"
#include "treyarch/chuck/vm/script_object.hh"
#include "treyarch/chuck/vm/vm_thread.hh"
#include "treyarch/mm/mm.hh"
#include "game/amalga/resource_manager.hh"
#include "game/amalga/resource_types.hh"
#include "game/app.hh"
#include "game/color.hh"
#include "game/environment_progression.hh"
#include "game/frontend/dialog_box.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_message_box_widget.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/game.hh"
#include "game/localized_string_table.hh"
#include "game/mission/mission_manager.hh"
#include "game/mission/mission_progression_data.hh"
#include "game/quest_manager.hh"
#include "game/region_pack_manager.hh"
#include "game/region_spawn_manager.hh"
#include "game/resource_key.hh"
#include "game/script/chuck_lib_mission.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"
#include "banana/util/memory_reference.hh"

namespace treyarch { namespace references {
    // written here only, nothing reads it
    util::memory_reference<i32> loading_hint_stage { 0x010F7070 };

    util::memory_reference<color32> city_life_message_color { 0x00F22158 };
}} // treyarch::references

using namespace treyarch;

// sub_8570B0
bool slf__terminate_script__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    mission_manager::inst()->terminate_script(stack.local_num(0x00) != 0.0f, false);

    return false;
}

// sub_857100
bool slf__set_loading_hint_stage__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    references::loading_hint_stage.write((i32)stack.local_num(0x00));

    return true;
}

// sub_83E140
bool slf__get_running_script_index__t::operator()(chuck::vm::vm_stack &stack) {
    f32 result = (f32)mission_manager::inst()->get_running_script_index();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_857120
bool slf__get_mission_index__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    f32 result = (f32)mission_manager::inst()->get_mission_index(arch_base_vhandle(stack.local_uint(0x00)));

    stack.push(&result, sizeof(result));

    return true;
}

// sub_83E170
bool slf__get_total_times_script_has_been_successfully_completed__t::operator()(chuck::vm::vm_stack &stack) {
    f32 result = (f32)mission_manager::inst()->get_total_times_script_has_been_successfully_completed();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_857160
bool slf__mission_icon_punched__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    f32 result = mission_manager::inst()->mission_icon_punched(arch_base_vhandle(stack.local_uint(0x00)), -1.0f) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8571C0
bool slf__is_mission_eligible__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    f32 result = mission_manager::inst()->is_mission_eligible(arch_base_vhandle(stack.local_uint(0x00))) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_857210
bool slf__get_mission_caption_index__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    i32 index;
    f32 result = -1.0f;

    if (mission_manager::inst()->get_mission_caption_index(arch_base_vhandle(stack.local_uint(0x00)), &index))
        result = (f32)index;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_857280
bool slf__setup_mission_instance_parameters__mission_district_instance_base__t::operator()(chuck::vm::vm_stack &stack) {
    auto* parameters = (chuck::vm::script_instance*)stack.pop_uint();

    if (mission_manager::inst()->setup_mission_instance_parameters(parameters))
        return true;

    // suspends the calling executable
    ++stack.thread->inst->parent->parent->suspend_count;

    return false;
}

// sub_83E1A0
bool slf__is_mission_running__t::operator()(chuck::vm::vm_stack &stack) {
    f32 result = mission_manager::inst()->is_mission_running() || mission_manager::inst()->is_mission_ready() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8572C0
bool slf__get_registry_value__str__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    string_hash key;
    key.initialize(mash::ALLOCATED, (const char*)stack.local_uint(0x04));

    string_hash entity;
    entity.initialize(mash::ALLOCATED, (const char*)stack.local_uint(0x00));

    f32 value;
    mission_manager::inst()->script_registry_insert_rational_key(entity, key, stack.local_num(0x08), &value, false);

    stack.push(&value, sizeof(value));

    return true;
}

// sub_857370
bool slf__get_registry_value__uint__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    string_hash key;
    key.initialize(mash::ALLOCATED, (const char*)stack.local_uint(0x04));

    string_hash entity;
    entity.initialize(mash::ALLOCATED, nullptr, stack.local_uint(0x00));

    f32 value;
    mission_manager::inst()->script_registry_insert_rational_key(entity, key, stack.local_num(0x08), &value, false);

    stack.push(&value, sizeof(value));

    return true;
}

// sub_857420
bool slf__set_registry_value__uint__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    string_hash key;
    key.initialize(mash::ALLOCATED, (const char*)stack.local_uint(0x04));

    string_hash entity;
    entity.initialize(mash::ALLOCATED, nullptr, stack.local_uint(0x00));

    f32 value;
    mission_manager::inst()->script_registry_insert_rational_key(entity, key, stack.local_num(0x08), &value, true);

    return true;
}

// sub_8574B0
bool slf__get_registry_value__uint__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    string_hash key;
    key.initialize(mash::ALLOCATED, (const char*)stack.local_uint(0x04));

    string_hash entity;
    entity.initialize(mash::ALLOCATED, nullptr, stack.local_uint(0x00));

    vector3d value;
    mission_manager::inst()->script_registry_insert_vector3d_key(entity, key, *(vector3d*)&stack.local_uint(0x08), &value, false);

    stack.push(&value, sizeof(value));

    return true;
}

// sub_857570
bool slf__save_checkpoint__vector3d__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x1C;

    mission_manager::inst()->save_checkpoint(*(vector3d*)&stack.local_uint(0x00),
                                             *(vector3d*)&stack.local_uint(0x0C),
                                             stack.local_num(0x18));

    return true;
}

// sub_83E1F0
bool slf__get_checkpoint__t::operator()(chuck::vm::vm_stack &stack) {
    f32 result = (f32)mission_manager::inst()->checkpoint;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_83E230
bool slf__get_overrride_checkpoint__t::operator()(chuck::vm::vm_stack &stack) {
    auto* registry = (mission_progression_registry*)retail::sub_7685B0(); // mission_progression_registry::inst

    auto stage_count = (i32)registry->stages.size();
    f32  result      = stage_count > 0 ? 0.0f : (f32)(stage_count - 1);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_841EB0
bool slf__is_debug_force_script_running__t::operator()(chuck::vm::vm_stack &stack) {
    f32 result = 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_841EB0
bool slf__is_debug_force_script_retried__t::operator()(chuck::vm::vm_stack &stack) {
    f32 result = 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8575B0
bool slf__game_time_advance__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    mission_manager::inst()->game_time_advance((i32)stack.local_num(0x00), (i32)stack.local_num(0x04));

    return true;
}

// sub_8575E0
bool slf__set_time_of_day_rate__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    mission_manager::inst()->set_time_of_day_rate((i32)stack.local_num(0x00));

    return true;
}

// sub_83E280
bool slf__game_time_get_seconds_since_midnight__t::operator()(chuck::vm::vm_stack &stack) {
    f32 result = (f32)(i32)mission_manager::inst()->game_time_get_seconds_since_midnight();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_857620
bool slf__internal_mission_manager_set_current_act__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    mission_manager::inst()->set_current_act((i32)stack.local_num(0x00), false);

    return true;
}

// sub_857640
bool slf__mission_manager_set_is_quitable__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    mission_manager::inst()->is_quitable = stack.local_num(0x00) != 0.0f;

    return true;
}

// sub_83E2B0
bool slf__mission_manager_is_act_transitioning__t::operator()(chuck::vm::vm_stack &stack) {
    f32 result = mission_manager::inst()->is_act_transitioning() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_83E310
bool slf__mission_manager_get_current_act__t::operator()(chuck::vm::vm_stack &stack) {
    f32 result = (f32)mission_manager::inst()->get_current_act();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_83E340
bool slf__mission_manager_get_current_act_internal__t::operator()(chuck::vm::vm_stack &stack) {
    f32 result = (f32)mission_manager::inst()->get_current_act_internal();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_857680
bool slf__set_mission_meta_text__widget_3d__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    mission_manager::inst()->set_mission_meta_text(stack.local_uint(0x00),
                                                   arch_base_vhandle(stack.local_uint(0x04)),
                                                   (i32)stack.local_num(0x08));

    return true;
}

// sub_8576B0
bool slf__set_mission_finished_screen_has_appeared__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    mission_manager::inst()->set_mission_finished_screen_has_appeared(stack.local_num(0x00) != 0.0f, stack.local_num(0x04) != 0.0f);

    return true;
}

// sub_823B80
bool slf__handle_finished_dialog_selection__t::operator()(chuck::vm::vm_stack &stack) {
    mission_manager::inst()->handle_finished_dialog_selection();

    return true;
}

// sub_857710
bool slf__hide_blackscreen_after_unload__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    mission_manager::inst()->hide_blackscreen_after_unload(stack.local_num(0x00) != 0.0f);

    return true;
}

// sub_83E370
bool slf__get_current_key_position__t::operator()(chuck::vm::vm_stack &stack) {
    vector3d result = mission_manager::inst()->get_mission_key_position();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_823B90
bool slf__test__t::operator()(chuck::vm::vm_stack &stack) {
    std::printf("test!");

    return true;
}

// sub_857840
bool slf__register_quest_giver__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    retail::sub_801B00((u32*)quest_manager::inst(), (i32)arch_base_vhandle(stack.local_uint(0x00)).resolve(), nullptr);

    return true;
}

// sub_857870
bool slf__unregister_quest_giver__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    retail::sub_7FC070((u32*)quest_manager::inst(), (i32*)arch_base_vhandle(stack.local_uint(0x00)).resolve());

    return true;
}

// sub_8578A0
bool slf__quest_event__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    mash::string name((const char*)stack.local_uint(0x00));

    retail::sub_800160((u32*)quest_manager::inst(), 5, (i32)&name, 0); // the quest event core, script category

    return true;
}

// sub_857910
bool slf__give_quest__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    mash::string name((const char*)stack.local_uint(0x00));

    quest_manager::inst()->give_quest(name);

    return true;
}

// sub_857980
bool slf__attach_conversation__entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    mash::string name((const char*)stack.local_uint(0x04));

    retail::sub_7F9010((u32*)quest_manager::inst(), (i32)arch_base_vhandle(stack.local_uint(0x00)).resolve(), (i32)&name, true);

    return true;
}

// sub_857A00
bool slf__attach_quest_conversation__entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    mash::string name((const char*)stack.local_uint(0x04));

    retail::sub_7FF430((u32*)quest_manager::inst(), (i32)arch_base_vhandle(stack.local_uint(0x00)).resolve(), (i32)&name, true);

    return true;
}

// sub_83E3A0
bool slf__completed_quest_count__t::operator()(chuck::vm::vm_stack &stack) {
    f32 result = (f32)quest_manager::inst()->completed_quest_count();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_857A80
bool slf__get_completed_quest__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    const char* result = stack.thread->create_string(quest_manager::inst()->get_completed_quest((i32)stack.local_num(0x00)).c_str());

    stack.push(&result, sizeof(result));

    return true;
}

// sub_83E3D0
bool slf__available_quest_count__t::operator()(chuck::vm::vm_stack &stack) {
    f32 result = (f32)quest_manager::inst()->available_quest_count();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_857B10
bool slf__get_available_quest__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    const char* result = stack.thread->create_string(quest_manager::inst()->get_available_quest((i32)stack.local_num(0x00)).c_str());

    stack.push(&result, sizeof(result));

    return true;
}

// sub_857BA0
bool slf__get_questgiver_name__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    const char* result;

    {
        mash::string name((const char*)stack.local_uint(0x00));

        result = stack.thread->create_string(quest_manager::inst()->get_questgiver_name(name).c_str());
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_857C50
bool slf__get_questgiver_loc__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    vector3d result;

    {
        mash::string name((const char*)stack.local_uint(0x00));

        result = quest_manager::inst()->get_questgiver_loc(name);
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_857F60
bool slf__city_life_failed_message__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    const char* text = references::game.read()->string_localizer->lookup_scripttext_string((i32)stack.local_num(0x00));

    igo_3d_message_box_widget* announcement = references::frontend.get().igo->unified_announcement;

    announcement->method_0dc()(announcement,
                               mash::string(text),
                               10.0f,
                               0,
                               0,
                               0.0f,
                               0.0f,
                               references::city_life_message_color.read());

    return true;
}

// sub_857CD0
bool slf__do_conversation_wait__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    // waits for the conversation to end
    if (stack.thread->entry)
        return !references::frontend.get().igo->conversation_menu_system->is_conversation_active();

    string_hash name;
    name.initialize(mash::ALLOCATED, (const char*)stack.local_uint(0x00));

    resource_key key;
    key.set(name, (u32)amalga::e_resource_type::JSON_CONVERSATION);

    if (u8* found = amalga::resource_manager::get_resource(key, nullptr, nullptr))
        retail::sub_717CE0((i32)references::frontend.get().igo->conversation_menu_system, (i32)found, 1, 0); // start the conversation

    return false;
}

// sub_83E400
bool slf__do_dialog_wait__t::operator()(chuck::vm::vm_stack &stack) {
    if (!stack.thread->entry) {
        void* allocation = mm::allocate(sizeof(dialog_box));
        auto* box        = allocation ? (dialog_box*)retail::sub_70BE00((u32*)allocation) : nullptr;

        references::frontend.get().igo->set_dialog_box(box, 1);

        return false;
    }

    // waits for a choice, then pushes it
    dialog_box* box = references::frontend.get().igo->dialog;

    if (!box->confirmed)
        return false;

    f32 result = box->choice ? 1.0f : 0.0f;

    box->hide();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_823BB0
bool slf__update_progression__t::operator()(chuck::vm::vm_stack &stack) {
    quest_manager::inst()->update_progression();

    return true;
}

// sub_83E4E0
bool slf__get_progression_level__t::operator()(chuck::vm::vm_stack &stack) {
    f32 result = references::environment_progression_state.read()->progression_level;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_857D70
bool slf__city_life_complete_message__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    f32 xp = stack.local_num(0x08);

    {
        char bonus_line[256];

        if (xp > 0.0f) {
            bonus_line[0] = ' ';
            std::sprintf(bonus_line + 1, references::game.read()->string_localizer->lookup_scripttext_string((i32)stack.local_num(0x04)), (i32)xp);
        } else
            bonus_line[0] = '\0';

        mash::string bonus(bonus_line);

        const char* text = references::game.read()->string_localizer->lookup_scripttext_string((i32)stack.local_num(0x00));

        igo_3d_message_box_widget* announcement = references::frontend.get().igo->unified_announcement;

        announcement->method_0dc()(announcement,
                                   text + bonus,
                                   10.0f,
                                   0,
                                   0,
                                   0.0f,
                                   0.0f,
                                   references::city_life_message_color.read());
    }

    entity* hero = references::g_world_ptr.read()->hero_ptr;

    retail::sub_636E00(hero->my_ifc_storage->get_ifc(entity_ifc_rpg), xp, 0); // award the xp
    quest_manager::inst()->set_completed_quest_timers();

    return true;
}

// sub_857EC0
bool slf__city_life_xp_message__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    char text[260];
    std::sprintf(text, references::game.read()->string_localizer->lookup_scripttext_string((i32)stack.local_num(0x00)), (i32)stack.local_num(0x04));

    igo_3d_message_box_widget* announcement = references::frontend.get().igo->unified_announcement;

    announcement->method_0dc()(announcement,
                               mash::string(text),
                               10.0f,
                               0,
                               0,
                               0.0f,
                               0.0f,
                               references::city_life_message_color.read());

    return true;
}

// sub_857F60
bool slf__city_life_in_progress_message__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    const char* text = references::game.read()->string_localizer->lookup_scripttext_string((i32)stack.local_num(0x00));

    igo_3d_message_box_widget* announcement = references::frontend.get().igo->unified_announcement;

    announcement->method_0dc()(announcement,
                               mash::string(text),
                               10.0f,
                               0,
                               0,
                               0.0f,
                               0.0f,
                               references::city_life_message_color.read());

    return true;
}

bool slf__has_succeeded_city_life__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E590)(this, stack);
}

bool slf__set_max_active_spawn_items__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857FD0)(this, stack);
}

bool slf__update_point_of_interest__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858030)(this, stack);
}

bool slf__set_spawn_placement_figures__num__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858090)(this, stack);
}

bool slf__push_spawn_item_during_mission__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8580E0)(this, stack);
}

bool slf__pop_spawn_item_during_mission__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858130)(this, stack);
}

bool slf__register_entity_with_spawn_manager__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858180)(this, stack);
}

bool slf__unregister_entity_with_spawn_manager__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858280)(this, stack);
}

bool slf__begin_populate_mission_with_entities__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E5C0)(this, stack);
}

bool slf__end_populate_mission_with_entities__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E5E0)(this, stack);
}

bool slf__force_populate_mission_cleanup__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E600)(this, stack);
}

bool slf__set_street_percent_override__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8582C0)(this, stack);
}

bool slf__get_entities__dynamic_array__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_880D60)(this, stack);
}

bool slf__get_spawn_position__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E630)(this, stack);
}

bool slf__has_optional_data__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858320)(this, stack);
}

bool slf__get_vector_data__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858390)(this, stack);
}

bool slf__get_str_data__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858440)(this, stack);
}

bool slf__get_num_data__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8584D0)(this, stack);
}

bool slf__get_vector_data_array__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_880EA0)(this, stack);
}

bool slf__get_str_data_array__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_880FB0)(this, stack);
}

bool slf__get_num_data_array__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8810A0)(this, stack);
}

bool slf__has_optional_data__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E6C0)(this, stack);
}

bool slf__add_no_spawn_zone__str__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858570)(this, stack);
}

bool slf__add_no_spawn_zone__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8585D0)(this, stack);
}

bool slf__remove_no_spawn_zone__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858670)(this, stack);
}

bool slf__is_point_within_no_spawn_zone__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8586B0)(this, stack);
}

bool slf__add_debris_no_spawn_zone__str__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858710)(this, stack);
}

bool slf__add_debris_no_spawn_zone__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858760)(this, stack);
}

bool slf__add_arbitrary_region__str__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8587F0)(this, stack);
}

bool slf__add_arbitrary_region__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858850)(this, stack);
}

bool slf__remove_arbitrary_region__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8588F0)(this, stack);
}

bool slf__is_point_within_arbitrary_region__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858930)(this, stack);
}

bool slf__has_persistence_data__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E730)(this, stack);
}

bool slf__start_persistence_data_save__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858990)(this, stack);
}

bool slf__save_persistence_vector3d__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8589D0)(this, stack);
}

bool slf__save_persistence_vector3d_array__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858A30)(this, stack);
}

bool slf__get_persistence_vector3d__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858AE0)(this, stack);
}

bool slf__get_persistence_vector3d_array__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_881190)(this, stack);
}

bool slf__save_persistence_str__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858B70)(this, stack);
}

bool slf__save_persistence_str_array__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858C00)(this, stack);
}

bool slf__get_persistence_str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858CB0)(this, stack);
}

bool slf__get_persistence_str_array__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_881250)(this, stack);
}

bool slf__save_persistence_num__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858D30)(this, stack);
}

bool slf__save_persistence_num_array__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858D80)(this, stack);
}

bool slf__get_persistence_num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858E30)(this, stack);
}

bool slf__get_persistence_num_array__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_881310)(this, stack);
}

bool slf__end_persistance_data_save__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E780)(this, stack);
}

bool slf__force_despawn_city_life__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E7B0)(this, stack);
}

bool slf__toggle_override_spawn_distance_in_mission__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__toggle_override_spawn_pacing_in_mission__num__num__num__num__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858EB0)(this, stack);
}

bool slf__get_all_city_life_of_category__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858F90)(this, stack);
}

bool slf__get_all_categories_on_city_life__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859030)(this, stack);
}

bool slf__create_category_for_mission__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8590E0)(this, stack);
}

bool slf__register_important_city_life_entity__str__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859130)(this, stack);
}

bool slf__get_important_city_life_entity__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859180)(this, stack);
}

bool slf__is_valid_spawn__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859210)(this, stack);
}

bool slf__save_safe_zone_wall_percentages__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859280)(this, stack);
}

bool slf__get_safe_zone_wall_percentages__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859310)(this, stack);
}

bool slf__set_safe_zone_wall_index__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8593A0)(this, stack);
}

bool slf__get_safe_zone_wall_index__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E7E0)(this, stack);
}

bool slf__end_summon__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E830)(this, stack);
}

bool slf__summon_deduct_meter_for_special_attack__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E850)(this, stack);
}

bool slf__mission_force_summon_availability__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859400)(this, stack);
}

bool slf__can_summon__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E870)(this, stack);
}

bool slf__call_in_summon__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859460)(this, stack);
}

bool slf__toggle_summon_ability_in_script__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8594A0)(this, stack);
}

bool slf__get_is_summon_ability_script_toggled__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E8C0)(this, stack);
}

bool slf__get_spawn_point_point_data__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8594E0)(this, stack);
}

bool slf__is_in_stars_mode__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E900)(this, stack);
}

bool slf__get_neighborhood_center__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E950)(this, stack);
}

bool slf__get_meta_neighborhood_center__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E9D0)(this, stack);
}

bool slf__get_current_flagsets__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859570)(this, stack);
}

bool slf__flag_always_on_for_despawn__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83EA50)(this, stack);
}

bool slf__get_spawn_position__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859600)(this, stack);
}

bool slf__set_current_stored_summon_actor__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8596B0)(this, stack);
}

// sub_83EA80
bool slf__despawn_and_lock_quest_giver_in_mission__t::operator()(chuck::vm::vm_stack &stack) {
    if (references::region_spawns_enabled.read() && region_pack_manager::inst())
        region_pack_manager::inst()->despawn_and_lock_quest_giver_in_mission(true);

    return true;
}

bool slf__set_collectibles_disabled__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8596E0)(this, stack);
}

bool slf__is_summon_summoned__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859720)(this, stack);
}

bool slf__is_cross_neighborhood_spawn_active__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83EAA0)(this, stack);
}

bool slf__is_spidey_near_quest_giver__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83EB10)(this, stack);
}
