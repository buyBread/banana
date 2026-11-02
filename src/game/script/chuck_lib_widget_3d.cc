#include <new>

#include "retail.hh"
#include "game/arch_base_vhandle.hh"
#include "game/frontend/finger_of_god.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_camera_widget.hh"
#include "game/frontend/igo/igo_3d_face_button_system.hh"
#include "game/frontend/igo/igo_3d_pauseless_dialog_widget.hh"
#include "game/app.hh"
#include "game/frontend/igo/igo_3d_script_widget.hh"
#include "game/frontend/igo/igo_3d_text.hh"
#include "game/frontend/message_text.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/game.hh"
#include "game/game_data.hh"
#include "game/geometry/geometry_manager.hh"
#include "game/input/input_mgr.hh"
#include "game/localized_string_table.hh"
#include "game/math/references.hh"
#include "game/mission/mission_manager.hh"
#include "game/polytube.hh"
#include "game/rpg_upgrades.hh"
#include "game/script/chuck_lib_widget.hh"
#include "game/script/chuck_lib_widget_3d.hh"
#include "game/script/script_access.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"
#include "treyarch/base/tl_fixed_string.hh"
#include "treyarch/chuck/vm/vm_dynamic_array_manager.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/vector.hh"

using namespace treyarch;

// sub_8604B0
bool slf__create_3d_widget__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto name = (const char*)stack.local_uint(0x00);

    igo_3d_script_widget* result;

    {
        native_resource_context context(stack);

        string_hash key;

        key.initialize(mash::ALLOCATED, name);

        result = references::frontend.get().igo->create_script_widget(key);

        stack.thread->get_instance(-1)->add_allocated_stuff(chuck::vm::script_garbage_collection_widget_3d, (u32)result);
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_860580
bool slf__create_3d_widget_from_params__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto name = (const char*)stack.local_uint(0x00);

    igo_3d_script_widget* result;

    {
        native_resource_context context(stack);

        string_hash key;

        key.initialize(mash::ALLOCATED, name);

        result = references::frontend.get().igo->create_script_widget_from_parameters(key);

        stack.thread->get_instance(-1)->add_allocated_stuff(chuck::vm::script_garbage_collection_widget_3d, (u32)result);
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_860650
bool slf__create_3d_mission_text_widget__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto name = (const char*)stack.local_uint(0x00);

    alignas(mash::string) u8 widget_name_storage[sizeof(mash::string)];
    alignas(mash::string) u8 text_name_storage  [sizeof(mash::string)];

    mash::string*         widget_name;
    mash::string*         text_name;
    igo_3d_script_widget* result;

    {
        native_resource_context context(stack);

        widget_name = new (widget_name_storage) mash::string("ui_HUD_mission_text");
        text_name   = new (text_name_storage)   mash::string("ui_HUD_mission_text");

        string_hash text_key;
        string_hash widget_key;
        string_hash record_key;

        text_key  .initialize(mash::ALLOCATED, text_name->c_str());
        widget_key.initialize(mash::ALLOCATED, widget_name->c_str());
        record_key.initialize(mash::ALLOCATED, name);

        result = references::frontend.get().igo->create_mission_text_widget(record_key, widget_key, text_key);

        stack.thread->get_instance(-1)->add_allocated_stuff(chuck::vm::script_garbage_collection_widget_3d, (u32)result);
    }

    stack.push(&result, sizeof(result));

    text_name  ->finalize(mash::ALLOCATED);
    widget_name->finalize(mash::ALLOCATED);

    return true;
}

// sub_8607B0
bool slf__create_3d_hint_text_widget__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto name = (const char*)stack.local_uint(0x00);

    alignas(mash::string) u8 widget_name_storage[sizeof(mash::string)];
    alignas(mash::string) u8 text_name_storage  [sizeof(mash::string)];

    mash::string*         widget_name;
    mash::string*         text_name;
    igo_3d_script_widget* result;

    {
        native_resource_context context(stack);

        widget_name = new (widget_name_storage) mash::string("ui_HUD_hint_text");
        text_name   = new (text_name_storage)   mash::string("ui_HUD_hint_text");

        string_hash text_key;
        string_hash widget_key;
        string_hash record_key;

        text_key  .initialize(mash::ALLOCATED, text_name->c_str());
        widget_key.initialize(mash::ALLOCATED, widget_name->c_str());
        record_key.initialize(mash::ALLOCATED, name);

        result = references::frontend.get().igo->create_hint_text_widget(record_key, widget_key, text_key);

        stack.thread->get_instance(-1)->add_allocated_stuff(chuck::vm::script_garbage_collection_widget_3d, (u32)result);
    }

    stack.push(&result, sizeof(result));

    text_name  ->finalize(mash::ALLOCATED);
    widget_name->finalize(mash::ALLOCATED);

    return true;
}

// sub_860910
bool slf__create_3d_text_widget_from_params__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto widget_name = (const char*)stack.local_uint(0x00);
    auto text_name   = (const char*)stack.local_uint(0x04);

    igo_3d_script_widget* result;

    {
        native_resource_context context(stack);

        string_hash text_key;
        string_hash widget_key;

        text_key  .initialize(mash::ALLOCATED, text_name);
        widget_key.initialize(mash::ALLOCATED, widget_name);

        result = references::frontend.get().igo->create_text_widget_from_parameters(widget_key, text_key);

        stack.thread->get_instance(-1)->add_allocated_stuff(chuck::vm::script_garbage_collection_widget_3d, (u32)result);
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8241E0
bool slf__destroy_all_3d_mission_text_widgets__t::operator()(chuck::vm::vm_stack &stack) {
    stack.thread->get_instance(-1)->clear_allocated_stuff(chuck::vm::script_garbage_collection_widget_3d);
    references::frontend.get().igo->destroy_all_mission_text_widgets();

    return true;
}

// sub_824210
bool slf__hide_all_3d_mission_text_widgets__t::operator()(chuck::vm::vm_stack &stack) {
    references::frontend.get().igo->hide_all_mission_text_widgets();

    return true;
}

// sub_824220
bool slf__destroy_all_3d_hint_text_widgets__t::operator()(chuck::vm::vm_stack &stack) {
    stack.thread->get_instance(-1)->clear_allocated_stuff(chuck::vm::script_garbage_collection_widget_3d);
    references::frontend.get().igo->destroy_all_hint_text_widgets();

    return true;
}

// sub_824250
bool slf__hide_all_3d_hint_text_widgets__t::operator()(chuck::vm::vm_stack &stack) {
    references::frontend.get().igo->hide_all_hint_text_widgets();

    return true;
}

// sub_860A20
bool slf__create_3d_button_widget__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto kind = stack.local_num(0x00);

    string_hash key;

    key.initialize(mash::ALLOCATED, "");

    if (igo_3d_face_button_system* buttons = references::frontend.get().igo->face_button_system)
        key = buttons->get_button_widget_key((i32)kind);

    auto result = references::frontend.get().igo->create_script_widget(key);

    stack.thread->get_instance(-1)->add_allocated_stuff(chuck::vm::script_garbage_collection_widget_3d, (u32)result);
    stack.push(&result, sizeof(result));

    return true;
}

// sub_860AB0
bool slf__destroy_3d_widget__widget_3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto widget = (igo_3d_script_widget*)stack.local_uint(0x00);

    stack.thread->get_instance(-1)->remove_allocated_stuff(chuck::vm::script_garbage_collection_widget_3d, (u32)widget);
    references::frontend.get().igo->destroy_script_widget(widget);

    return true;
}

// sub_824260
bool slf__destroy_all_3d_widgets__t::operator()(chuck::vm::vm_stack &stack) {
    stack.thread->get_instance(-1)->clear_allocated_stuff(chuck::vm::script_garbage_collection_widget_3d);
    references::frontend.get().igo->destroy_all_script_widgets();

    return true;
}

// sub_860AF0
bool slf__move_ui_camera_to_marker__widget_3d__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto widget      = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto marker_name = (const char*)stack.local_uint(0x04);
    auto duration    = stack.local_num(0x08);

    if (references::frontend.get().igo->is_script_widget_valid(widget) &&
        references::frontend.get().igo->camera_widget) {

        string_hash marker;

        marker.initialize(mash::ALLOCATED, marker_name);

        references::frontend.get().igo->camera_widget->move_to_marker(widget, marker, duration, &references::unk_010f7780.get());
    }

    return true;
}

// sub_860B70
bool slf__move_ui_camera_to_home__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto duration = stack.local_num(0x00);

    if (references::frontend.get().igo->camera_widget) {
        references::frontend.get().igo->camera_widget->kill_layer_0_anims();
        references::frontend.get().igo->camera_widget->reset_entity_po();
        references::frontend.get().igo->camera_widget->reset_position();

        retail::sub_69A910(references::frontend.get().igo->camera_widget, duration); // igo_3d_camera_widget move home
    }

    return true;
}

// sub_860BD0
bool slf__enable_health_meter__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto enabled = stack.local_num(0x00);

    ui_frontend* igo = references::frontend.get().igo;

    igo->vtable->set_meters_shown(igo, enabled != 0.0f);

    return true;
}

// sub_860C20
bool slf__enable_radar__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto enabled = stack.local_num(0x00);

    ui_frontend* igo = references::frontend.get().igo;

    igo->vtable->set_radar_shown(igo, enabled != 0.0f);

    return true;
}

// sub_860C70
bool slf__enable_boss_meter__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto boss = stack.local_uint(0x00);
    auto type = stack.local_num(0x04);

    references::frontend.get().igo->boss_meter_system->remove_all_meters();

    retail::sub_70CEC0(references::frontend.get().igo->boss_meter_system, boss, (i32)type); // igo_3d_boss_meter_system add

    return true;
}

