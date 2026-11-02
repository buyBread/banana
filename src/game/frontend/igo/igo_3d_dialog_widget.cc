#include <new>

#include "retail.hh"
#include "game/app.hh"
#include "game/event/event_manager.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_dialog_widget.hh"
#include "game/frontend/igo/igo_3d_loading_screen.hh"
#include "game/frontend/igo/igo_3d_text.hh"
#include "game/frontend/message_text.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/game.hh"
#include "game/input/input_device.hh"
#include "game/input/input_mgr.hh"
#include "game/mission/mission_progression_data.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/references.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/math/trig.hh"

using namespace treyarch;

// sub_6F9C50
igo_3d_dialog_widget::~igo_3d_dialog_widget() {
    if (objectives_title) {
        objectives_title->destroy();
        objectives_title = nullptr;
    }

    if (objectives_subtitle) {
        objectives_subtitle->destroy();
        objectives_subtitle = nullptr;
    }

    for (u32 index = 0; index < lines.size(); ++index) {
        if (lines[index]) {
            lines[index]->destroy();
            lines[index] = nullptr;
        }
    }

    lines.clear();
    line_points.clear();
    line_scores.clear();
    line_not_required.clear();

    if (button_1) {
        delete button_1;
        button_1 = nullptr;
    }

    if (button_2) {
        delete button_2;
        button_2 = nullptr;
    }

    if (nav_button_1) {
        delete nav_button_1;
        nav_button_1 = nullptr;
    }

    if (nav_button_2) {
        delete nav_button_2;
        nav_button_2 = nullptr;
    }

    if (objectives_text) {
        objectives_text->destroy();
        objectives_text = nullptr;
    }

    release_entity();
}

// slot 1
void igo_3d_dialog_widget::init() {
    ((void (__thiscall*)(igo_3d_dialog_widget*))retail::sub_68E590)(this);
}

// slot 2
void igo_3d_dialog_widget::update(f32 time_inc) {
    ((void (__thiscall*)(igo_3d_dialog_widget*, f32))retail::sub_6D0AB0)(this, time_inc);
}

// slot 3
void igo_3d_dialog_widget::render() {
    ((void (__thiscall*)(igo_3d_dialog_widget*))retail::sub_68E8F0)(this);
}

// slot 42
void igo_3d_dialog_widget::spawn_entity() {
    ((void (__thiscall*)(igo_3d_dialog_widget*))retail::sub_6FEAC0)(this);
}

// sub_6A20D0
void igo_3d_dialog_widget::render_congomerate_mesh(ngl::mesh*              mesh,
                                                   i32                     index,
                                                   const matrix4x4        &trans_matrix,
                                                   const matrix4x4        &rel_pos_matrix,
                                                   ngl::mesh_params*       mesh_params,
                                                   ngl::shader_param_set*  shader_params) {

    for (u32 hidden = 0; hidden < hidden_members.size(); ++hidden) {
        if (hidden_members[hidden] == index)
            return;
    }

    simple_text_nav_button* button = nullptr;

    if (button_1 && (index == button_1->member_a || index == button_1->member_b))
        button = button_1;
    else if (button_2 && (index == button_2->member_a || index == button_2->member_b))
        button = button_2;

    if (button && (!button->shown || !button->selected))
        return;

    igo_3d_widget::render_congomerate_mesh(mesh, index, trans_matrix, rel_pos_matrix, mesh_params, shader_params);
}

// slot 46
void igo_3d_dialog_widget::render_text(igo_3d_text*            text,
                                       const matrix4x4        &trans_matrix,
                                       const matrix4x4        &rel_pos_matrix,
                                       ngl::mesh_params*       mesh_params,
                                       ngl::shader_param_set*  shader_params) {

    ((void (__thiscall*)(igo_3d_dialog_widget*, igo_3d_text*, const matrix4x4&, const matrix4x4&, ngl::mesh_params*, ngl::shader_param_set*))retail::sub_6B97E0)(this,
                                                                                                                                                                 text,
                                                                                                                                                                 trans_matrix,
                                                                                                                                                                 rel_pos_matrix,
                                                                                                                                                                 mesh_params,
                                                                                                                                                                 shader_params);
}

