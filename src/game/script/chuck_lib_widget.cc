#include <new>

#include "retail.hh"
#include "game/app.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_radar_widget.hh"
#include "game/frontend/igo/igo_3d_timer_widget.hh"
#include "game/frontend/igo/igo_3d_zoom_map.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/game.hh"
#include "game/localized_string_table.hh"
#include "game/script/chuck_lib_widget.hh"
#include "treyarch/chuck/vm/vm_dynamic_array_manager.hh"

using namespace treyarch;

// sub_823D50
mash::string* treyarch::format_script_text(mash::string* out,
                                           mash::string* format,
                                           i32           byte_offset,
                                           const f32*    nums) {

    new (out) mash::string();

    i32 mark = format->find("%", 0);

    if (mark == mash::string::npos)
        *out = *format;
    else {
        // every temporary dies through finalize, the destructor's twin
        alignas(mash::string) u8 storage[sizeof(mash::string)];
        alignas(mash::string) u8 separator_storage[sizeof(mash::string)];

        nums = (const f32*)((const u8*)nums + byte_offset);

        i32 previous = -2;

        do {
            auto* piece = new (storage) mash::string(format->substr(previous + 2, mark - previous - 2));
            *out += *piece;
            piece->finalize(mash::ALLOCATED);

            switch (format->data()[mark + 1]) {
                case '%':
                    *out += "%";
                    break;

                case 'd': {
                    auto* number = new (storage) mash::string(mash::string::from_int((i32)*nums));
                    *out += *number;
                    number->finalize(mash::ALLOCATED);

                    ++nums;
                    break;
                }

                case 'f': {
                    auto* number = new (storage) mash::string(mash::string::from_float(*nums));
                    *out += *number;
                    number->finalize(mash::ALLOCATED);

                    ++nums;
                    break;
                }

                case 't': {
                    f32 seconds = *nums++;

                    if (seconds > 0.0f) {
                        i32 whole = (i32)seconds;

                        auto* time = new (storage) mash::string(mash::string::fmt, "%02d:%02d", whole / 60, whole % 60);
                        *out += *time;
                        time->finalize(mash::ALLOCATED);
                    } else
                        *out += "--:--";

                    break;
                }

                case 'T': {
                    f32 seconds = *nums++;

                    auto* separator = new (separator_storage) mash::string(references::game.read()->string_localizer->lookup_localized_string(258));

                    mash::string* time;

                    if (seconds > 0.0f) {
                        i32 whole        = (i32)seconds;
                        i32 milliseconds = (i32)(((f64)seconds - (f32)whole) * 1000.0);

                        if (milliseconds >= 1000)
                            milliseconds = 0;

                        time = new (storage) mash::string(mash::string::fmt,
                                                          "%02d:%02d%s%002d",
                                                          whole / 60,
                                                          whole % 60,
                                                          separator->c_str(),
                                                          milliseconds);
                    } else
                        time = new (storage) mash::string(mash::string::fmt, "--:--%s---", separator->c_str());

                    *out += *time;
                    time->finalize(mash::ALLOCATED);
                    separator->finalize(mash::ALLOCATED);

                    break;
                }

                default:
                    break;
            }

            previous = mark;

            if (mark + 2 >= format->size())
                break;

            mark = format->find("%", mark + 2);
        } while (mark != mash::string::npos);

        if (previous + 2 < format->size()) {
            auto* rest = new (storage) mash::string(format->substr(previous + 2, mash::string::npos));
            *out += *rest;
            rest->finalize(mash::ALLOCATED);
        }
    }

    format->finalize(mash::ALLOCATED);

    return out;
}

// sub_85ED40
mash::string* treyarch::format_script_text(mash::string*                         out,
                                           mash::string*                         format,
                                           chuck::vm::script_dynamic_array<f32>* nums) {

    if (nums->array->size()) {
        alignas(mash::string) u8 storage[sizeof(mash::string)];

        // the elements go in as they lie, one num word after another
        format_script_text(out, new (storage) mash::string(*format), 0, (const f32*)nums->array->begin());
    } else
        new (out) mash::string(*format);

    format->finalize(mash::ALLOCATED);

    return out;
}

