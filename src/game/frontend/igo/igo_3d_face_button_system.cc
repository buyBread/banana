#include <new>

#include "game/frontend/igo/igo_3d_face_button_system.hh"

using namespace treyarch;

// sub_68DCF0
string_hash igo_3d_face_button_system::get_button_widget_key(u32 kind) {
    if (kind <= 11)
        return references::button_widget_keys.get()[kind];

    string_hash result;

    result.initialize(mash::ALLOCATED, "");

    return result;
}

// sub_68DD30
mash::string* igo_3d_face_button_system::get_button_anim_name(mash::string* result, u32 kind, u32 index) {
    if (kind > 11 || index > 22)
        return new (result) mash::string("");

    return new (result) mash::string(references::button_anim_names.get()[kind][index].text);
}

// sub_6B8820
bool igo_3d_face_button_system::has_button(i32 id) {
    return find_button(id) != nullptr;
}

// sub_6DE2E0
void igo_3d_face_button_system::remove_all_buttons() {
    engine_lock_scope scope(&lock);

    for (auto* position = buttons.begin(); position != buttons.end();) {
        igo_3d_face_button_widget* button = position->value;

        position = buttons.erase(position);

        if (button)
            delete button;
    }
}

// sub_6DE220
void igo_3d_face_button_system::remove_button(i32 id) {
    engine_lock_scope scope(&lock);

    for (auto* position = buttons.begin(); position != buttons.end();) {
        igo_3d_face_button_widget* button = position->value;

        if (button && button->id == id) {
            position = buttons.erase(position);
            delete button;
        } else
            position = position->next;
    }
}

// sub_6B8860
bool igo_3d_face_button_system::is_button_anim_playing(i32 id) {
    igo_3d_face_button_widget* button = find_button(id);

    if (!button)
        return false;

    return button->is_anim_playing();
}

// sub_6D07B0
void igo_3d_face_button_system::set_button_pos(i32 id, f32 x, f32 y) {
    igo_3d_face_button_widget* button = find_button(id);

    if (button)
        button->set_button_pos(x, y);
}

// sub_6D0810
void igo_3d_face_button_system::set_button_pos(i32 id, vector3d pos) {
    igo_3d_face_button_widget* button = find_button(id);

    if (!button)
        return;

    button->set_button_pos(pos.x, pos.y);
    button->set_position_z(pos.z);
}

// sub_6D08D0
void igo_3d_face_button_system::set_button_scale(i32 id, f32 scale) {
    igo_3d_face_button_widget* button = find_button(id);

    if (button)
        button->set_scale(scale);
}

// sub_6D0910
void igo_3d_face_button_system::set_button_alpha(i32 id, f32 alpha) {
    igo_3d_face_button_widget* button = find_button(id);

    if (button)
        button->set_alpha(alpha);
}

// sub_6B8B40
void igo_3d_face_button_system::show_x_over_button(i32 id, const bool &shown) {
    igo_3d_face_button_widget* button = find_button(id);

    if (!button)
        return;

    if (shown) {
        if (!button->has_x_widget())
            button->init_x_widget();
    } else
        button->destroy_x_widget();
}

// sub_6B8BA0
bool igo_3d_face_button_system::is_x_over_button_shown(i32 id) {
    igo_3d_face_button_widget* button = find_button(id);

    return button && button->has_x_widget() && button->is_x_widget_shown();
}