// +0x150 slot 3
void igo_3d_dialog_widget::unk_003(i32 unk) {
    ((void (__thiscall*)(igo_generic_input_listener*, i32))retail::sub_6ECDF0)((igo_generic_input_listener*)this, unk);
}

// +0x150 slot 4
void igo_3d_dialog_widget::unk_004(i32 unk) {
    ((void (__thiscall*)(igo_generic_input_listener*, i32))retail::sub_6ECEC0)((igo_generic_input_listener*)this, unk);
}

// sub_68E680
void igo_3d_dialog_widget::unk_005(i32) {
    if (!takes_input() || bring_up_mode != 1)
        return;

    if (select_button(1)) {
        string_hash button_down;
        button_down.initialize(mash::ALLOCATED, "ui_button_down");

        retail::sub_900D60(button_down.source_hash_code, 0x10000000, 0); // post the sound event on the global game object
    }
}

// sub_68E5B0
void igo_3d_dialog_widget::unk_006(i32) {
    if (!takes_input() || bring_up_mode != 1)
        return;

    if (select_button(2)) {
        string_hash button_up;
        button_up.initialize(mash::ALLOCATED, "ui_button_up");

        retail::sub_900D60(button_up.source_hash_code, 0x10000000, 0); // post the sound event on the global game object
    }
}

// sub_68E750
void igo_3d_dialog_widget::unk_007(i32) {
    if (!takes_input())
        return;

    if (button_1_shown) {
        string_hash button_ok;
        button_ok.initialize(mash::ALLOCATED, "ui_button_ok");

        retail::sub_900D60(button_ok.source_hash_code, 0x10000000, 0); // post the sound event on the global game object
    }

    result = 1;

    close();
}

// sub_68E820
void igo_3d_dialog_widget::unk_010(i32) {
    if (!takes_input() || !button_2_shown)
        return;

    string_hash button_cancel;
    button_cancel.initialize(mash::ALLOCATED, "ui_button_cancel");

    retail::sub_900D60(button_cancel.source_hash_code, 0x10000000, 0); // post the sound event on the global game object

    result = 0;

    close();
}

// sub_6B9F30
void igo_3d_dialog_widget::set_title_by_id(u32 text_id) {
    if (objectives_title)
        objectives_title->set_text(text_id);
}

// sub_68EAF0
void igo_3d_dialog_widget::set_title(mash::string title) {
    if (objectives_title)
        objectives_title->set_text_no_localize(title);
}

// sub_6BA110
void igo_3d_dialog_widget::set_text_by_id(u32 text_id) {
    if (objectives_subtitle)
        objectives_subtitle->set_text(text_id);
}

// sub_6B9F80
void igo_3d_dialog_widget::set_text(mash::string text) {
    if (!objectives_subtitle)
        return;

    input_mgr* inputs = input_mgr::inst();

    if (inputs) {
        input_device* device = inputs->devices[joystick_2_device];

        if (!device)
            device = inputs->devices[joystick_1_device];

        if (device && device->unk_034())
            message_text::substitute_pad_tokens(&text);
    }

    objectives_subtitle->set_text_no_localize(text);
}

// sub_6F7120
void igo_3d_dialog_widget::clear() {
    button_1_shown     = false;
    button_2_shown     = false;
    selected_button    = -1;
    unk_1c0            = -1;
    first_visible_line = -1;

    for (u32 index = 0; index < lines.size(); ++index) {
        if (lines[index]) {
            lines[index]->destroy();
            lines[index] = nullptr;
        }
    }

    lines.clear();
    line_points.clear();
    line_scores.clear();
    line_not_required.clear();

    if (objectives_title)
        objectives_title->set_text_no_localize("");

    unk_224 = 0.0f;
    unk_22c = 0;
}