// sub_824290
bool slf__disable_boss_meter__t::operator()(chuck::vm::vm_stack &stack) {
    references::frontend.get().igo->boss_meter_system->remove_all_meters();

    return true;
}

// sub_860CB0
bool slf__add_boss_meter__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto boss = stack.local_uint(0x00);
    auto type = stack.local_num(0x04);

    auto result = (f32)retail::sub_70CEC0(references::frontend.get().igo->boss_meter_system, boss, (i32)type); // igo_3d_boss_meter_system add

    stack.push(&result, sizeof(result));

    return true;
}

// sub_860D00
bool slf__add_boss_meter__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto type = stack.local_num(0x00);

    auto result = (f32)retail::sub_70D060(references::frontend.get().igo->boss_meter_system, (i32)type); // igo_3d_boss_meter_system add

    stack.push(&result, sizeof(result));

    return true;
}

// sub_860D40
bool slf__remove_boss_meter__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto id = stack.local_num(0x00);

    references::frontend.get().igo->boss_meter_system->remove_meter((i32)id);

    return true;
}

// sub_824290
bool slf__remove_all_boss_meters__t::operator()(chuck::vm::vm_stack &stack) {
    references::frontend.get().igo->boss_meter_system->remove_all_meters();

    return true;
}

// sub_860D70
bool slf__get_boss_meter_health_value__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto id = stack.local_num(0x00);

    f32 result = references::frontend.get().igo->boss_meter_system->get_health((i32)id);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_860DB0
bool slf__set_boss_meter_health_value__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto id     = stack.local_num(0x00);
    auto health = stack.local_num(0x04);

    references::frontend.get().igo->boss_meter_system->set_health((i32)id, health);

    return true;
}

// sub_8242B0
bool slf__clear_mission_objectives__t::operator()(chuck::vm::vm_stack &stack) {
    alignas(mash::string) u8 storage[sizeof(mash::string)];

    const char* text = references::game.read()->string_localizer->lookup_localized_string(340);

    auto* title = new (storage) mash::string(text);

    references::frontend.get().igo->pauseless_dialog->clear();
    references::frontend.get().igo->pauseless_dialog->set_title(*title);

    title->finalize(mash::ALLOCATED);

    return true;
}

// sub_860DE0
bool slf__add_mission_objective__num__t::operator()(chuck::vm::vm_stack &stack) {
    // the count word covers the text id and the format's nums
    i32 count = (i32)stack.pop_num();

    stack.move_sp(-4 * count);

    auto text_id = stack.local_num(0x00);

    alignas(mash::string) u8 format_storage[sizeof(mash::string)];
    alignas(mash::string) u8 result_storage[sizeof(mash::string)];
    alignas(mash::string) u8 storage[sizeof(mash::string)];

    const char* text = references::game.read()->string_localizer->lookup_scripttext_string((i32)text_id);

    auto* format = new (format_storage) mash::string(text);

    mash::string* result = format_script_text((mash::string*)result_storage,
                                              new (storage) mash::string(*format),
                                              0,
                                              &stack.local_num(0x04));

    references::frontend.get().igo->pauseless_dialog->add_text_line(result->c_str());

    result->finalize(mash::ALLOCATED);
    format->finalize(mash::ALLOCATED);

    return true;
}

// sub_860EF0
bool slf__add_mission_objective__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    chuck::vm::script_dynamic_array<f32> nums((chuck::vm::chuck_dynamic_array_t*)stack.local_uint(0x04));

    auto text_id = stack.local_num(0x00);

    alignas(mash::string) u8 format_storage[sizeof(mash::string)];
    alignas(mash::string) u8 result_storage[sizeof(mash::string)];
    alignas(mash::string) u8 storage[sizeof(mash::string)];

    const char* text = references::game.read()->string_localizer->lookup_scripttext_string((i32)text_id);

    auto* format = new (format_storage) mash::string(text);

    mash::string* result = format_script_text((mash::string*)result_storage,
                                              new (storage) mash::string(*format),
                                              &nums);

    references::frontend.get().igo->pauseless_dialog->add_text_line(result->c_str());

    result->finalize(mash::ALLOCATED);
    format->finalize(mash::ALLOCATED);

    return true;
}

// sub_860FF0
bool slf__add_mission_objective_debug__str__t::operator()(chuck::vm::vm_stack &stack) {
    i32 count = (i32)stack.pop_num();

    stack.move_sp(-4 * count);

    return true;
}

// sub_824360
bool slf__clear_3d_dialog_text__t::operator()(chuck::vm::vm_stack &stack) {
    references::frontend.get().igo->pauseless_dialog->clear();
    references::frontend.get().igo->pauseless_dialog->set_title("");

    return true;
}

// sub_861030
bool slf__add_3d_dialog_text__num__t::operator()(chuck::vm::vm_stack &stack) {
    // the count word covers the text id and the format's nums
    i32 count = (i32)stack.pop_num();

    stack.move_sp(-4 * count);

    auto text_id = stack.local_num(0x00);

    alignas(mash::string) u8 format_storage[sizeof(mash::string)];
    alignas(mash::string) u8 result_storage[sizeof(mash::string)];
    alignas(mash::string) u8 storage[sizeof(mash::string)];

    const char* text = references::game.read()->string_localizer->lookup_scripttext_string((i32)text_id);

    auto* format = new (format_storage) mash::string(text);

    mash::string* result = format_script_text((mash::string*)result_storage,
                                              new (storage) mash::string(*format),
                                              0,
                                              &stack.local_num(0x04));

    references::frontend.get().igo->pauseless_dialog->set_text(result->c_str());

    result->finalize(mash::ALLOCATED);
    format->finalize(mash::ALLOCATED);

    return true;
}

// sub_861140
bool slf__add_3d_dialog_text__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    chuck::vm::script_dynamic_array<f32> nums((chuck::vm::chuck_dynamic_array_t*)stack.local_uint(0x04));

    auto text_id = stack.local_num(0x00);

    alignas(mash::string) u8 format_storage[sizeof(mash::string)];
    alignas(mash::string) u8 result_storage[sizeof(mash::string)];
    alignas(mash::string) u8 storage[sizeof(mash::string)];

    const char* text = references::game.read()->string_localizer->lookup_scripttext_string((i32)text_id);

    auto* format = new (format_storage) mash::string(text);

    mash::string* result = format_script_text((mash::string*)result_storage,
                                              new (storage) mash::string(*format),
                                              &nums);

    references::frontend.get().igo->pauseless_dialog->set_text(result->c_str());

    result->finalize(mash::ALLOCATED);
    format->finalize(mash::ALLOCATED);

    return true;
}

// sub_860FF0
bool slf__add_3d_dialog_text_debug__str__t::operator()(chuck::vm::vm_stack &stack) {
    i32 count = (i32)stack.pop_num();

    stack.move_sp(-4 * count);

    return true;
}

// sub_861240
bool slf__add_3d_objective_text__num__t::operator()(chuck::vm::vm_stack &stack) {
    // the count word covers the text id and the format's nums
    i32 count = (i32)stack.pop_num();

    stack.move_sp(-4 * count);

    auto text_id = stack.local_num(0x00);

    alignas(mash::string) u8 format_storage[sizeof(mash::string)];
    alignas(mash::string) u8 result_storage[sizeof(mash::string)];
    alignas(mash::string) u8 storage[sizeof(mash::string)];

    const char* text = references::game.read()->string_localizer->lookup_scripttext_string((i32)text_id);

    auto* format = new (format_storage) mash::string(text);

    mash::string* result = format_script_text((mash::string*)result_storage,
                                              new (storage) mash::string(*format),
                                              0,
                                              &stack.local_num(0x04));

    references::frontend.get().igo->pauseless_dialog->add_text_line(result->c_str());

    result->finalize(mash::ALLOCATED);
    format->finalize(mash::ALLOCATED);

    return true;
}

// sub_861350
bool slf__add_3d_objective_text__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    chuck::vm::script_dynamic_array<f32> nums((chuck::vm::chuck_dynamic_array_t*)stack.local_uint(0x04));

    auto text_id = stack.local_num(0x00);

    alignas(mash::string) u8 format_storage[sizeof(mash::string)];
    alignas(mash::string) u8 result_storage[sizeof(mash::string)];
    alignas(mash::string) u8 storage[sizeof(mash::string)];

    const char* text = references::game.read()->string_localizer->lookup_scripttext_string((i32)text_id);

    auto* format = new (format_storage) mash::string(text);

    mash::string* result = format_script_text((mash::string*)result_storage,
                                              new (storage) mash::string(*format),
                                              &nums);

    references::frontend.get().igo->pauseless_dialog->add_text_line(result->c_str());

    result->finalize(mash::ALLOCATED);
    format->finalize(mash::ALLOCATED);

    return true;
}

// sub_860FF0
bool slf__add_3d_objective_text_debug__str__t::operator()(chuck::vm::vm_stack &stack) {
    i32 count = (i32)stack.pop_num();

    stack.move_sp(-4 * count);

    return true;
}

// sub_861450
bool slf__bring_up_3d_dialog__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    // the third argument is never read
    auto show_button_2 = stack.local_num(0x00);
    auto show_button_1 = stack.local_num(0x04);

    if (!stack.thread->entry) {
        references::frontend.get().igo->loading_screen->blackscreen_off(0.0f, false);

        igo_3d_pauseless_dialog_widget* dialog = references::frontend.get().igo->pauseless_dialog;

        dialog->set_button_2_text(410);
        dialog->set_button_1_text(344);
        dialog->show_button_2(show_button_2 != 0.0f);
        dialog->show_button_1(show_button_1 != 0.0f);
        dialog->igo_3d_dialog_widget::bring_up(2);

        return false;
    }

    if (references::frontend.get().igo->pauseless_dialog->is_dialog_shown())
        return false;

    auto result = (f32)references::frontend.get().igo->pauseless_dialog->get_selected_button();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_861570
