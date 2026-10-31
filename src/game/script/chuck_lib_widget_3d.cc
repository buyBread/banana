#include <new>

#include "retail.hh"
#include "game/arch_base_vhandle.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_camera_widget.hh"
#include "game/frontend/igo/igo_3d_face_button_system.hh"
#include "game/app.hh"
#include "game/frontend/igo/igo_3d_script_widget.hh"
#include "game/frontend/igo/igo_3d_text.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/game.hh"
#include "game/input/input_mgr.hh"
#include "game/localized_string_table.hh"
#include "game/math/references.hh"
#include "game/script/chuck_lib_widget.hh"
#include "game/script/chuck_lib_widget_3d.hh"
#include "game/script/script_access.hh"
#include "treyarch/base/tl_fixed_string.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/string_hash.hh"

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

        result = ((igo_3d_script_widget* (__thiscall*)(ui_frontend*, string_hash))retail::sub_70AC10)(references::frontend.get().igo, key); // ui_frontend::create_script_widget

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

        result = ((igo_3d_script_widget* (__thiscall*)(ui_frontend*, string_hash))retail::sub_70AD30)(references::frontend.get().igo, key); // ui_frontend::create_script_widget_from_params

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

        result = ((igo_3d_script_widget* (__thiscall*)(ui_frontend*, string_hash, string_hash, string_hash))retail::sub_70AF40)(references::frontend.get().igo, // ui_frontend::create_mission_text_widget
                                                                                                                              record_key,
                                                                                                                              widget_key,
                                                                                                                              text_key);

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

        result = ((igo_3d_script_widget* (__thiscall*)(ui_frontend*, string_hash, string_hash, string_hash))retail::sub_70B050)(references::frontend.get().igo, // ui_frontend::create_hint_text_widget
                                                                                                                              record_key,
                                                                                                                              widget_key,
                                                                                                                              text_key);

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

        result = ((igo_3d_script_widget* (__thiscall*)(ui_frontend*, string_hash, string_hash))retail::sub_70AE40)(references::frontend.get().igo, widget_key, text_key); // ui_frontend::create_text_widget_from_params

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

    auto result = ((igo_3d_script_widget* (__thiscall*)(ui_frontend*, string_hash))retail::sub_70AC10)(references::frontend.get().igo, key); // ui_frontend::create_script_widget

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

bool slf__enable_health_meter__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860BD0)(this, stack);
}

bool slf__enable_radar__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860C20)(this, stack);
}

bool slf__enable_boss_meter__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860C70)(this, stack);
}

bool slf__disable_boss_meter__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824290)(this, stack);
}

bool slf__add_boss_meter__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860CB0)(this, stack);
}

bool slf__add_boss_meter__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860D00)(this, stack);
}

bool slf__remove_boss_meter__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860D40)(this, stack);
}

bool slf__remove_all_boss_meters__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824290)(this, stack);
}

bool slf__get_boss_meter_health_value__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860D70)(this, stack);
}

bool slf__set_boss_meter_health_value__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860DB0)(this, stack);
}

bool slf__clear_mission_objectives__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8242B0)(this, stack);
}

bool slf__add_mission_objective__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860DE0)(this, stack);
}

bool slf__add_mission_objective__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860EF0)(this, stack);
}

bool slf__add_mission_objective_debug__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860FF0)(this, stack);
}

bool slf__clear_3d_dialog_text__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824360)(this, stack);
}

bool slf__add_3d_dialog_text__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861030)(this, stack);
}

bool slf__add_3d_dialog_text__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861140)(this, stack);
}

bool slf__add_3d_dialog_text_debug__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860FF0)(this, stack);
}

bool slf__add_3d_objective_text__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861240)(this, stack);
}

bool slf__add_3d_objective_text__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861350)(this, stack);
}

bool slf__add_3d_objective_text_debug__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860FF0)(this, stack);
}

bool slf__bring_up_3d_dialog__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861450)(this, stack);
}

bool slf__set_3d_dialog_title__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861570)(this, stack);
}

bool slf__set_3d_dialog_title_debug__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__show_game_button__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861610)(this, stack);
}

bool slf__remove_all_game_buttons__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8243B0)(this, stack);
}

bool slf__remove_game_button__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861690)(this, stack);
}

bool slf__play_game_button_anim__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8616C0)(this, stack);
}

bool slf__game_button_anim_playing__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861700)(this, stack);
}

bool slf__set_game_button_pos__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861770)(this, stack);
}

bool slf__set_game_button_pos__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8617B0)(this, stack);
}