// sub_6A22F0
void igo_3d_dialog_widget::add_objective(mission_objective_data* objective, bool indented) {
    if (!objective ||
        !objective->is_flagged(mission_progression_flag_activated) ||
        !objective->is_flagged(mission_progression_flag_displayable))

        return;

    f32 target = objective->target_score;
    f32 actual = unk_22c ? 0.0f : objective->actual_score;

    alignas(mash::string) u8 description_storage[sizeof(mash::string)];
    alignas(mash::string) u8 line_storage       [sizeof(mash::string)];

    auto* description = new (description_storage) mash::string(objective->short_description);

    bool not_required = !objective->is_flagged(mission_progression_flag_required);

    auto* line = new (line_storage) mash::string();

    *line = indented ? "  -" : "";

    auto append_score = [line](f32 score) {
        alignas(mash::string) u8 storage[sizeof(mash::string)];

        auto* number = new (storage) mash::string((i32)score);

        *line += *number;

        number->finalize(mash::ALLOCATED);
    };

    if (target != 0.0f) {
        *line += "(";

        if (actual != 0.0f) {
            append_score(actual);

            *line += "/";
        }

        append_score(target);

        *line += ") ";
    }

    *line += *description;

    f32 points = objective->is_flagged(mission_progression_flag_failed) ? 0.0f : objective->points;

    add_mission_objective(*line, (i32)points, actual, target, not_required);

    line->finalize(mash::ALLOCATED);
    description->finalize(mash::ALLOCATED);
}

// slot 60
void igo_3d_dialog_widget::add_mission_objective(mash::string line, i32 points, f32 actual_score, f32 target_score, bool not_required) {
    ((void (__thiscall*)(igo_3d_dialog_widget*, mash::string, i32, f32, f32, bool))retail::sub_711E00)(this,
                                                                                                       line,
                                                                                                       points,
                                                                                                       actual_score,
                                                                                                       target_score,
                                                                                                       not_required);
}

// sub_68EB60
void igo_3d_dialog_widget::add_text_line(mash::string line) {
    add_mission_objective(line, 0, 0.0f, 0.0f, true);
}

// sub_68EBF0
void igo_3d_dialog_widget::set_button_texts(mash::string first, mash::string second) { // a lone text lands on button 2's label whichever it is
    if (second.empty()) {
        if (first.empty()) {
            set_button_text(mash::string(""), true);
            set_button_text(mash::string(""), false);

            button_1_shown = false;
            button_2_shown = false;
        } else {
            set_button_text(first, false);
            set_button_text(mash::string(""), true);

            button_1_shown = true;
            button_2_shown = false;
        }
    } else if (first.empty()) {
        set_button_text(second, false);
        set_button_text(mash::string(""), true);

        button_1_shown = false;
        button_2_shown = true;
    } else {
        set_button_text(second, false);
        set_button_text(first, true);

        button_1_shown = true;
        button_2_shown = true;
    }
}

// sub_6A21C0
void igo_3d_dialog_widget::set_button_text(mash::string text, bool first) {
    simple_text_nav_button* button = first ? button_1 : button_2;

    if (button) {
        alignas(mash::string) u8 storage[sizeof(mash::string)];

        button->set_text(new (storage) mash::string(text));
    }
}

// sub_6DE490
void igo_3d_dialog_widget::bring_up(i32 mode) {
    if (is_dialog_shown())
        return;

    references::frontend.get().igo->loading_screen->blackscreen_off(0.0f, false);

    event_manager::raise_event(references::dialog_3d_entered.read(), arch_base_vhandle());

    game_was_paused = references::game.read()->game_paused;

    unk_071(true);
    set_shown(true);

    bring_up_mode = mode;

    if (!game_was_paused)
        retail::sub_97C970((i32)references::game.read()); // game::pause_action

    unk_078();

    set_first_visible_line(0);

    set_lines_shown(false);

    select_button(button_2_shown && !button_1_shown ? 2 : 1);

    if (button_1)
        button_1->set_shown(button_1_shown);

    if (button_2)
        button_2->set_shown(button_2_shown);

    fade_in(0.5f, 0.0f, 1.0f, 0.0f);

    unk_1e8.start(0.0f, 1.0f, 0.5f, 0.0f);
    unk_204.start(0.0f, 1.0f, 0.25f, 0.0f);

    retail::sub_6D8720((i32)references::frontend.get().igo, 1); // starts the dialog background fade

    string_hash objectives_on;
    objectives_on.initialize(mash::ALLOCATED, "ui_objectives_on");

    retail::sub_900D60(objectives_on.source_hash_code, 0x10000000, 0); // post the sound event on the global game object

    faded_in = true;
}