bool slf__set_3d_dialog_title__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto text_id = stack.local_num(0x00);

    alignas(mash::string) u8 storage[sizeof(mash::string)];

    const char* text = references::game.read()->string_localizer->lookup_scripttext_string((i32)text_id);

    auto* title = new (storage) mash::string(text);

    references::frontend.get().igo->pauseless_dialog->set_title(title->c_str());

    title->finalize(mash::ALLOCATED);

    return true;
}

// sub_869EB0
bool slf__set_3d_dialog_title_debug__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    return true;
}

// sub_861610
bool slf__show_game_button__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto kind = stack.local_num(0x00);

    igo_3d_face_button_system* system = references::frontend.get().igo->face_button_system;

    f32 result = -1.0f;

    if (system)
        result = (f32)retail::sub_70C7E0((u32*)system, (i32)kind, -1.0f, 0.0f); // show_button

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8243B0
bool slf__remove_all_game_buttons__t::operator()(chuck::vm::vm_stack &stack) {
    igo_3d_face_button_system* system = references::frontend.get().igo->face_button_system;

    if (system)
        system->remove_all_buttons();

    return true;
}

// sub_861690
bool slf__remove_game_button__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto id = stack.local_num(0x00);

    igo_3d_face_button_system* system = references::frontend.get().igo->face_button_system;

    if (system)
        system->remove_button((i32)id);

    return true;
}

// sub_8616C0
bool slf__play_game_button_anim__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto id   = stack.local_num(0x00);
    auto anim = stack.local_num(0x04);

    igo_3d_face_button_system* system = references::frontend.get().igo->face_button_system;

    if (system)
        retail::sub_6D06E0((u32*)system, (i32)id, (i32)anim, 1, 0); // play the button anim

    return true;
}

// sub_861700
bool slf__game_button_anim_playing__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto id = stack.local_num(0x00);

    igo_3d_face_button_system* system = references::frontend.get().igo->face_button_system;

    f32 result = 0.0f;

    if (system)
        result = (f32)system->is_button_anim_playing((i32)id);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_861770
bool slf__set_game_button_pos__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto id = stack.local_num(0x00);
    auto x  = stack.local_num(0x04);
    auto y  = stack.local_num(0x08);

    igo_3d_face_button_system* system = references::frontend.get().igo->face_button_system;

    if (system)
        system->set_button_pos((i32)id, x, y);

    return true;
}

// sub_8617B0
bool slf__set_game_button_pos__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto id  = stack.local_num(0x00);
    auto pos = (const vector3d*)&stack.local_uint(0x04);

    igo_3d_face_button_system* system = references::frontend.get().igo->face_button_system;

    if (system)
        system->set_button_pos((i32)id, *pos);

    return true;
}

// sub_8617F0
bool slf__get_2d_ui_pos_from_world_pos__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    const vector3d  &world     = *(const vector3d*)&stack.local_uint(0x00);
    const matrix4x4 &to_view   = geometry_manager::references::world_to_view.get();
    const matrix4x4 &to_screen = geometry_manager::references::view_to_screen.get();

    auto view_x = (f32)((f64)world.x * to_view.x.x + (f64)world.y * to_view.y.x + (f64)world.z * to_view.z.x + (f64)to_view.w.x);
    auto view_y = (f32)((f64)world.x * to_view.x.y + (f64)world.y * to_view.y.y + (f64)world.z * to_view.z.y + (f64)to_view.w.y);
    auto view_z = (f32)((f64)world.x * to_view.x.z + (f64)world.y * to_view.y.z + (f64)world.z * to_view.z.z + (f64)to_view.w.z);

    auto screen_x = (f32)((f64)view_x * to_screen.x.x + (f64)view_y * to_screen.y.x + (f64)view_z * to_screen.z.x + (f64)to_screen.w.x);
    auto screen_y = (f32)((f64)view_x * to_screen.x.y + (f64)view_y * to_screen.y.y + (f64)view_z * to_screen.z.y + (f64)to_screen.w.y);
    auto screen_z = (f32)((f64)view_x * to_screen.x.z + (f64)view_y * to_screen.y.z + (f64)view_z * to_screen.z.z + (f64)to_screen.w.z);
    auto screen_w = (f32)((f64)view_x * to_screen.x.w + (f64)view_y * to_screen.y.w + (f64)view_z * to_screen.z.w + (f64)to_screen.w.w);

    f32 scale = 0.0f;

    if (screen_w != 0.0f)
        scale = (f32)(1.0 / (f64)screen_w);

    vector3d result((f32)((f64)screen_x * scale),
                    (f32)((f64)screen_y * scale),
                    (f32)((f64)screen_z * scale));

    stack.push(&result, sizeof(result));

    return true;
}

// sub_861B00
bool slf__set_game_button_scale__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto id    = stack.local_num(0x00);
    auto scale = stack.local_num(0x04);

    igo_3d_face_button_system* system = references::frontend.get().igo->face_button_system;

    if (system)
        system->set_button_scale((i32)id, scale);

    return true;
}

// sub_861B30
bool slf__set_game_button_alpha__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto id    = stack.local_num(0x00);
    auto alpha = stack.local_num(0x04);

    igo_3d_face_button_system* system = references::frontend.get().igo->face_button_system;

    if (system)
        system->set_button_alpha((i32)id, alpha);

    return true;
}

// sub_861B60
bool slf__show_X_over_button__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto id   = stack.local_num(0x00);
    auto show = stack.local_num(0x04);

    igo_3d_face_button_system* system = references::frontend.get().igo->face_button_system;

    if (system) {
        bool shown = show > 0.0f;

        system->show_x_over_button((i32)id, shown);
    }

    return true;
}

// sub_861BB0
bool slf__is_X_over_button_shown__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto id = stack.local_num(0x00);

    igo_3d_face_button_system* system = references::frontend.get().igo->face_button_system;

    f32 result = 0.0f;

    if (system)
        result = (f32)system->is_x_over_button_shown((i32)id);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_869EB0
bool slf__load_debug_3d_widget__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    return true;
}

// sub_8622F0
bool slf__set_debug_3d_widget_pos__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    return true;
}

// sub_841EB0
bool slf__get_debug_3d_widget_x__t::operator()(chuck::vm::vm_stack &stack) {
    f32 result = 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_841EB0
bool slf__get_debug_3d_widget_y__t::operator()(chuck::vm::vm_stack &stack) {
    f32 result = 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_869EB0
bool slf__set_debug_3d_widget_scale__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    return true;
}

// sub_841EB0
bool slf__get_debug_3d_widget_scale__t::operator()(chuck::vm::vm_stack &stack) {
    f32 result = 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_869EB0
bool slf__set_debug_3d_widget_tilt__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    return true;
}

// sub_841EB0
bool slf__get_debug_3d_widget_tilt__t::operator()(chuck::vm::vm_stack &stack) {
    f32 result = 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_869EB0
bool slf__set_debug_3d_widget_spin__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    return true;
}

// sub_841EB0
bool slf__get_debug_3d_widget_spin__t::operator()(chuck::vm::vm_stack &stack) {
    f32 result = 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_869EB0
bool slf__set_debug_3d_widget_rotation__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    return true;
}

// sub_841EB0
bool slf__get_debug_3d_widget_rotation__t::operator()(chuck::vm::vm_stack &stack) {
    f32 result = 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_83F550
bool slf__get_debug_3d_widget_entity__t::operator()(chuck::vm::vm_stack &stack) {
    u32 result = 0;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_869EB0
bool slf__debug_3d_widget_play_anim__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    return true;
}

// sub_869EB0
bool slf__debug_3d_widget_set_cowbell_variant__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    return true;
}

// sub_869EB0
bool slf__debug_3d_widget_add_ui_parameter_files_to_menu__debug_menu__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    return true;
}

// sub_83ED90
bool slf__get_ui_camera_entity__t::operator()(chuck::vm::vm_stack &stack) {
    igo_3d_camera_widget* camera = references::frontend.get().igo->camera_widget;

    arch_base_vhandle result;

    if (camera)
        result = camera->entity_vh;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_861C20
bool slf__ui_camera_play_anim__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto anim = (const char*)stack.local_uint(0x00);

    if (references::frontend.get().igo->camera_widget) {
        references::frontend.get().igo->camera_widget->kill_layer_0_anims();

        tl_fixed_string name(anim);

        references::frontend.get().igo->camera_widget->play_anim(name);
    }

    return true;
}

// sub_861C70
bool slf__ui_camera_move_to_marker__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto marker_name = (const char*)stack.local_uint(0x00);
    auto duration    = stack.local_num(0x04);

    if (references::frontend.get().igo->unknown_widget_188 &&
        references::frontend.get().igo->camera_widget) {

        string_hash marker;

        marker.initialize(mash::ALLOCATED, marker_name);

        ui_frontend* igo = references::frontend.get().igo;

        igo->camera_widget->move_to_marker(igo->unknown_widget_188, marker, duration, &references::unk_010f7780.get());
    }

    return true;
}

// sub_8243D0
bool slf__ui_camera_reset_starting_anim_pos__t::operator()(chuck::vm::vm_stack &stack) {
    if (references::frontend.get().igo->camera_widget) {
        references::frontend.get().igo->camera_widget->kill_layer_0_anims();
        references::frontend.get().igo->camera_widget->reset_entity_po();
    }

    return true;
}

// sub_861CF0
bool slf__ui_camera_set_edit_mode__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto enabled = stack.local_num(0x00);

    if (igo_3d_camera_widget* camera = references::frontend.get().igo->camera_widget)
        camera->set_edit_mode(enabled != 0.0f);

    return true;
}

