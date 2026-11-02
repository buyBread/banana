#include "retail.hh"
#include "game/frontend/igo/igo_3d_text.hh"
#include "game/frontend/igo/simple_text_nav_button.hh"

using namespace treyarch;

// slot 1
void simple_nav_button::nav_select(f32 unk) {
    ((void (__thiscall*)(simple_nav_button*, f32))retail::sub_68AEA0)(this, unk);
}

// sub_68AE50
void simple_nav_button::update(f32 time_inc) {
    if (selected)
        selected_time = (f32)((f64)time_inc + (f64)selected_time);

    if ((f64)selected_time >= (f64)selected_time_limit)
        selected = false;
}

// sub_68F720
simple_text_nav_button::~simple_text_nav_button() {
    if (text) {
        text->destroy();
        text = nullptr;
    }
}

// sub_68F850
void simple_text_nav_button::nav_select(f32) {
    if (text)
        text->set_unk_18(false);

    unk_1c   = 0.0f;
    unk_20   = 0;
    selected = true;
}

// sub_68F7F0
void simple_text_nav_button::update(f32) {
    if (!text)
        return;

    if (shown) {
        if (!text->is_shown())
            text->set_shown(true);
    } else if (text->is_shown())
        text->set_shown(false);
}

// sub_68F880
void simple_text_nav_button::nav_deselect(f32) {
    if (text)
        text->set_unk_18(true);

    unk_1c   = 0.0f;
    unk_20   = 3;
    selected = false;
}

// slot 4
void simple_text_nav_button::set_shown(bool shown) {
    ((void (__thiscall*)(simple_text_nav_button*, bool))retail::sub_68F8B0)(this, shown);
}

// sub_68F780
void simple_text_nav_button::set_text(mash::string* value) {
    if (text)
        text->set_text_no_localize(*value);

    value->finalize(mash::ALLOCATED);
}
