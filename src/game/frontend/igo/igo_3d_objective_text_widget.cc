#include "retail.hh"
#include "game/frontend/igo/igo_3d_objective_text_widget.hh"

using namespace treyarch;

// sub_6F88B0
igo_3d_objective_text_widget::~igo_3d_objective_text_widget() {
    queue.clear();

    if (shown_message) {
        shown_message->destroy();
        shown_message = nullptr;
    }

    if (text) {
        text->destroy();
        text = nullptr;
    }
}

// slot 1
void igo_3d_objective_text_widget::init() {
    ((void (__thiscall*)(igo_3d_objective_text_widget*))retail::sub_70B3C0)(this);
}

// slot 2
void igo_3d_objective_text_widget::update(f32 time_inc) {
    ((void (__thiscall*)(igo_3d_objective_text_widget*, f32))retail::sub_6F6530)(this, time_inc);
}

// slot 3
void igo_3d_objective_text_widget::render() {
    ((void (__thiscall*)(igo_3d_objective_text_widget*))retail::sub_68B480)(this);
}

// sub_6E7BB0
void igo_3d_objective_text_widget::spawn_entity() {
    igo_3d_widget::spawn_entity();

    unattached_texts.push_back(text);

    string_hash member;

    member.initialize(mash::ALLOCATED, "ui_hud_obj_body_overlay_2");
    overlay_2_member = get_mesh_index(member);

    member.initialize(mash::ALLOCATED, "ui_hud_obj_body_overlay_3");
    overlay_3_member = get_mesh_index(member);

    member.initialize(mash::ALLOCATED, "ui_hud_obj_body_overlay_4");
    overlay_4_member = get_mesh_index(member);
}

// sub_68B4C0
void igo_3d_objective_text_widget::render_congomerate_mesh(ngl::mesh*              mesh,
                                                           i32                     index,
                                                           const matrix4x4        &trans_matrix,
                                                           const matrix4x4        &rel_pos_matrix,
                                                           ngl::mesh_params*       mesh_params,
                                                           ngl::shader_param_set*  shader_params) {

    if ((index == overlay_2_member ||
         index == overlay_3_member ||
         index == overlay_4_member) &&
        get_alpha() != 1.0f)

        return;

    igo_3d_widget::render_congomerate_mesh(mesh, index, trans_matrix, rel_pos_matrix, mesh_params, shader_params);
}

// slot 54
void igo_3d_objective_text_widget::suspend(bool on) {
    ((void (__thiscall*)(igo_3d_objective_text_widget*, bool))retail::sub_68B450)(this, on);
}

// slot 55
void igo_3d_objective_text_widget::show_message(mash::string text, f32 duration) {
    ((void (__thiscall*)(igo_3d_objective_text_widget*, mash::string, f32))retail::sub_6E7820)(this, text, duration);
}

// slot 56
void igo_3d_objective_text_widget::show_message_by_id(u32 text_id, f32 duration) {
    ((void (__thiscall*)(igo_3d_objective_text_widget*, u32, f32))retail::sub_6B2A70)(this, text_id, duration);
}

// slot 57
void igo_3d_objective_text_widget::unk_057(message_text*) {}

// slot 58
bool igo_3d_objective_text_widget::unk_058() {
    return ((bool (__thiscall*)(igo_3d_objective_text_widget*))retail::sub_6F8960)(this);
}

// slot 1
void igo_3d_hint_text_widget::init() {
    ((void (__thiscall*)(igo_3d_hint_text_widget*))retail::sub_70B480)(this);
}

// slot 2
void igo_3d_hint_text_widget::update(f32 time_inc) {
    ((void (__thiscall*)(igo_3d_hint_text_widget*, f32))retail::sub_6F8970)(this, time_inc);
}

// slot 59
void igo_3d_hint_text_widget::unk_059(mash::string text, i32 unk, f32 duration) {
    ((void (__thiscall*)(igo_3d_hint_text_widget*, mash::string, i32, f32))retail::sub_68B520)(this, text, unk, duration);
}

// slot 60
void igo_3d_hint_text_widget::unk_060(u32 text_id, i32 unk, f32 duration) {
    ((void (__thiscall*)(igo_3d_hint_text_widget*, u32, i32, f32))retail::sub_6B2BE0)(this, text_id, unk, duration);
}

// sub_6E7AC0
void igo_3d_objective_text_widget::clear(bool fade) {
    message_text* message = shown_message;

    if (message) {
        if (!fade) {
            message->destroy();

            shown_message = nullptr;

            text->set_text_no_localize("");
        } else if (message->end_time < 0.0f || message->fade_out_start < 0.0f) {
            message->fade_out_start = message->time;
            message->end_time       = (f32)((f64)message->time + 0.5);
        } else {
            bool fading = message->time > message->fade_out_start && message->end_time > message->time;

            if (!fading && message->fade_out_start > message->time)
                message->time = message->fade_out_start;
        }
    }

    queue.clear();
}