// sub_824400
bool slf__ui_camera_inc_x_pos__t::operator()(chuck::vm::vm_stack &stack) {
    if (igo_3d_camera_widget* camera = references::frontend.get().igo->camera_widget)
        camera->inc_x_pos();

    return true;
}

// sub_824420
bool slf__ui_camera_dec_x_pos__t::operator()(chuck::vm::vm_stack &stack) {
    if (igo_3d_camera_widget* camera = references::frontend.get().igo->camera_widget)
        camera->dec_x_pos();

    return true;
}

// sub_824440
bool slf__ui_camera_inc_y_pos__t::operator()(chuck::vm::vm_stack &stack) {
    if (igo_3d_camera_widget* camera = references::frontend.get().igo->camera_widget)
        camera->inc_y_pos();

    return true;
}

// sub_824460
bool slf__ui_camera_dec_y_pos__t::operator()(chuck::vm::vm_stack &stack) {
    if (igo_3d_camera_widget* camera = references::frontend.get().igo->camera_widget)
        camera->dec_y_pos();

    return true;
}

// sub_824480
bool slf__ui_camera_inc_z_pos__t::operator()(chuck::vm::vm_stack &stack) {
    if (igo_3d_camera_widget* camera = references::frontend.get().igo->camera_widget)
        camera->inc_z_pos();

    return true;
}

// sub_8244A0
bool slf__ui_camera_dec_z_pos__t::operator()(chuck::vm::vm_stack &stack) {
    if (igo_3d_camera_widget* camera = references::frontend.get().igo->camera_widget)
        camera->dec_z_pos();

    return true;
}

// sub_8244C0
bool slf__ui_camera_inc_tilt__t::operator()(chuck::vm::vm_stack &stack) {
    if (igo_3d_camera_widget* camera = references::frontend.get().igo->camera_widget)
        camera->inc_tilt();

    return true;
}

// sub_8244E0
bool slf__ui_camera_dec_tilt__t::operator()(chuck::vm::vm_stack &stack) {
    if (igo_3d_camera_widget* camera = references::frontend.get().igo->camera_widget)
        camera->dec_tilt();

    return true;
}

// sub_824500
bool slf__ui_camera_inc_spin__t::operator()(chuck::vm::vm_stack &stack) {
    if (igo_3d_camera_widget* camera = references::frontend.get().igo->camera_widget)
        camera->inc_spin();

    return true;
}

// sub_824520
bool slf__ui_camera_dec_spin__t::operator()(chuck::vm::vm_stack &stack) {
    if (igo_3d_camera_widget* camera = references::frontend.get().igo->camera_widget)
        camera->dec_spin();

    return true;
}

// sub_824540
bool slf__ui_camera_inc_rotation__t::operator()(chuck::vm::vm_stack &stack) {
    if (igo_3d_camera_widget* camera = references::frontend.get().igo->camera_widget)
        camera->inc_rotation();

    return true;
}

// sub_824560
bool slf__ui_camera_dec_rotation__t::operator()(chuck::vm::vm_stack &stack) {
    if (igo_3d_camera_widget* camera = references::frontend.get().igo->camera_widget)
        camera->dec_rotation();

    return true;
}

// sub_861D40
bool slf__ui_camera_reset_pos__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto duration = stack.local_num(0x00);

    if (references::frontend.get().igo->camera_widget) {
        references::frontend.get().igo->camera_widget->reset_position();

        retail::sub_69A910(references::frontend.get().igo->camera_widget, duration); // igo_3d_camera_widget move home
    }

    return true;
}

// sub_83EDE0
bool slf__add_ui_light__t::operator()(chuck::vm::vm_stack &stack) {
    auto result = (f32)((i32 (__thiscall*)(ui_frontend*))retail::sub_71FB70)(references::frontend.get().igo); // ui_frontend add ui light, always -1

    stack.push(&result, sizeof(result));

    return true;
}

// sub_861D80
bool slf__ui_light_exists__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto id = stack.local_num(0x00);

    ui_light* light = ((ui_light* (__thiscall*)(ui_frontend*, i32))retail::sub_A90D90)(references::frontend.get().igo, (i32)id); // ui_frontend ui light lookup, always null

    f32 result = light ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_861DE0
bool slf__get_ui_light_x__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto id = stack.local_num(0x00);

    ui_light* light = ((ui_light* (__thiscall*)(ui_frontend*, i32))retail::sub_A90D90)(references::frontend.get().igo, (i32)id); // ui_frontend ui light lookup, always null

    f32 result = light ? light->x : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_861E40
bool slf__get_ui_light_y__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto id = stack.local_num(0x00);

    ui_light* light = ((ui_light* (__thiscall*)(ui_frontend*, i32))retail::sub_A90D90)(references::frontend.get().igo, (i32)id); // ui_frontend ui light lookup, always null

    f32 result = light ? light->y : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_861EA0
bool slf__get_ui_light_near_dist__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto id = stack.local_num(0x00);

    ui_light* light = ((ui_light* (__thiscall*)(ui_frontend*, i32))retail::sub_A90D90)(references::frontend.get().igo, (i32)id); // ui_frontend ui light lookup, always null

    f32 result = light ? light->near_dist : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_861F00
bool slf__get_ui_light_far_dist__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto id = stack.local_num(0x00);

    ui_light* light = ((ui_light* (__thiscall*)(ui_frontend*, i32))retail::sub_A90D90)(references::frontend.get().igo, (i32)id); // ui_frontend ui light lookup, always null

    f32 result = light ? light->far_dist : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_861F60
bool slf__set_ui_light_x__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto id    = stack.local_num(0x00);
    auto value = stack.local_num(0x04);

    if (ui_light* light = ((ui_light* (__thiscall*)(ui_frontend*, i32))retail::sub_A90D90)(references::frontend.get().igo, (i32)id)) // ui_frontend ui light lookup, always null
        light->x = value;

    return true;
}

// sub_861F90
bool slf__set_ui_light_y__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto id    = stack.local_num(0x00);
    auto value = stack.local_num(0x04);

    if (ui_light* light = ((ui_light* (__thiscall*)(ui_frontend*, i32))retail::sub_A90D90)(references::frontend.get().igo, (i32)id)) // ui_frontend ui light lookup, always null
        light->y = value;

    return true;
}

// sub_861FC0
bool slf__set_ui_light_near_dist__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto id    = stack.local_num(0x00);
    auto value = stack.local_num(0x04);

    if (ui_light* light = ((ui_light* (__thiscall*)(ui_frontend*, i32))retail::sub_A90D90)(references::frontend.get().igo, (i32)id)) // ui_frontend ui light lookup, always null
        light->near_dist = value;

    return true;
}

// sub_861FF0
bool slf__set_ui_light_far_dist__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto id    = stack.local_num(0x00);
    auto value = stack.local_num(0x04);

    if (ui_light* light = ((ui_light* (__thiscall*)(ui_frontend*, i32))retail::sub_A90D90)(references::frontend.get().igo, (i32)id)) // ui_frontend ui light lookup, always null
        light->far_dist = value;

    return true;
}

// sub_862020
bool slf__set_ui_light_widget_index__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto index = stack.local_num(0x00);

    ((void (__thiscall*)(ui_frontend*, i32))retail::sub_689610)(references::frontend.get().igo, (i32)index); // ui_frontend set ui light widget index, empty

    return true;
}

// sub_869EB0
bool slf__set_debug_3d_text_marker__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    return true;
}

// sub_869EB0
bool slf__set_debug_3d_text_str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    return true;
}

// sub_869EB0
bool slf__set_debug_show_3d_text_regions__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    return true;
}

// sub_8622F0
bool slf__set_debug_child_widget__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    return true;
}

// sub_869EB0
bool slf__reattach_child_widget__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    return true;
}

// sub_862040
bool slf__set_cutscene_texture__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto texture = (const char*)stack.local_uint(0x00);

    alignas(mash::string) u8 name_storage[sizeof(mash::string)];

    auto* name = new (name_storage) mash::string(texture);

    references::frontend.get().set_cutscene_texture(name);

    name->finalize(mash::ALLOCATED);

    return true;
}

// sub_8620B0
bool slf__display_cutscene__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto duration = stack.local_num(0x00);
    auto delay    = stack.local_num(0x04);

    references::frontend.get().display_cutscene(duration, delay);

    return true;
}

// sub_824580
bool slf__hide_cutscene__t::operator()(chuck::vm::vm_stack &stack) {
    references::frontend.get().hide_cutscene();

    return true;
}

// sub_83EE10
bool slf__is_displaying_cutscene__t::operator()(chuck::vm::vm_stack &stack) {
    f32 result = references::frontend.get().draw_cutscene_quad ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_824590
bool slf__send_hide_cutscene_quad_signal__t::operator()(chuck::vm::vm_stack &stack) {
    references::frontend.get().send_hide_cutscene_quad_signal();

    return true;
}

// sub_8245A0
bool slf__display_ped_warning__t::operator()(chuck::vm::vm_stack &stack) {
    ((void (__thiscall*)(frontend_manager*))retail::sub_5B4A70)(&references::frontend.get()); // empty

    return true;
}

// sub_8245B0
bool slf__hide_ped_warning__t::operator()(chuck::vm::vm_stack &stack) {
    ((void (__thiscall*)(frontend_manager*))retail::sub_5B4A70)(&references::frontend.get()); // empty
    ((void (__thiscall*)(frontend_manager*, f32))retail::sub_689610)(&references::frontend.get(), -1.0f); // empty

    return true;
}

// sub_8620E0
bool slf__display_ped_warning_with_duration__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto duration = stack.local_num(0x00);

    ((void (__thiscall*)(frontend_manager*, f32))retail::sub_689610)(&references::frontend.get(), duration); // empty
    ((void (__thiscall*)(frontend_manager*))retail::sub_5B4A70)(&references::frontend.get()); // empty

    return true;
}