bool slf__get_2d_ui_pos_from_world_pos__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8617F0)(this, stack);
}

bool slf__set_game_button_scale__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861B00)(this, stack);
}

bool slf__set_game_button_alpha__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861B30)(this, stack);
}

bool slf__show_X_over_button__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861B60)(this, stack);
}

bool slf__is_X_over_button_shown__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861BB0)(this, stack);
}

bool slf__load_debug_3d_widget__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__set_debug_3d_widget_pos__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__get_debug_3d_widget_x__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841EB0)(this, stack);
}

bool slf__get_debug_3d_widget_y__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841EB0)(this, stack);
}

bool slf__set_debug_3d_widget_scale__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__get_debug_3d_widget_scale__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841EB0)(this, stack);
}

bool slf__set_debug_3d_widget_tilt__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__get_debug_3d_widget_tilt__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841EB0)(this, stack);
}

bool slf__set_debug_3d_widget_spin__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__get_debug_3d_widget_spin__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841EB0)(this, stack);
}

bool slf__set_debug_3d_widget_rotation__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__get_debug_3d_widget_rotation__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841EB0)(this, stack);
}

bool slf__get_debug_3d_widget_entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83F550)(this, stack);
}

bool slf__debug_3d_widget_play_anim__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__debug_3d_widget_set_cowbell_variant__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__debug_3d_widget_add_ui_parameter_files_to_menu__debug_menu__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
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

        ((void (__thiscall*)(igo_3d_widget*, const tl_fixed_string*))retail::sub_699A10)(references::frontend.get().igo->camera_widget, &name); // igo_3d_widget::play_anim
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

bool slf__add_ui_light__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83EDE0)(this, stack);
}

bool slf__ui_light_exists__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861D80)(this, stack);
}

bool slf__get_ui_light_x__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861DE0)(this, stack);
}

bool slf__get_ui_light_y__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861E40)(this, stack);
}

bool slf__get_ui_light_near_dist__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861EA0)(this, stack);
}

bool slf__get_ui_light_far_dist__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861F00)(this, stack);
}

bool slf__set_ui_light_x__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861F60)(this, stack);
}

bool slf__set_ui_light_y__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861F90)(this, stack);
}

bool slf__set_ui_light_near_dist__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861FC0)(this, stack);
}

bool slf__set_ui_light_far_dist__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861FF0)(this, stack);
}

bool slf__set_ui_light_widget_index__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862020)(this, stack);
}

bool slf__set_debug_3d_text_marker__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__set_debug_3d_text_str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__set_debug_show_3d_text_regions__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__set_debug_child_widget__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__reattach_child_widget__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__set_cutscene_texture__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862040)(this, stack);
}

bool slf__display_cutscene__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8620B0)(this, stack);
}

bool slf__hide_cutscene__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824580)(this, stack);
}

bool slf__is_displaying_cutscene__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83EE10)(this, stack);
}

bool slf__send_hide_cutscene_quad_signal__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824590)(this, stack);
}

bool slf__display_ped_warning__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8245A0)(this, stack);
}

bool slf__hide_ped_warning__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8245B0)(this, stack);
}

bool slf__display_ped_warning_with_duration__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8620E0)(this, stack);
}

bool slf__create_meter__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862110)(this, stack);
}

bool slf__create_meter__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8621F0)(this, stack);
}

bool slf__create_meter_with_label_placement__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862270)(this, stack);
}

bool slf__set_meter_value__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622D0)(this, stack);
}

bool slf__set_meter_scale__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__set_meter_color__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__set_meter_pos__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860000)(this, stack);
}

bool slf__delete_meter__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862300)(this, stack);
}

bool slf__set_meter_visibility__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862320)(this, stack);
}

bool slf__set_meter_label__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862360)(this, stack);
}

bool slf__create_fog__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862430)(this, stack);
}

bool slf__create_fog__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83EE60)(this, stack);
}

bool slf__destroy_fog__finger_of_god__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8624C0)(this, stack);
}

bool slf__terminate_script__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862940)(this, stack);
}

bool slf__terminate_script__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862990)(this, stack);
}

bool slf__format_string__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8629E0)(this, stack);
}

bool slf__show_mission_success_screen__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862AE0)(this, stack);
}

bool slf__close_3d_mission_info_screen__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8245E0)(this, stack);
}

bool slf__lock_minimap__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862C90)(this, stack);
}

bool slf__unlock_minimap__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824600)(this, stack);
}

bool slf__minimap_buildings_invisible__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862CE0)(this, stack);
}