// sub_6DE8F0
void igo_3d_dialog_widget::close() {
    if (!is_dialog_shown())
        return;

    unk_071(false);

    fade_out(0.25f, 1.0f, 0.0f, 0.25f);

    unk_1e8.start(0.0f, 1.0f, 0.5f, 0.0f);

    retail::sub_6D8800((i32)references::frontend.get().igo, 1); // ends the dialog background fade

    event_manager::raise_event(references::dialog_3d_exited.read(), arch_base_vhandle());

    string_hash objectives_off;
    objectives_off.initialize(mash::ALLOCATED, "ui_objectives_off");

    retail::sub_900D60(objectives_off.source_hash_code, 0x10000000, 0); // post the sound event on the global game object
}

// slot 66
bool igo_3d_dialog_widget::unk_066() {
    return ((bool (__thiscall*)(igo_3d_dialog_widget*))retail::sub_6F9FB0)(this);
}

// slot 67
bool igo_3d_dialog_widget::unk_067() {
    return ((bool (__thiscall*)(igo_3d_dialog_widget*))retail::sub_6F9FC0)(this);
}

// slot 68
bool igo_3d_dialog_widget::unk_068() {
    return ((bool (__thiscall*)(igo_3d_dialog_widget*))retail::sub_68E9B0)(this);
}

// slot 69
u8 igo_3d_dialog_widget::is_dialog_shown() {
    return ((u8 (__thiscall*)(igo_3d_dialog_widget*))retail::sub_68E980)(this);
}

// slot 70
bool igo_3d_dialog_widget::unk_070() {
    return ((bool (__thiscall*)(igo_3d_dialog_widget*))retail::sub_68E990)(this);
}

// slot 71
bool igo_3d_dialog_widget::unk_071(bool unk) {
    return ((bool (__thiscall*)(igo_3d_dialog_widget*, bool))retail::sub_68E9A0)(this, unk);
}

// slot 72
i32 igo_3d_dialog_widget::get_selected_button() {
    return ((i32 (__thiscall*)(igo_3d_dialog_widget*))retail::sub_68E9E0)(this);
}

// slot 73
i32 igo_3d_dialog_widget::unk_073() {
    return ((i32 (__thiscall*)(igo_3d_dialog_widget*))retail::sub_68EAC0)(this);
}

// slot 74
i32 igo_3d_dialog_widget::unk_074() {
    return ((i32 (__thiscall*)(igo_3d_dialog_widget*))retail::sub_68EA10)(this);
}

// sub_6A2170
void igo_3d_dialog_widget::set_selected_button(i32 button) {
    selected_button = button - 1;

    i32 count = (i32)lines.size();

    if (button - 1 < 0)
        selected_button = 0;

    if (selected_button >= count)
        selected_button = count - 1;
}

// sub_68EA20
bool igo_3d_dialog_widget::select_button(i32 button) {
    i32 index = button - 1;

    if (button_1_shown && index == 0) {
        if (selected_button == 0)
            return false;
    } else if (!button_2_shown || index != 1 || selected_button == 1)
        return false;

    selected_button = index;

    if (button_1) {
        if (index == 0)
            button_1->nav_select(0.0f);
        else
            button_1->nav_deselect(0.0f);
    }

    if (button_2) {
        if (selected_button == 0)
            button_2->nav_deselect(0.0f);
        else
            button_2->nav_select(0.0f);
    }

    return true;
}

