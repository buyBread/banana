#include "retail.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_camera_widget.hh"
#include "game/frontend/igo/igo_3d_script_widget.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/wds/entity/conglomerate.hh"
#include "treyarch/mm/mm.hh"

using namespace treyarch;

// sub_701110
igo_3d_script_widget::igo_3d_script_widget(string_hash widget_key, string_hash text_key) : igo_3d_widget(widget_key),
                                                                                           continuous_rotation(0.0f),
                                                                                           continuous_tilt(0.0f),
                                                                                           continuous_spin(0.0f),
                                                                                           is_child(false),
                                                                                           max_timer(-1.0f),
                                                                                           timer(0.0f),
                                                                                           text_parameters_key(text_key),
                                                                                           sound_played(false),
                                                                                           persistent(false),
                                                                                           instance_id(0) {

    texts.clear();
}

// sub_6F86E0
igo_3d_script_widget::~igo_3d_script_widget() {
    for (auto* position = texts.begin(); position != texts.end();) {
        igo_3d_text* text = position->value;

        position = texts.erase(position);

        if (text)
            text->destroy();
    }
}

// sub_68AEC0
void igo_3d_script_widget::init() {
    instance_id = ++instance_counter;
}

// slot 2
void igo_3d_script_widget::update(f32 time_inc) {
    retail::sub_6CD3E0(this, time_inc);
}

// sub_6DCF70
void igo_3d_script_widget::render_child_widget(igo_3d_widget*          child,
                                               const matrix4x4        &trans_matrix,
                                               const matrix4x4        &rel_pos_matrix,
                                               ngl::mesh_params*       mesh_params,
                                               ngl::shader_param_set*  shader_params) {

    ui_frontend* frontend = references::frontend.get().igo;

    if (child == frontend->camera_widget || frontend->is_script_widget_valid((igo_3d_script_widget*)child))
        igo_3d_widget::render_child_widget(child, trans_matrix, rel_pos_matrix, mesh_params, shader_params);
    else
        erase_member_widget(child);
}

// slot 54
void igo_3d_script_widget::unk_054(f32 time_inc) {
    retail::sub_6B22D0((i32)this, time_inc);
}

// sub_68AEE0
void igo_3d_script_widget::set_continuous_rotation(f32 rate) {
    continuous_rotation = rate;
}

// sub_68AF00
void igo_3d_script_widget::set_continuous_tilt(f32 rate) {
    continuous_tilt = rate;
}

// sub_68C940
void igo_3d_script_widget::set_continuous_spin(f32 rate) {
    continuous_spin = rate;
}

// sub_6F6500
void igo_3d_script_widget::attach_child_widget(igo_3d_script_widget* child, i32 member) {
    child->is_child = true;

    if (member != invalid_conglom_member)
        member_widgets[member] = child;
}

// sub_6DCF50
void igo_3d_script_widget::detach_child_widget(igo_3d_script_widget* child) {
    child->is_child = false;

    erase_member_widget(child);
}

// sub_6F8790
bool igo_3d_script_widget::get_is_child() {
    return is_child;
}

// sub_6F87A0
u32 igo_3d_script_widget::get_instance_id() {
    return instance_id;
}

// sub_70B290
igo_3d_text* igo_3d_script_widget::get_text(i32 id) {
    if (id == -2)
        return nullptr;

    if (member_texts.size()) {
        auto* found = member_texts.find(id);

        if (found != member_texts.end())
            return found->value.second;
    }

    igo_3d_text* text = nullptr;

    void* memory = mm::allocate(sizeof(igo_3d_text));

    if (memory)
        text = (igo_3d_text*)retail::sub_701610(memory, text_parameters_key.source_hash_code); // igo_3d_text::igo_3d_text

    texts.push_front(text);

    if (text)
        member_texts[id] = text;

    return text;
}