// sub_83EC10
bool slf__get_timer_widget__t::operator()(chuck::vm::vm_stack &stack) {
    igo_3d_timer_widget* result = references::frontend.get().igo->timer_widget;

    stack.push(&result, sizeof(result));

    return true;
}

bool slf__is_blackscreen__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83EC40)(this, stack);
}

bool slf__blackscreen_on__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E280)(this, stack);
}

bool slf__blackscreen_on_from_cutscene__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E2C0)(this, stack);
}

bool slf__blackscreen_on_from_cutscene_no_freeze__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E300)(this, stack);
}

bool slf__blackscreen_off__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E340)(this, stack);
}

bool slf__letterbox__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E380)(this, stack);
}

bool slf__add_tip__entity__str__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E3B0)(this, stack);
}

bool slf__remove_tip__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E410)(this, stack);
}

bool slf__activate_stealth_meter__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E440)(this, stack);
}

bool slf__activate_follow_meter__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E490)(this, stack);
}

bool slf__activate_distance_widget__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E4D0)(this, stack);
}

bool slf__activate_distance_widget__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_823CA0)(this, stack);
}

bool slf__set_distance_widget_min_distance__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E510)(this, stack);
}

bool slf__set_distance_widget_max_distance__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E540)(this, stack);
}

bool slf__flip_distance_widget_orientation_with_hot_cold__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E570)(this, stack);
}

bool slf__update_distance_widget__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E5B0)(this, stack);
}

bool slf__deactivate_distance_widget__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_823CC0)(this, stack);
}

bool slf__set_distance_bar_near_tint__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E5D0)(this, stack);
}

bool slf__set_distance_bar_mid_tint__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E5F0)(this, stack);
}

bool slf__set_distance_bar_far_tint__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E610)(this, stack);
}

bool slf__reset_distance_bar_tints__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_823D00)(this, stack);
}

bool slf__start_exitable_state__num__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__set_mission_text__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E630)(this, stack);
}

bool slf__set_mission_text_debug__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

// sub_85E910
bool slf__remove_map_poi__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto index = stack.local_num(0x00);

    if (index >= 0.0f)
        references::frontend.get().igo->zoom_map->remove_poi((i32)index);

    return true;
}

// sub_85E950
bool slf__add_zoom_map_poi__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto position = *(const vector3d*)&stack.local_uint(0x00);
    auto kind     = stack.local_num(0x0C);

    auto result = (f32)references::frontend.get().igo->zoom_map->add_poi(position, (i32)kind, 0);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_85E9B0
bool slf__set_zoom_map_poi_shown__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto index = stack.local_num(0x00);
    auto shown = stack.local_num(0x04);

    references::frontend.get().igo->zoom_map->set_poi_shown((i32)index, shown != 0.0f);

    return true;
}

// sub_85E9F0
bool slf__add_zoom_map_safe_zone_marker__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto position = *(const vector3d*)&stack.local_uint(0x00);

    auto result = (f32)references::frontend.get().igo->zoom_map->add_safe_zone_marker(position);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_85EA40
bool slf__add_zoom_map_safe_zone_marker__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    arch_base_vhandle tracked(stack.local_uint(0x00));

    auto result = (f32)references::frontend.get().igo->zoom_map->add_safe_zone_marker(tracked);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_85E910
bool slf__remove_zoom_map_safe_zone_marker__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto index = stack.local_num(0x00);

    if (index >= 0.0f)
        references::frontend.get().igo->zoom_map->remove_poi((i32)index);

    return true;
}

// sub_85E9B0
bool slf__set_zoom_map_safe_zone_marker_shown__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto index = stack.local_num(0x00);
    auto shown = stack.local_num(0x04);

    references::frontend.get().igo->zoom_map->set_poi_shown((i32)index, shown != 0.0f);

    return true;
}

// sub_85EA80
bool slf__set_zoom_map_safe_zone_marker_state__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto index = stack.local_num(0x00);
    auto state = stack.local_num(0x04);

    references::frontend.get().igo->zoom_map->set_marker_state((i32)index, (i32)state);

    return true;
}

// sub_85EAB0
bool slf__enable_mini_map__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto enabled = stack.local_num(0x00);

    references::frontend.get().igo->radar_widget->set_shown(enabled != 0.0f);

    return true;
}

