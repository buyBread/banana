#include "retail.hh"
#include "game/frontend/dialog_box.hh"

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

// +0x10 slot 2
void dialog_box::update(f32 time_inc) {
    ((void (__thiscall*)(igo_3d_widget*, f32))retail::sub_6D0310)((igo_3d_widget*)this, time_inc);
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

// slot 24
void dialog_box::show(bool freeze_hero) {
    ((void (__thiscall*)(dialog_box*, bool))retail::sub_6A0D30)(this, freeze_hero);
}

// slot 25
void dialog_box::hide() {
    ((void (__thiscall*)(dialog_box*))retail::sub_68C210)(this);
}