// slot 77
void igo_3d_dialog_widget::unk_077(i32 unk) {
    ((void (__thiscall*)(igo_3d_dialog_widget*, i32))retail::sub_68EAD0)(this, unk);
}

// slot 78
void igo_3d_dialog_widget::unk_078() {
    ((void (__thiscall*)(igo_3d_dialog_widget*))retail::sub_6A75A0)(this);
}

// sub_6A2260
void igo_3d_dialog_widget::set_button_1_text(u32 text_id) {
    if (button_1 && button_1->text)
        button_1->text->set_text_by_id(text_id);
}

// sub_6A2280
void igo_3d_dialog_widget::set_button_2_text(u32 text_id) {
    if (button_2 && button_2->text)
        button_2->text->set_text_by_id(text_id);
}

// sub_68E920
void igo_3d_dialog_widget::show_button_1(bool shown) {
    button_1_shown = shown;
    unk_15e        = true;

    nav_button_1->set_shown(shown);
}

// sub_68E950
void igo_3d_dialog_widget::show_button_2(bool shown) {
    button_2_shown = shown;
    unk_15e        = true;

    nav_button_2->set_shown(shown);
}

// sub_6A22A0
void igo_3d_dialog_widget::set_lines_shown(bool shown) {
    for (u32 index = 0; index < lines.size(); ++index) {
        if (lines[index])
            lines[index]->set_shown(shown);
    }
}

// sub_6BA160
void igo_3d_dialog_widget::show_visible_lines() {
    for (i32 index = 0; (u32)index < lines.size(); ++index) {
        if (!lines[index])
            continue;

        if (index >= 0 &&
            index < (i32)lines.size() &&
            index >= first_visible_line &&
            index < first_visible_line + 5)

            lines[index]->set_shown(true);
        else
            lines[index]->set_shown(false);
    }
}

// sub_6D0DB0
// inlined @ sub_6DE490
bool igo_3d_dialog_widget::set_first_visible_line(i32 index) {
    if (index < 0)
        index = 0;
    else if ((u32)index >= lines.size())
        index = lines.size() - 1;

    bool changed = index != first_visible_line;

    first_visible_line = index;

    if (changed)
        show_visible_lines();

    return changed;
}

// inlined @ sub_68E5B0
// inlined @ sub_68E680
// inlined @ sub_68E750
// inlined @ sub_68E820
bool igo_3d_dialog_widget::takes_input() {
    return is_dialog_shown() && unk_070() && unk_068();
}

// sub_6A49F0
void igo_3d_dialog_widget::spawn_reward_orbs(i32 experience) {
    if (experience <= 0)
        return;

    i32 count     = experience / 20;
    i32 remainder = experience % 20;

    if (remainder > 0)
        ++count;

    f32      step   = (f32)((f64)(f32)(360.0 / (f64)(f32)count) * 0.017453293005625408);
    vector3d center = references::g_world_ptr.read()->get_hero_ptr()->my_abs_po->get_position();

    for (i32 orb = 0; orb < count; ++orb) {
        f32 angle  = (f32)((f64)(f32)orb * (f64)step);
        f32 sine   = math::cos((f32)(4.71238899230957 + (f64)angle)); // three quarters of a turn on
        f32 cosine = math::cos(angle);

        f32 x_offset = (f32)((f64)sine * 4.0);
        f32 z_offset = (f32)((f64)cosine * 4.0);

        entity* hero = references::g_world_ptr.read()->get_hero_ptr();

        if (!hero)
            continue;

        vector3d position((f32)((f64)center.x + (f64)x_offset),
                          (f32)((f64)center.y + 0.0),
                          (f32)((f64)center.z + (f64)z_offset));

        f32 amount = orb == 0 && remainder > 0 ? (f32)remainder : 20.0f;

        retail::sub_673200(hero->my_ifc_storage->get_ifc(entity_ifc_rpg), amount, 3, (i32)&position, nullptr); // rpg_interface: spawns the orb
    }
}