// sub_85EAF0
bool slf__disable_zoom_map__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    bool disable = stack.local_num(0x00) != 0.0f;

    igo_3d_zoom_map* zoom_map = references::frontend.get().igo->zoom_map;

    zoom_map->disabled          = disable;
    zoom_map->movement_disabled = disable;

    return true;
}

// sub_85EB40
bool slf__bring_up_zoom_map__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    bool through_script = (f64)stack.local_num(0x00) > 0.0001;

    igo_3d_zoom_map* zoom_map = references::frontend.get().igo->zoom_map;

    if (!zoom_map->is_open)
        ((void (__thiscall*)(igo_3d_zoom_map*))retail::sub_5B4A70)(zoom_map); // igo_3d_zoom_map select toggle, empty

    zoom_map->brought_up_through_script = through_script;

    return true;
}

// sub_823D20
bool slf__kill_zoom_map__t::operator()(chuck::vm::vm_stack &stack) {
    igo_3d_zoom_map* zoom_map = references::frontend.get().igo->zoom_map;

    if (zoom_map->is_open)
        ((void (__thiscall*)(igo_3d_zoom_map*))retail::sub_5B4A70)(zoom_map); // igo_3d_zoom_map select toggle, empty

    zoom_map->brought_up_through_script = false;

    return true;
}

// sub_83EC80
bool slf__is_zoom_map_up__t::operator()(chuck::vm::vm_stack &stack) {
    f32 result = references::frontend.get().igo->zoom_map->is_open_or_zooming() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_85EB90
bool slf__snap_to_zoom_map_point__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto x = stack.local_num(0x00);
    auto z = stack.local_num(0x04);

    references::frontend.get().igo->zoom_map->snap_to_point(x, z);

    return true;
}

// sub_85EBC0
bool slf__set_zoom_map_help_text__num__t::operator()(chuck::vm::vm_stack &stack) {
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

    references::frontend.get().igo->zoom_map->set_help_text(result->c_str());

    result->finalize(mash::ALLOCATED);
    format->finalize(mash::ALLOCATED);

    return true;
}

// sub_85ECC0
bool slf__feature_current_zoom_poi__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto feature = stack.local_num(0x00);

    references::frontend.get().igo->zoom_map->feature_current_poi(feature != 0.0f);

    return true;
}

// sub_83ECD0
bool slf__is_a_zoom_poi_featured__t::operator()(chuck::vm::vm_stack &stack) {
    f32 result = references::frontend.get().igo->zoom_map->get_selected_poi() != -1 ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_85ED00
bool slf__disable_zoom_map_movement__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto disable = stack.local_num(0x00);

    references::frontend.get().igo->zoom_map->movement_disabled = disable != 0.0f;

    return true;
}

bool slf__bring_up_dialog_box_debug__str__num__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85EDF0)(this, stack);
}

bool slf__begin_movie__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85EE20)(this, stack);
}

bool slf__offscreen_indicator_add__entity__num__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85EE40)(this, stack);
}

// sub_85EE60
bool slf__set_zoom_map_poi_meta_title__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto index   = stack.local_num(0x00);
    auto text_id = stack.local_num(0x04);

    alignas(mash::string) u8 title_storage[sizeof(mash::string)];
    alignas(mash::string) u8 storage[sizeof(mash::string)];

    const char* text = references::game.read()->string_localizer->lookup_scripttext_string((i32)text_id);

    auto* title = new (title_storage) mash::string(text);

    references::frontend.get().igo->zoom_map->set_poi_meta_title((i32)index, new (storage) mash::string(*title));

    title->finalize(mash::ALLOCATED);

    return true;
}

// sub_85EF00
bool slf__set_zoom_map_poi_meta_text__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    // the count word covers the index, the text id and the format's nums
    i32 count = (i32)stack.pop_num();

    stack.move_sp(-4 * count);

    auto index   = stack.local_num(0x00);
    auto text_id = stack.local_num(0x04);

    alignas(mash::string) u8 format_storage[sizeof(mash::string)];
    alignas(mash::string) u8 result_storage[sizeof(mash::string)];
    alignas(mash::string) u8 storage[sizeof(mash::string)];
    alignas(mash::string) u8 text_storage[sizeof(mash::string)];

    const char* text = references::game.read()->string_localizer->lookup_scripttext_string((i32)text_id);

    auto* format = new (format_storage) mash::string(text);

    mash::string* result = format_script_text((mash::string*)result_storage,
                                              new (storage) mash::string(*format),
                                              0,
                                              &stack.local_num(0x08));

    references::frontend.get().igo->zoom_map->set_poi_meta_text((i32)index, new (text_storage) mash::string(*result));

    result->finalize(mash::ALLOCATED);
    format->finalize(mash::ALLOCATED);

    return true;
}

