#include "retail.hh"
#include "game/frontend/igo/igo_3d_pauseless_dialog_widget.hh"

using namespace treyarch;

// slot 2
void igo_3d_pauseless_dialog_widget::update(f32 time_inc) {
    ((void (__thiscall*)(igo_3d_pauseless_dialog_widget*, f32))retail::sub_6ED2D0)(this, time_inc);
}

// +0x150 slot 3
void igo_3d_pauseless_dialog_widget::unk_003(i32 unk) {
    ((void (__thiscall*)(igo_generic_input_listener*, i32))retail::sub_6ED580)((igo_generic_input_listener*)this, unk);
}

// +0x150 slot 4
void igo_3d_pauseless_dialog_widget::unk_004(i32 unk) {
    ((void (__thiscall*)(igo_generic_input_listener*, i32))retail::sub_6ED590)((igo_generic_input_listener*)this, unk);
}

// +0x150 slot 7
void igo_3d_pauseless_dialog_widget::unk_007(i32 unk) {
    ((void (__thiscall*)(igo_generic_input_listener*, i32))retail::sub_6DEDC0)((igo_generic_input_listener*)this, unk);
}

// +0x150 slot 10
void igo_3d_pauseless_dialog_widget::unk_010(i32 unk) {
    ((void (__thiscall*)(igo_generic_input_listener*, i32))retail::sub_689610)((igo_generic_input_listener*)this, unk);
}

// slot 58
void igo_3d_pauseless_dialog_widget::clear() {
    ((void (__thiscall*)(igo_3d_pauseless_dialog_widget*))retail::sub_6F7310)(this);
}

// slot 65
void igo_3d_pauseless_dialog_widget::close() {
    ((void (__thiscall*)(igo_3d_pauseless_dialog_widget*))retail::sub_6DEBC0)(this);
}
