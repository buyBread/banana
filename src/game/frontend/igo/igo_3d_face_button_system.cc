#include <intrin.h>
#include <new>

#include "retail.hh"
#include "game/frontend/igo/igo_3d_face_button_system.hh"
#include "treyarch/mm/mm.hh"

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

// sub_70C7E0
i32 igo_3d_face_button_system::show_button(u32 kind, f32 unk_0, f32 unk_1) {
    if (kind > 11)
        return -1;

    void* allocation = mm::allocate(sizeof(igo_3d_face_button_widget));

    igo_3d_face_button_widget* button = nullptr;

    if (allocation)
        button = (igo_3d_face_button_widget*)retail::sub_704300((u32*)allocation, references::button_widget_keys.get()[kind].source_hash_code);

    if (!button)
        return -1;

    i32 id = _InterlockedIncrement((volatile long*)&last_button_id);

    button->init();

    button->kind = kind;
    button->id   = id;

    f32 x;
    f32 y;

    default_button->get_position(&x, &y);

    button->set_center_pos(x, y);
    button->set_button_pos(x, y);

    button->set_scale(default_button->get_scale());
    button->set_tilt_angle(default_button->get_tilt_angle());
    button->set_spin_angle(default_button->get_spin_angle());
    button->set_rotation_angle(default_button->get_rotation_angle());
    button->set_widescreen_alignment(default_button->get_widescreen_alignment());

    {
        engine_lock_scope scope(&lock);

        buttons.push_back(button);
    }

    button->unk_054(unk_0, unk_1);

    return id;
}

// sub_6D06E0
void igo_3d_face_button_system::play_button_anim(i32 id, u32 anim, bool sets_arrow, i32 unk) {
    igo_3d_face_button_widget* button = find_button(id);

    if (!button || button->kind > 11 || anim > 22)
        return;

    const tl_fixed_string &name = references::button_anim_names.get()[button->kind][anim];

    if (name != tl_fixed_string("")) {
        button->kill_layer_0_anims();
        button->play_anim(name);

        if (sets_arrow) {
            u32 arrow = (u32)retail::sub_68DC90(anim); // the anim's arrow, 4 for none

            if (arrow <= 3) {
                button->unk_059(references::button_arrow_keys.get()[arrow], unk);
                button->unk_063(anim);
            }
        }
    }
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