bool slf__slide_in_ally_health_widget__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824100)(this, stack);
}

bool slf__slide_out_ally_health_widget__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824120)(this, stack);
}

bool slf__set_ally_health_icon__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F010)(this, stack);
}

bool slf__set_ally_health_meter_value__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F040)(this, stack);
}

bool slf__set_ally_health_icon__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F070)(this, stack);
}

bool slf__clear_ally_health_widget__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83ED30)(this, stack);
}

bool slf__slide_in_boss_health_widget__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824140)(this, stack);
}

bool slf__slide_out_boss_health_widget__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824170)(this, stack);
}

bool slf__set_boss_health_icon__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F0B0)(this, stack);
}

bool slf__set_boss_health_icon__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F0E0)(this, stack);
}

bool slf__clear_boss_health_widget__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83ED60)(this, stack);
}

bool slf__disable_grunt_widget__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8241A0)(this, stack);
}

bool slf__enable_grunt_widget__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8241C0)(this, stack);
}

bool slf__enable_summon_power_mode__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F120)(this, stack);
}

// sub_85DD30
bool slf__timer_widget__is_valid__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (igo_3d_timer_widget*)stack.local_uint(0x00);

    ui_frontend* igo = references::frontend.get().igo;

    f32 result = igo && self == igo->timer_widget ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_85DD80
bool slf__timer_widget__get_time_modifier__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (igo_3d_timer_widget*)stack.local_uint(0x00);

    f32 result = self ? self->time_modifier : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_85DDD0
bool slf__timer_widget__set_time_modifier__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self     = (igo_3d_timer_widget*)stack.local_uint(0x00);
    auto modifier = stack.local_num(0x04);

    if (self)
        self->time_modifier = modifier;

    return true;
}

// sub_85DDF0
bool slf__timer_widget__get_time_left__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (igo_3d_timer_widget*)stack.local_uint(0x00);

    f32 result = self ? self->time_left : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_85DE40
bool slf__timer_widget__is_timer_halted__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (igo_3d_timer_widget*)stack.local_uint(0x00);

    f32 result = self && self->timer_too_high ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_85DEA0
bool slf__timer_widget__set_time_left__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (igo_3d_timer_widget*)stack.local_uint(0x00);
    auto left = stack.local_num(0x04);

    if (self)
        self->set_time_left(left);

    return true;
}

// sub_85DEC0
bool slf__timer_widget__freeze__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (igo_3d_timer_widget*)stack.local_uint(0x00);

    if (self)
        self->running = false;

    return true;
}

// sub_85DEE0
bool slf__timer_widget__run__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (igo_3d_timer_widget*)stack.local_uint(0x00);

    if (self)
        self->running = true;

    return true;
}

// sub_85DF00
bool slf__timer_widget__show_timer__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (igo_3d_timer_widget*)stack.local_uint(0x00);

    if (self)
        self->set_shown(true);

    return true;
}

// sub_85DF20
bool slf__timer_widget__show_timer__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self        = (igo_3d_timer_widget*)stack.local_uint(0x00);
    auto text_frozen = stack.local_num(0x04);

    if (self)
        self->show(true, false, (i32)text_frozen);

    return true;
}

// sub_85DF50
bool slf__timer_widget__show_timer_corner__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (igo_3d_timer_widget*)stack.local_uint(0x00);

    if (self)
        self->set_shown_shifted(true, true);

    return true;
}

// sub_85DF80
bool slf__timer_widget__hide_timer__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (igo_3d_timer_widget*)stack.local_uint(0x00);

    if (self)
        self->set_shown(false);

    return true;
}

// sub_85DFA0
bool slf__timer_widget__inc_time_left__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (igo_3d_timer_widget*)stack.local_uint(0x00);
    auto delta = stack.local_num(0x04);

    if (self)
        self->inc_time_left(delta);

    return true;
}