bool slf__minimap_wallmode_disable__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862D30)(this, stack);
}

bool slf__get_collectible_level__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83EED0)(this, stack);
}

bool slf__temporarily_purchase_upgrade__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862D80)(this, stack);
}

bool slf__temporarily_purchase_all_upgrades__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824620)(this, stack);
}

bool slf__is_upgrade_purchased__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862E40)(this, stack);
}

bool slf__suspend_instructional_text__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862E90)(this, stack);
}

bool slf__clear_instructional_text_no_fade__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824660)(this, stack);
}

bool slf__rvb_clear_instructional_text__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824680)(this, stack);
}

bool slf__try_to_purchase_upgrade__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862DB0)(this, stack);
}

bool slf__get_upgrade_id__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862DE0)(this, stack);
}

bool slf__set_rvb_hint_text_debug__num__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8632D0)(this, stack);
}

bool slf__is_vibration_option_enabled__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83F4F0)(this, stack);
}

bool slf__hide_rvb_hint_text__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8246A0)(this, stack);
}

bool slf__set_rvb_instructional_text__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862EE0)(this, stack);
}

bool slf__get_root_debug_menu__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83F550)(this, stack);
}

bool slf__is_motion_sensor_option_enabled__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841EB0)(this, stack);
}

bool slf__clear_rvb_instructional_queue__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824680)(this, stack);
}

bool slf__set_rvb_instructional_text_debug__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8630D0)(this, stack);
}

bool slf__create_polytube__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83F570)(this, stack);
}

bool slf__enumerate_resources_to_debug_menu__debug_menu_entry__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860000)(this, stack);
}

bool slf__clear_rvb_hint_queue__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8246A0)(this, stack);
}

bool slf__set_rvb_hint_text__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_863120)(this, stack);
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

        ((void (__thiscall*)(igo_3d_widget*, const tl_fixed_string*))retail::sub_699A10)(self, &name); // igo_3d_widget::play_anim
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

    ((void (__thiscall*)(igo_3d_widget*, const tl_fixed_string*))retail::sub_699A10)(self, &name); // igo_3d_widget::play_anim

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

    if (device && device->unk_034()) {
        alignas(4) u8 message[0x30]; // message_text

        auto set_text = (void          (__thiscall*)(void*, mash::string))retail::sub_68B3C0;
        auto get_text = (mash::string* (__thiscall*)(void*, mash::string*))retail::sub_90AB90;

        retail::sub_68B310((u32*)message); // message_text::message_text

        set_text(message, mash::string(*format));

        retail::sub_6B2760((u32*)message); // rewrites the pad tokens

        mash::string* rewritten = get_text(message, (mash::string*)storage);

        bool changed = format->compare(rewritten->c_str()) != 0;

        rewritten->finalize(mash::ALLOCATED);

        if (changed) {
            rewritten = get_text(message, (mash::string*)storage);

            *format = *rewritten;

            rewritten->finalize(mash::ALLOCATED);
        }

        retail::sub_68B440((i32*)message);
        retail::sub_68B3A0((u32*)message); // message_text::~message_text
    }

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

bool slf__finger_of_god__set_visibility__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862510)(this, stack);
}

bool slf__finger_of_god__set_active__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862560)(this, stack);
}

bool slf__finger_of_god__set_pos__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8625B0)(this, stack);
}

bool slf__finger_of_god__set_radius__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8625F0)(this, stack);
}

bool slf__finger_of_god__deactive_fog__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862620)(this, stack);
}

bool slf__finger_of_god__set_poi_icon__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862640)(this, stack);
}

bool slf__finger_of_god__toggle_fog__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862510)(this, stack);
}

bool slf__finger_of_god__fog_requires_senses__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862670)(this, stack);
}

bool slf__finger_of_god__place_fog__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8626C0)(this, stack);
}

bool slf__finger_of_god__place_fog_reappear__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862710)(this, stack);
}

bool slf__finger_of_god__arrived_at_fog__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862760)(this, stack);
}

bool slf__finger_of_god__set_poi_active__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862790)(this, stack);
}

bool slf__finger_of_god__set_gta_widget_active__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8627E0)(this, stack);
}

bool slf__finger_of_god__set_gta_widget_inner_radius__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862830)(this, stack);
}

bool slf__finger_of_god__set_gta_widget_outer_radius__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862870)(this, stack);
}

bool slf__finger_of_god__set_gta_widget_pos_offset__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8628B0)(this, stack);
}

bool slf__finger_of_god__set_gta_widget_render_scale__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862900)(this, stack);
}