// sub_862110
bool slf__create_meter__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto text_id = stack.local_num(0x00);

    ui_frontend* igo = references::frontend.get().igo;

    if (!igo->mission_specific_meter || !igo->mission_specific_meter->is_shown()) {
        igo->set_generic_meter_visible(true);

        alignas(mash::string) u8 label_storage[sizeof(mash::string)];
        alignas(mash::string) u8 storage[sizeof(mash::string)];

        const char* text = references::game.read()->string_localizer->lookup_scripttext_string((i32)text_id);

        auto* label = new (label_storage) mash::string(text);

        references::frontend.get().igo->set_generic_meter_label(new (storage) mash::string(label->c_str()));

        label->finalize(mash::ALLOCATED);
    }

    f32 result = 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8621F0
bool slf__create_meter__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto label = (const char*)stack.local_uint(0x00);

    ui_frontend* igo = references::frontend.get().igo;

    if (!igo->mission_specific_meter || !igo->mission_specific_meter->is_shown()) {
        igo->set_generic_meter_visible(true);

        alignas(mash::string) u8 storage[sizeof(mash::string)];

        references::frontend.get().igo->set_generic_meter_label(new (storage) mash::string(label));
    }

    f32 result = 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_862270
bool slf__create_meter_with_label_placement__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    // the placement is never read
    auto label = (const char*)stack.local_uint(0x00);

    references::frontend.get().igo->set_generic_meter_visible(true);

    alignas(mash::string) u8 storage[sizeof(mash::string)];

    references::frontend.get().igo->set_generic_meter_label(new (storage) mash::string(label));

    f32 result = 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8622D0
bool slf__set_meter_value__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto value = stack.local_num(0x04);

    references::frontend.get().igo->set_generic_meter_value(value);

    return true;
}

// sub_8622F0
bool slf__set_meter_scale__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    return true;
}

// sub_8622F0
bool slf__set_meter_color__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    return true;
}

// sub_860000
bool slf__set_meter_pos__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    return true;
}

// sub_862300
bool slf__delete_meter__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    references::frontend.get().igo->set_generic_meter_visible(false);

    return true;
}

// sub_862320
bool slf__set_meter_visibility__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto visible = stack.local_num(0x00);

    references::frontend.get().igo->set_generic_meter_visible(visible != 0.0f);

    return true;
}

// sub_862360
bool slf__set_meter_label__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto text_id = stack.local_num(0x04);

    if (!references::frontend.get().igo)
        return true;

    alignas(mash::string) u8 label_storage[sizeof(mash::string)];
    alignas(mash::string) u8 storage[sizeof(mash::string)];

    const char* text = references::game.read()->string_localizer->lookup_scripttext_string((i32)text_id);

    auto* label = new (label_storage) mash::string(text);

    references::frontend.get().igo->set_generic_meter_label(new (storage) mash::string(label->c_str()));

    label->finalize(mash::ALLOCATED);

    return true;
}

// sub_862430
bool slf__create_fog__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto position = *(const vector3d*)&stack.local_uint(0x00);
    auto radius   = stack.local_num(0x0C);
    auto unk_2c   = stack.local_num(0x10);

    auto create_fog = (finger_of_god* (__thiscall*)(ui_frontend*, vector3d, f32, f32, bool, i32))retail::sub_7115F0;

    finger_of_god*    fog = create_fog(references::frontend.get().igo, position, radius, unk_2c, true, 0);
    arch_base_vhandle result;

    if (fog) {
        stack.thread->get_instance(-1)->add_allocated_stuff(chuck::vm::script_garbage_collection_finger_of_god, (u32)fog);
        result = fog->my_handle;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_83EE60
bool slf__create_fog__t::operator()(chuck::vm::vm_stack &stack) {
    auto create_fog = (finger_of_god* (__thiscall*)(ui_frontend*, bool))retail::sub_7116D0;

    finger_of_god*    fog = create_fog(references::frontend.get().igo, true);
    arch_base_vhandle result;

    if (fog) {
        stack.thread->get_instance(-1)->add_allocated_stuff(chuck::vm::script_garbage_collection_finger_of_god, (u32)fog);
        result = fog->my_handle;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8624C0
bool slf__destroy_fog__finger_of_god__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    arch_base_vhandle handle(stack.local_uint(0x00));

    auto fog = (u32)handle.resolve();

    stack.thread->get_instance(-1)->remove_allocated_stuff(chuck::vm::script_garbage_collection_finger_of_god, fog);
    references::frontend.get().igo->destroy_fog((finger_of_god*)handle.resolve());

    return true;
}

// sub_862940
bool slf__terminate_script__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    mission_manager::inst()->terminate_script(stack.local_num(0x00) != 0.0f, false);

    return false;
}

// sub_862990
bool slf__terminate_script__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    mission_manager::inst()->terminate_script(stack.local_num(0x00) != 0.0f, false);

    return false;
}

// sub_8629E0
bool slf__format_string__num__t::operator()(chuck::vm::vm_stack &stack) {
    // the count word covers the text id and the format's nums
    i32 count = (i32)stack.pop_num();

    stack.move_sp(-4 * count);

    auto text_id = stack.local_num(0x00);

    alignas(mash::string) u8 format_storage[sizeof(mash::string)];
    alignas(mash::string) u8 result_storage[sizeof(mash::string)];
    alignas(mash::string) u8 storage[sizeof(mash::string)];

    const char* text = references::game.read()->string_localizer->lookup_scripttext_string((i32)text_id);

    auto* format = new (format_storage) mash::string(text);

    mash::string* result = format_script_text((mash::string*)result_storage,
                                              new (storage) mash::string(*format),
                                              0,
                                              &stack.local_num(0x04));

    // the script gets the characters of a string that dies right after
    const char* characters = result->c_str();

    stack.push(&characters, sizeof(characters));

    result->finalize(mash::ALLOCATED);
    format->finalize(mash::ALLOCATED);

    return true;
}

// sub_862AE0
bool slf__show_mission_success_screen__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto type      = stack.local_num(0x00);
    auto parameter = stack.local_num(0x04);

    igo_3d_pauseless_dialog_widget* dialog = references::frontend.get().igo->pauseless_dialog;

    dialog->clear();
    dialog->set_title_by_id(3436);
    dialog->set_text("");

    retail::sub_6ECF90((u32*)dialog); // fills in the results

    dialog->unk_22c = 0;

    if (type == 0.0f) {
        dialog->set_button_1_text(410);
        dialog->show_button_1(true);
        dialog->set_button_2_text(689);
        dialog->show_button_2(true);
    } else {
        dialog->show_button_1(false);

        if (type == 1.0f) {
            dialog->set_button_2_text(689);
            dialog->show_button_2(true);
        } else if (type == 2.0f) {
            dialog->set_button_2_text(3043);
            dialog->show_button_2(true);
        } else if (type == 3.0f) {
            dialog->set_button_2_text(410);
            dialog->show_button_2(true);
        }
    }

    mission_manager::inst()->success_screen_type      = (i32)type;
    mission_manager::inst()->success_screen_parameter = (i32)parameter;

    dialog->igo_3d_dialog_widget::bring_up(1);

    return true;
}

// sub_8245E0
bool slf__close_3d_mission_info_screen__t::operator()(chuck::vm::vm_stack &stack) {
    references::frontend.get().igo->pauseless_dialog->close();

    return true;
}

// sub_862C90
bool slf__lock_minimap__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto center = (const vector3d*)&stack.local_uint(0x00);
    auto zoom   = stack.local_num(0x0C);

    ui_frontend* igo = references::frontend.get().igo;

    if (igo && igo->radar_widget)
        igo->radar_widget->lock_minimap(*center, zoom);

    return true;
}

// sub_824600
bool slf__unlock_minimap__t::operator()(chuck::vm::vm_stack &stack) {
    ui_frontend* igo = references::frontend.get().igo;

    if (igo && igo->radar_widget)
        igo->radar_widget->unlock_minimap();

    return true;
}

// sub_862CE0
bool slf__minimap_buildings_invisible__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto invisible = stack.local_num(0x00);

    ui_frontend* igo = references::frontend.get().igo;

    if (igo && igo->radar_widget)
        igo->radar_widget->set_buildings_invisible(invisible != 0.0f);

    return true;
}

// sub_862D30
bool slf__minimap_wallmode_disable__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto disabled = stack.local_num(0x00);

    ui_frontend* igo = references::frontend.get().igo;

    if (igo && igo->radar_widget)
        igo->radar_widget->set_wallmode_disabled(disabled != 0.0f);

    return true;
}

// sub_83EED0
bool slf__get_collectible_level__t::operator()(chuck::vm::vm_stack &stack) {
    void* manager = references::upgrade_manager.read();

    f32 result;

    if (manager)
        result = (f32)(i32)retail::sub_9216A0((u32*)manager);
    else
        result = 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_862D80
bool slf__temporarily_purchase_upgrade__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    void* manager = references::upgrade_manager.read();

    if (manager)
        retail::sub_921870((u32*)manager, (i32)stack.local_num(0x00));

    return true;
}

// sub_824620
bool slf__temporarily_purchase_all_upgrades__t::operator()(chuck::vm::vm_stack &stack) {
    // the manager opens with every upgrade by id
    auto upgrades = (mash::vector<rpg_upgrade>*)references::upgrade_manager.read();

    if (!upgrades)
        return true;

    i32 count = (i32)upgrades->size;

    for (i32 id = 0; id < count; ++id)
        retail::sub_921870((u32*)references::upgrade_manager.read(), id);

    return true;
}