// sub_85DFC0
bool slf__timer_widget__count_up__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (igo_3d_timer_widget*)stack.local_uint(0x00);

    if (self)
        self->count_down = false;

    return true;
}

// sub_85DFE0
bool slf__timer_widget__count_down__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (igo_3d_timer_widget*)stack.local_uint(0x00);

    if (self)
        self->count_down = true;

    return true;
}

// sub_85E000
bool slf__timer_widget__turn_green__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (igo_3d_timer_widget*)stack.local_uint(0x00);

    if (self)
        ((void (__thiscall*)(igo_3d_timer_widget*, color32, f32))retail::sub_AD2D40)(self, color32(60, 180, 60, 255), 1.0f); // igo_3d_timer_widget::anim_color, empty

    return true;
}

// sub_85E040
bool slf__timer_widget__turn_green__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self      = (igo_3d_timer_widget*)stack.local_uint(0x00);
    auto anim_time = stack.local_num(0x04);

    if (self)
        ((void (__thiscall*)(igo_3d_timer_widget*, color32, f32))retail::sub_AD2D40)(self, color32(60, 180, 60, 255), anim_time); // igo_3d_timer_widget::anim_color, empty

    return true;
}

// sub_85E080
bool slf__timer_widget__turn_yellow__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (igo_3d_timer_widget*)stack.local_uint(0x00);

    if (self)
        ((void (__thiscall*)(igo_3d_timer_widget*, color32, f32))retail::sub_AD2D40)(self, color32(230, 208, 63, 255), 1.0f); // igo_3d_timer_widget::anim_color, empty

    return true;
}

// sub_85E0C0
bool slf__timer_widget__turn_yellow__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self      = (igo_3d_timer_widget*)stack.local_uint(0x00);
    auto anim_time = stack.local_num(0x04);

    if (self)
        ((void (__thiscall*)(igo_3d_timer_widget*, color32, f32))retail::sub_AD2D40)(self, color32(230, 208, 63, 255), anim_time); // igo_3d_timer_widget::anim_color, empty

    return true;
}

// sub_85E100
bool slf__timer_widget__turn_red__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (igo_3d_timer_widget*)stack.local_uint(0x00);

    if (self)
        ((void (__thiscall*)(igo_3d_timer_widget*, color32, f32))retail::sub_AD2D40)(self, color32(160, 48, 48, 255), 1.0f); // igo_3d_timer_widget::anim_color, empty

    return true;
}

// sub_85E140
bool slf__timer_widget__turn_red__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self      = (igo_3d_timer_widget*)stack.local_uint(0x00);
    auto anim_time = stack.local_num(0x04);

    if (self)
        ((void (__thiscall*)(igo_3d_timer_widget*, color32, f32))retail::sub_AD2D40)(self, color32(160, 48, 48, 255), anim_time); // igo_3d_timer_widget::anim_color, empty

    return true;
}

// sub_85E180
bool slf__timer_widget__turn_flashing__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (igo_3d_timer_widget*)stack.local_uint(0x00);

    if (self)
        ((void (__thiscall*)(igo_3d_timer_widget*))retail::sub_5B4A70)(self); // igo_3d_timer_widget::turn_flashing, empty

    return true;
}

// sub_85E1A0
bool slf__timer_widget__run_when_hidden__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self       = (igo_3d_timer_widget*)stack.local_uint(0x00);
    auto run_hidden = stack.local_num(0x04);

    if (self)
        self->set_run_when_hidden(run_hidden != 0.0f);

    return true;
}

// sub_85E1E0
bool slf__timer_widget__display_hundredths__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self    = (igo_3d_timer_widget*)stack.local_uint(0x00);
    auto display = stack.local_num(0x04);

    if (self)
        self->set_display_hundredths(display != 0.0f);

    return true;
}

// sub_85E220
bool slf__timer_widget__display_thousandths__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self    = (igo_3d_timer_widget*)stack.local_uint(0x00);
    auto display = stack.local_num(0x04);

    if (self)
        self->set_display_thousandths(display != 0.0f);

    return true;
}

// sub_85E260
bool slf__timer_widget__set_z__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (igo_3d_timer_widget*)stack.local_uint(0x00);
    auto z    = stack.local_num(0x04);

    if (self)
        igo_3d_timer_widget::set_z(z);

    return true;
}
