#include "retail.hh"
#include "game/app.hh"
#include "game/frontend/dialog_box.hh"
#include "game/game.hh"

using namespace treyarch;

// sub_6F93C0
dialog_box::~dialog_box() {
    if (updown_button) {
        delete updown_button;
        updown_button = nullptr;
    }

    if (confirm_button) {
        delete confirm_button;
        confirm_button = nullptr;
    }
}

// slot 2
void dialog_box::unk_002(i32 unk) {
    ((void (__thiscall*)(dialog_box*, i32))retail::sub_68C230)(this, unk);
}

// slot 3
void dialog_box::unk_003(i32 unk) {
    ((void (__thiscall*)(dialog_box*, i32))retail::sub_689640)(this, unk);
}

// slot 4
void dialog_box::unk_004(i32 unk) {
    ((void (__thiscall*)(dialog_box*, i32))retail::sub_689640)(this, unk);
}

// slot 5
void dialog_box::unk_005(i32 unk) {
    ((void (__thiscall*)(dialog_box*, i32))retail::sub_689640)(this, unk);
}

// slot 6
void dialog_box::unk_006(i32 unk) {
    ((void (__thiscall*)(dialog_box*, i32))retail::sub_6A0D90)(this, unk);
}

// slot 7
void dialog_box::unk_007(i32 unk) {
    ((void (__thiscall*)(dialog_box*, i32))retail::sub_6A0DE0)(this, unk);
}

// sub_6D0310
void dialog_box::update(f32 time_inc) {
    if (references::dialog_box_move_buttons.read()) {
        if (updown_button)
            updown_button->set_center_pos(references::dialog_box_updown_button_x.read(), references::dialog_box_updown_button_y.read());

        if (confirm_button)
            confirm_button->set_center_pos(references::dialog_box_confirm_button_x.read(), references::dialog_box_confirm_button_y.read());
    }

    if (choice_changing) {
        choice_change_time = (f32)((f64)choice_change_time + (f64)time_inc);

        if ((f64)choice_change_time > (f64)0.25f)
            choice_changing = false;
    }

    shown_time = (f32)((f64)time_inc + (f64)shown_time);

    if (unfreeze_delay) {
        if (unfreeze_delay == 1) {
            for (i32 index = 0; index < hero_freezes; ++index)
                app::inst()->get_game()->freeze_hero(false);

            unfreeze_delay = 0;
            hero_freezes   = 0;
        } else
            --unfreeze_delay;
    }

    if (shown) {
        retail::sub_6B1B30((u32*)(igo_generic_input_listener*)this); // polls the inputs and dispatches a new press

        if (input_delay > 0)
            --input_delay;
    }
}

// +0x10 slot 3
void dialog_box::render() {
    ((void (__thiscall*)(igo_3d_widget*))retail::sub_68C240)((igo_3d_widget*)this);
}

// +0x10 slot 42
void dialog_box::spawn_entity() {
    ((void (__thiscall*)(igo_3d_widget*))retail::sub_6EBC40)((igo_3d_widget*)this);
}

// +0x10 slot 44
i32 dialog_box::unk_044(i32 unk_0, i32 unk_1, void* unk_2, i32 unk_3, i32 unk_4, i32 unk_5) {
    return ((i32 (__thiscall*)(igo_3d_widget*, i32, i32, void*, i32, i32, i32))retail::sub_6B4380)((igo_3d_widget*)this,
                                                                                                   unk_0,
                                                                                                   unk_1,
                                                                                                   unk_2,
                                                                                                   unk_3,
                                                                                                   unk_4,
                                                                                                   unk_5);
}

// sub_6A0D30
void dialog_box::show(bool freeze_hero) {
    shown     = true;
    confirmed = false;
    choice    = 0;
    freeze    = freeze_hero;

    if (freeze_hero) {
        app::inst()->get_game()->freeze_hero(true);
        ++hero_freezes;
    }

    input_delay = 2;
    shown_time  = 0.0f;
}

// slot 25
void dialog_box::hide() {
    ((void (__thiscall*)(dialog_box*))retail::sub_68C210)(this);
}