// sub_862E40
bool slf__is_upgrade_purchased__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    f32 result = 0.0f;

    void* manager = references::upgrade_manager.read();

    if (manager) {
        auto id = stack.local_num(0x00);

        if (id >= 0.0f)
            result = (f32)retail::sub_9219C0((u32*)manager, (i32)id);
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_862E90
bool slf__suspend_instructional_text__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto suspended = stack.local_num(0x00);

    ui_frontend* igo = references::frontend.get().igo;

    if (igo && igo->objective_text)
        igo->objective_text->suspend(suspended != 0.0f);

    return true;
}

// sub_824660
bool slf__clear_instructional_text_no_fade__t::operator()(chuck::vm::vm_stack &stack) {
    ui_frontend* igo = references::frontend.get().igo;

    if (igo && igo->objective_text)
        igo->objective_text->clear(false);

    return true;
}

// sub_824680
bool slf__rvb_clear_instructional_text__t::operator()(chuck::vm::vm_stack &stack) {
    ui_frontend* igo = references::frontend.get().igo;

    if (igo && igo->objective_text)
        igo->objective_text->clear(true);

    return true;
}

// sub_862DB0
bool slf__try_to_purchase_upgrade__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    void* manager = references::upgrade_manager.read();

    if (manager)
        retail::sub_9302D0((i32)manager, (i32)stack.local_num(0x00), 1); // try_to_purchase_upgrade

    return true;
}

// sub_862DE0
bool slf__get_upgrade_id__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    f32 result = -1.0f;

    if (references::upgrade_manager.read()) {
        string_hash name;

        name.initialize(mash::ALLOCATED, (const char*)stack.local_uint(0x00));

        result = (f32)retail::sub_9210B0((u32*)references::upgrade_manager.read(), (u32*)&name); // get_upgrade_id
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8632D0
bool slf__set_rvb_hint_text_debug__num__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    // the second argument is never read
    auto duration = stack.local_num(0x00);
    auto text     = (const char*)stack.local_uint(0x08);

    ui_frontend* igo = references::frontend.get().igo;

    if (igo && igo->hint_text)
        igo->unified_announcement->show_message(text,
                                                duration,
                                                0,
                                                0,
                                                0.0f,
                                                0.0f,
                                                references::hint_text_color.read());

    return true;
}

// sub_83F4F0
bool slf__is_vibration_option_enabled__t::operator()(chuck::vm::vm_stack &stack) {
    game* the_game = references::game.read();

    f32 result;

    if (the_game && the_game->data)
        result = (f32)*retail::sub_77CB90((u32*)the_game->data);
    else
        result = 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8246A0
bool slf__hide_rvb_hint_text__t::operator()(chuck::vm::vm_stack &stack) {
    ui_frontend* igo = references::frontend.get().igo;

    if (igo && igo->hint_text)
        igo->hint_text->clear(true);

    return true;
}

// sub_862EE0
bool slf__set_rvb_instructional_text__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    // the count word covers every argument, the format's nums last
    i32 count = (i32)stack.pop_num();

    stack.move_sp(-4 * count);

    auto duration = stack.local_num(0x00);
    auto text_id  = stack.local_num(0x04);

    ui_frontend* igo = references::frontend.get().igo;

    if (!igo || !igo->objective_text)
        return true;

    alignas(mash::string) u8 format_storage[sizeof(mash::string)];
    alignas(mash::string) u8 result_storage[sizeof(mash::string)];
    alignas(mash::string) u8 storage[sizeof(mash::string)];

    auto* format = new (format_storage) mash::string();

    input_mgr*    inputs = input_mgr::inst();
    input_device* device = inputs->devices[joystick_2_device];

    if (!device)
        device = inputs->devices[joystick_1_device];

    i32 id = (i32)text_id;

    // a pad that answers both shows the next text instead
    if (device && device->unk_034() && device->unk_03c((i32)text_id))
        ++id;

    *format = references::game.read()->string_localizer->lookup_scripttext_string(id);

    mash::string* result = format_script_text((mash::string*)result_storage,
                                              new (storage) mash::string(*format),
                                              0,
                                              &stack.local_num(0x08));

    // a lone "$" shows nothing
    auto* marker = new (storage) mash::string("$");
    bool  blank  = result->is_equal(marker->c_str());
    marker->finalize(mash::ALLOCATED);

    if (blank) {
        auto* empty = new (storage) mash::string("");
        *result = *empty;
        empty->finalize(mash::ALLOCATED);
    }

    igo->objective_text->show_message(*result, duration);

    result->finalize(mash::ALLOCATED);
    format->finalize(mash::ALLOCATED);

    return true;
}

// sub_83F550
bool slf__get_root_debug_menu__t::operator()(chuck::vm::vm_stack &stack) {
    u32 result = 0;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_841EB0
bool slf__is_motion_sensor_option_enabled__t::operator()(chuck::vm::vm_stack &stack) {
    f32 result = 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_824680
bool slf__clear_rvb_instructional_queue__t::operator()(chuck::vm::vm_stack &stack) {
    ui_frontend* igo = references::frontend.get().igo;

    if (igo && igo->objective_text)
        igo->objective_text->clear(true);

    return true;
}

// sub_8630D0
bool slf__set_rvb_instructional_text_debug__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto duration = stack.local_num(0x00);
    auto text     = (const char*)stack.local_uint(0x04);

    ui_frontend* igo = references::frontend.get().igo;

    if (igo && igo->objective_text)
        igo->objective_text->show_message(text, duration);

    return true;
}

// sub_83F570
bool slf__create_polytube__t::operator()(chuck::vm::vm_stack &stack) {
    auto tube = (polytube*)retail::sub_804090((u32**)&references::g_world_ptr.read()->ent_mgr, 0x2000); // wds_entity_manager, creates a "wds_polytube"

    arch_base_vhandle handle = tube->my_handle;

    retail::sub_654A00((i32)tube);

    tube->set_visible(true, false);

    stack.thread->get_instance(-1)->add_allocated_stuff(chuck::vm::script_garbage_collection_entity, handle.value);

    stack.push(&handle, sizeof(handle));

    return true;
}

// sub_860000
bool slf__enumerate_resources_to_debug_menu__debug_menu_entry__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    return true;
}

// sub_8246A0
bool slf__clear_rvb_hint_queue__t::operator()(chuck::vm::vm_stack &stack) {
    ui_frontend* igo = references::frontend.get().igo;

    if (igo && igo->hint_text)
        igo->hint_text->clear(true);

    return true;
}

// sub_863120
bool slf__set_rvb_hint_text__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    // the count word covers every argument, the format's nums last
    i32 count = (i32)stack.pop_num();

    stack.move_sp(-4 * count);

    // the second argument is never read
    auto duration = stack.local_num(0x00);
    auto text_id  = stack.local_num(0x08);

    ui_frontend* igo = references::frontend.get().igo;

    if (!igo || !igo->hint_text)
        return true;

    alignas(mash::string) u8 format_storage[sizeof(mash::string)];
    alignas(mash::string) u8 result_storage[sizeof(mash::string)];
    alignas(mash::string) u8 storage[sizeof(mash::string)];

    const char* text = references::game.read()->string_localizer->lookup_scripttext_string((i32)text_id);

    auto* format = new (format_storage) mash::string(text);

    mash::string* result = format_script_text((mash::string*)result_storage,
                                              new (storage) mash::string(*format),
                                              0,
                                              &stack.local_num(0x0C));

    // a lone "$" shows nothing
    auto* marker = new (storage) mash::string("$");
    bool  blank  = result->is_equal(marker->c_str());
    marker->finalize(mash::ALLOCATED);

    if (blank) {
        auto* empty = new (storage) mash::string("");
        *result = *empty;
        empty->finalize(mash::ALLOCATED);
    }

    // the hint text widget is tested, the message box shows it
    igo->unified_announcement->show_message(*result,
                                            duration,
                                            0,
                                            0,
                                            0.0f,
                                            0.0f,
                                            references::hint_text_color.read());

    result->finalize(mash::ALLOCATED);
    format->finalize(mash::ALLOCATED);

    return true;
}

// sub_85F1A0
bool slf__widget_3d__is_valid__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (igo_3d_script_widget*)stack.local_uint(0x00);

    f32 result = self && references::frontend.get().igo->is_script_widget_valid(self) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_85F220
bool slf__widget_3d__set_shown__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto shown = stack.local_num(0x04);

    if (self && references::frontend.get().igo->is_script_widget_valid(self))
        self->set_shown(shown != 0.0f);

    return true;
}

// sub_85F280
bool slf__widget_3d__set_position__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto x    = stack.local_num(0x04);
    auto y    = stack.local_num(0x08);

    if (self && references::frontend.get().igo->is_script_widget_valid(self))
        self->set_center_pos(x, y);

    return true;
}

// sub_85F2C0
bool slf__widget_3d__set_position__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self     = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto position = (const vector3d*)&stack.local_uint(0x04);

    if (self && references::frontend.get().igo->is_script_widget_valid(self))
        self->set_position(position->x, position->y, position->z);

    return true;
}

// sub_85F310
bool slf__widget_3d__set_z__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto z    = stack.local_num(0x04);

    if (self && references::frontend.get().igo->is_script_widget_valid(self)) {
        f32 x;
        f32 y;

        self->get_position(&x, &y);
        self->set_position(x, y, z);
    }

    return true;
}

// sub_85F370
bool slf__widget_3d__set_scale__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto scale = stack.local_num(0x04);

    if (self && references::frontend.get().igo->is_script_widget_valid(self))
        self->set_scale(scale);

    return true;
}

// sub_85F3B0
bool slf__widget_3d__set_scale_offset__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self   = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto offset = (const vector3d*)&stack.local_uint(0x04);

    if (self && references::frontend.get().igo->is_script_widget_valid(self))
        self->set_scale_offset(offset->x, offset->y, offset->z);

    return true;
}

// sub_85F400
bool slf__widget_3d__set_tilt__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto tilt = stack.local_num(0x04);

    if (self && references::frontend.get().igo->is_script_widget_valid(self))
        self->set_tilt_angle(tilt);

    return true;
}

// sub_85F440
bool slf__widget_3d__set_spin__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto spin = stack.local_num(0x04);

    if (self && references::frontend.get().igo->is_script_widget_valid(self))
        self->set_spin_angle(spin);

    return true;
}

// sub_85F480
bool slf__widget_3d__set_rotation__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self     = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto rotation = stack.local_num(0x04);

    if (self && references::frontend.get().igo->is_script_widget_valid(self))
        self->set_rotation_angle(rotation);

    return true;
}

// sub_85F4C0
bool slf__widget_3d__set_max_timer__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self      = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto max_timer = stack.local_num(0x04);

    if (self && references::frontend.get().igo->is_script_widget_valid(self))
        self->max_timer = max_timer;

    return true;
}

// sub_85F500
bool slf__widget_3d__set_color__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self  = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto color = (const vector3d*)&stack.local_uint(0x04);

    if (self && references::frontend.get().igo->is_script_widget_valid(self))
        self->set_color(color->x, color->y, color->z);

    return true;
}

// sub_85F550
bool slf__widget_3d__set_continuous_tilt__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto rate = stack.local_num(0x04);

    if (self && references::frontend.get().igo->is_script_widget_valid(self))
        self->set_continuous_tilt(rate);

    return true;
}

// sub_85F590
bool slf__widget_3d__set_continuous_spin__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto rate = stack.local_num(0x04);

    if (self && references::frontend.get().igo->is_script_widget_valid(self))
        self->set_continuous_spin(rate);

    return true;
}

// sub_85F5D0
bool slf__widget_3d__set_continuous_rotation__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto rate = stack.local_num(0x04);

    if (self && references::frontend.get().igo->is_script_widget_valid(self))
        self->set_continuous_rotation(rate);

    return true;
}

// sub_85F610
bool slf__widget_3d__set_widescreen_alignment__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self      = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto alignment = stack.local_num(0x04);

    if (self && references::frontend.get().igo->is_script_widget_valid(self))
        self->set_widescreen_alignment((igo_3d_widget_parameters::e_wide_align_flags)(i32)alignment);

    return true;
}

// sub_85F650
bool slf__widget_3d__set_uv_offset__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto u    = stack.local_num(0x04);
    auto v    = stack.local_num(0x08);

    if (self && references::frontend.get().igo->is_script_widget_valid(self))
        self->set_uv_offset(u, v);

    return true;
}

// sub_85F690
bool slf__widget_3d__fade_in__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self     = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto duration = stack.local_num(0x04);

    if (self && references::frontend.get().igo->is_script_widget_valid(self))
        self->fade_in(duration, 0.0f, 1.0f, 0.0f);

    return true;
}

// sub_85F6E0
bool slf__widget_3d__fade_out__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self     = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto duration = stack.local_num(0x04);

    if (self && references::frontend.get().igo->is_script_widget_valid(self))
        self->fade_out(duration, 1.0f, 0.0f, 0.0f);

    return true;
}

// sub_85F730
bool slf__widget_3d__set_alpha__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto alpha = stack.local_num(0x04);

    if (self && references::frontend.get().igo->is_script_widget_valid(self))
        self->set_alpha(alpha);

    return true;
}

// sub_85F770
bool slf__widget_3d__play_anim__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto anim = (const char*)stack.local_uint(0x04);

    native_resource_context context(stack);

    if (self && references::frontend.get().igo->is_script_widget_valid(self)) {
        self->kill_layer_0_anims();

        tl_fixed_string name(anim);

        self->play_anim(name);
    }

    return true;
}

// sub_85F840
bool slf__widget_3d__play_button_anim__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self  = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto kind  = stack.local_num(0x04);
    auto index = stack.local_num(0x08);

    if (!self || !references::frontend.get().igo->is_script_widget_valid(self))
        return true;

    alignas(mash::string) u8 storage[sizeof(mash::string)];

    igo_3d_face_button_system* buttons = references::frontend.get().igo->face_button_system;

    mash::string* anim = buttons->get_button_anim_name((mash::string*)storage, (i32)kind, (i32)index);

    self->kill_layer_0_anims();

    tl_fixed_string name(anim->c_str());

    self->play_anim(name);

    anim->finalize(mash::ALLOCATED);

    return true;
}

// sub_85F8F0
bool slf__widget_3d__is_anim_playing__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (igo_3d_script_widget*)stack.local_uint(0x00);

    f32 result = self && references::frontend.get().igo->is_script_widget_valid(self) ? (f32)self->is_anim_playing() : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_85F960
bool slf__widget_3d__get_text_id__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto member = (const char*)stack.local_uint(0x04);

    f32 result = -2.0f;

    if (self && references::frontend.get().igo->is_script_widget_valid(self)) {
        string_hash nodename;
        nodename.initialize(mash::ALLOCATED, member);

        result = (f32)self->get_mesh_index(nodename);
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_85FA00
bool slf__widget_3d__set_text_shown__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self  = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto id    = stack.local_num(0x04);
    auto shown = stack.local_num(0x08);

    if (self && references::frontend.get().igo->is_script_widget_valid(self)) {
        igo_3d_text* text = self->get_text((i32)id);

        if (text)
            text->set_shown(shown != 0.0f);
    }

    return true;
}

// sub_85FA70
bool slf__widget_3d__set_text__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    // the count word covers every argument after self, the format's nums last
    i32 count = (i32)stack.pop_num();

    stack.move_sp(-4 * (count + 1));

    auto self    = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto id      = stack.local_num(0x04);
    auto text_id = stack.local_num(0x08);

    if (!self || !references::frontend.get().igo->is_script_widget_valid(self))
        return true;

    igo_3d_text* text = self->get_text((i32)id);

    if (!text)
        return true;

    // every string here dies through finalize, the destructor's twin
    alignas(mash::string) u8 format_storage[sizeof(mash::string)];
    alignas(mash::string) u8 result_storage[sizeof(mash::string)];
    alignas(mash::string) u8 storage[sizeof(mash::string)];

    auto* format = new (format_storage) mash::string();

    input_mgr*    inputs = input_mgr::inst();
    input_device* device = inputs->devices[joystick_2_device];

    if (!device)
        device = inputs->devices[joystick_1_device];

    *format = references::game.read()->string_localizer->lookup_scripttext_string((i32)text_id);

    if (device && device->unk_034())
        message_text::substitute_pad_tokens(format);

    mash::string* result = format_script_text((mash::string*)result_storage,
                                              new (storage) mash::string(*format),
                                              0,
                                              &stack.local_num(0x0C));

    // a lone "$" shows nothing
    auto* marker = new (storage) mash::string("$");
    bool  blank  = result->is_equal(marker->c_str());
    marker->finalize(mash::ALLOCATED);

    if (blank) {
        auto* empty = new (storage) mash::string("");
        *result = *empty;
        empty->finalize(mash::ALLOCATED);
    }

    text->set_text_no_localize(result->c_str());

    result->finalize(mash::ALLOCATED);
    format->finalize(mash::ALLOCATED);

    return true;
}

// sub_85FD10
bool slf__widget_3d__set_null_text__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto id   = stack.local_num(0x04);

    if (self && references::frontend.get().igo->is_script_widget_valid(self)) {
        igo_3d_text* text = self->get_text((i32)id);

        if (text)
            text->set_text_no_localize("");
    }

    return true;
}

// sub_85FD70
bool slf__widget_3d__set_text_debug__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    i32 count = (i32)stack.pop_num();

    stack.move_sp(-4 * (count + 1));

    return true;
}

// sub_85FDB0
bool slf__widget_3d__set_text_pos__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto id   = stack.local_num(0x04);
    auto x    = stack.local_num(0x08);
    auto y    = stack.local_num(0x0C);

    if (!self || !references::frontend.get().igo->is_script_widget_valid(self))
        return true;

    igo_3d_text* text = self->get_text((i32)id);

    if (text) {
        text->set_text_position_x(x);
        text->set_text_position_y(y);
    }

    return true;
}

// sub_85FE20
bool slf__widget_3d__set_text_dimensions__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self   = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto id     = stack.local_num(0x04);
    auto width  = stack.local_num(0x08);
    auto height = stack.local_num(0x0C);

    if (self && references::frontend.get().igo->is_script_widget_valid(self)) {
        igo_3d_text* text = self->get_text((i32)id);

        if (text)
            text->set_dimensions(width, height);
    }

    return true;
}

// sub_85FE70
bool slf__widget_3d__set_text_scale__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self  = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto id    = stack.local_num(0x04);
    auto scale = stack.local_num(0x08);

    if (self && references::frontend.get().igo->is_script_widget_valid(self)) {
        igo_3d_text* text = self->get_text((i32)id);

        if (text)
            text->set_font_scale(scale);
    }

    return true;
}

// sub_85FEC0
bool slf__widget_3d__set_text_button_scale__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self  = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto id    = stack.local_num(0x04);
    auto scale = stack.local_num(0x08);

    if (self && references::frontend.get().igo->is_script_widget_valid(self)) {
        igo_3d_text* text = self->get_text((i32)id);

        if (text)
            text->set_button_scale(scale);
    }

    return true;
}

// sub_85FF10
bool slf__widget_3d__set_text_valign__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self  = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto id    = stack.local_num(0x04);
    auto align = stack.local_num(0x08);

    if (self && references::frontend.get().igo->is_script_widget_valid(self)) {
        igo_3d_text* text = self->get_text((i32)id);

        if (text)
            text->set_v_justification((i32)align);
    }

    return true;
}

// sub_85FF60
bool slf__widget_3d__set_text_halign__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self  = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto id    = stack.local_num(0x04);
    auto align = stack.local_num(0x08);

    if (self && references::frontend.get().igo->is_script_widget_valid(self)) {
        igo_3d_text* text = self->get_text((i32)id);

        if (text)
            text->set_h_justification((i32)align);
    }

    return true;
}

// sub_85FFB0
bool slf__widget_3d__set_text_font__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto id   = stack.local_num(0x04);
    auto font = stack.local_num(0x08);

    if (self && references::frontend.get().igo->is_script_widget_valid(self)) {
        igo_3d_text* text = self->get_text((i32)id);

        if (text)
            text->set_font((i32)font, 2);
    }

    return true;
}

// sub_860000
bool slf__widget_3d__set_text_color__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    return true;
}

// sub_860010
bool slf__widget_3d__set_text_color__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self  = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto id    = stack.local_num(0x04);
    auto color = (const vector3d*)&stack.local_uint(0x08);

    if (self && references::frontend.get().igo->is_script_widget_valid(self)) {
        igo_3d_text* text = self->get_text((i32)id);

        if (text)
            text->set_font_color(color->x, color->y, color->z);
    }

    return true;
}

// sub_860070
bool slf__widget_3d__set_text_wordwrap__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto id   = stack.local_num(0x04);
    auto wrap = stack.local_num(0x08);

    if (self && references::frontend.get().igo->is_script_widget_valid(self)) {
        igo_3d_text* text = self->get_text((i32)id);

        if (text)
            text->set_word_wrap(wrap != 0.0f);
    }

    return true;
}

// sub_8600E0
bool slf__widget_3d__set_text_x_scale__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self  = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto id    = stack.local_num(0x04);
    auto scale = stack.local_num(0x08);

    if (self && references::frontend.get().igo->is_script_widget_valid(self)) {
        igo_3d_text* text = self->get_text((i32)id);

        if (text)
            text->set_x_scale(scale);
    }

    return true;
}

// sub_860130
bool slf__widget_3d__set_text_character_gap__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto id   = stack.local_num(0x04);
    auto gap  = stack.local_num(0x08);

    if (self && references::frontend.get().igo->is_script_widget_valid(self)) {
        igo_3d_text* text = self->get_text((i32)id);

        if (text)
            text->set_character_gap(gap);
    }

    return true;
}

// sub_860180
bool slf__widget_3d__set_text_wordwrap_line_gap__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto id   = stack.local_num(0x04);
    auto gap  = stack.local_num(0x08);

    if (self && references::frontend.get().igo->is_script_widget_valid(self)) {
        igo_3d_text* text = self->get_text((i32)id);

        if (text)
            text->set_word_wrap_line_gap(gap);
    }

    return true;
}

// sub_8601D0
bool slf__widget_3d__set_text_shadow__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self  = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto id    = stack.local_num(0x04);
    auto color = (const vector3d*)&stack.local_uint(0x08);

    if (!self || !references::frontend.get().igo->is_script_widget_valid(self))
        return true;

    igo_3d_text* text = self->get_text((i32)id);

    if (text) {
        text->set_shadow(true);
        text->set_shadow_color(color->x, color->y, color->z);
    }

    return true;
}

// sub_860240
bool slf__widget_3d__set_text_shadow_alpha__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self  = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto id    = stack.local_num(0x04);
    auto alpha = stack.local_num(0x08);

    if (!self || !references::frontend.get().igo->is_script_widget_valid(self))
        return true;

    igo_3d_text* text = self->get_text((i32)id);

    if (!text)
        return true;

    // no alpha turns the shadow off and keeps the old alpha
    if (alpha <= 0.0f)
        text->set_shadow(false);
    else {
        text->set_shadow(true);
        text->set_shadow_alpha(alpha);
    }

    return true;
}

// sub_8602B0
bool slf__widget_3d__set_text_typewriter__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto id   = stack.local_num(0x04);
    auto on   = stack.local_num(0x08);

    if (self && references::frontend.get().igo->is_script_widget_valid(self)) {
        igo_3d_text* text = self->get_text((i32)id);

        if (text)
            text->typewriter = on != 0.0f;
    }

    return true;
}

// sub_860310
bool slf__widget_3d__is_text_done_updating__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto id   = stack.local_num(0x04);

    igo_3d_text* text = nullptr;

    if (self && references::frontend.get().igo->is_script_widget_valid(self))
        text = self->get_text((i32)id);

    f32 result = text ? (f32)text->all_lines_revealed() : 1.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8603A0
bool slf__widget_3d__end_text_typewriter__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto id   = stack.local_num(0x04);

    if (self && references::frontend.get().igo->is_script_widget_valid(self)) {
        igo_3d_text* text = self->get_text((i32)id);

        if (text)
            text->typewriter = false;
    }

    return true;
}

// sub_8603E0
bool slf__widget_3d__get_child_id_from_name__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto name = (const char*)stack.local_uint(0x04);

    f32 result = -2.0f;

    if (self && references::frontend.get().igo->is_script_widget_valid(self)) {
        alignas(mash::string) u8 storage[sizeof(mash::string)];

        result = (f32)self->find_child(new (storage) mash::string(name));
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8603E0
bool slf__widget_3d__get_text_id_from_name__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto name = (const char*)stack.local_uint(0x04);

    f32 result = -2.0f;

    if (self && references::frontend.get().igo->is_script_widget_valid(self)) {
        alignas(mash::string) u8 storage[sizeof(mash::string)];

        result = (f32)self->find_child(new (storage) mash::string(name));
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_860460
bool slf__widget_3d__set_persistent__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (igo_3d_script_widget*)stack.local_uint(0x00);
    auto on   = stack.local_num(0x04);

    if (self && references::frontend.get().igo->is_script_widget_valid(self))
        self->persistent = on != 0.0f;

    return true;
}

// sub_862510
bool slf__finger_of_god__set_visibility__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (finger_of_god*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self)
        self->set_poi_active(on != 0.0f);

    return true;
}

// sub_862560
bool slf__finger_of_god__set_active__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (finger_of_god*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self)
        self->set_active(on != 0.0f);

    return true;
}

// sub_8625B0
bool slf__finger_of_god__set_pos__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self = (finger_of_god*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self)
        self->set_pos(*(const vector3d*)&stack.local_uint(0x04));

    return true;
}

// sub_8625F0
bool slf__finger_of_god__set_radius__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (finger_of_god*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto radius = stack.local_num(0x04);

    if (self)
        self->set_radius(radius);

    return true;
}

// sub_862620
bool slf__finger_of_god__deactive_fog__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (finger_of_god*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self)
        self->set_active(false);

    return true;
}

// sub_862640
bool slf__finger_of_god__set_poi_icon__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (finger_of_god*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto icon = stack.local_num(0x04);

    if (self)
        self->set_poi_icon((i32)icon, 1);

    return true;
}

// sub_862510
bool slf__finger_of_god__toggle_fog__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (finger_of_god*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self)
        self->set_poi_active(on != 0.0f);

    return true;
}

// sub_862670
bool slf__finger_of_god__fog_requires_senses__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (finger_of_god*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self)
        self->fog_requires_senses(on != 0.0f);

    return true;
}

// sub_8626C0
bool slf__finger_of_god__place_fog__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x18;

    auto self   = (finger_of_god*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto radius = stack.local_num(0x10);
    auto unk_2c = stack.local_num(0x14);

    if (self) {
        self->set_pos(*(const vector3d*)&stack.local_uint(0x04));
        self->set_radius(radius);
        self->unk_2c = unk_2c;
    }

    return true;
}

// sub_862710
bool slf__finger_of_god__place_fog_reappear__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self   = (finger_of_god*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto radius = stack.local_num(0x10);

    if (self) {
        self->set_pos(*(const vector3d*)&stack.local_uint(0x04));
        self->set_radius(radius);
    }

    return true;
}

// sub_862760
bool slf__finger_of_god__arrived_at_fog__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (finger_of_god*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self) {
        self->set_active(false);
        self->arrived_at_fog();
    }

    return true;
}

// sub_862790
bool slf__finger_of_god__set_poi_active__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (finger_of_god*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (!self)
        return true;

    if (entity_tracker* tracker = self->get_tracker())
        tracker->set_poi_active(on != 0.0f);

    return true;
}

// sub_8627E0
bool slf__finger_of_god__set_gta_widget_active__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (finger_of_god*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (!self)
        return true;

    if (entity_tracker* tracker = self->get_tracker())
        tracker->set_gta_widget_active(on != 0.0f, false);

    return true;
}

// sub_862830
bool slf__finger_of_god__set_gta_widget_inner_radius__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (finger_of_god*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto radius = stack.local_num(0x04);

    if (!self)
        return true;

    if (entity_tracker* tracker = self->get_tracker())
        tracker->set_gta_widget_inner_ring_radius(radius);

    return true;
}

// sub_862870
bool slf__finger_of_god__set_gta_widget_outer_radius__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (finger_of_god*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto radius = stack.local_num(0x04);

    if (!self)
        return true;

    if (entity_tracker* tracker = self->get_tracker())
        tracker->set_gta_widget_outer_ring_radius(radius);

    return true;
}

// sub_8628B0
bool slf__finger_of_god__set_gta_widget_pos_offset__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self = (finger_of_god*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (!self)
        return true;

    if (entity_tracker* tracker = self->get_tracker())
        tracker->set_gta_widget_pos_offset(*(const vector3d*)&stack.local_uint(0x04));

    return true;
}

// sub_862900
bool slf__finger_of_god__set_gta_widget_render_scale__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (finger_of_god*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto scale = stack.local_num(0x04);

    if (!self)
        return true;

    if (entity_tracker* tracker = self->get_tracker())
        tracker->set_gta_widget_render_scale(scale);

    return true;
}
