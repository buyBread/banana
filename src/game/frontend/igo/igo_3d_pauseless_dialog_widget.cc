#include "retail.hh"
#include "game/event/event_manager.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_pauseless_dialog_widget.hh"
#include "treyarch/mash/string_hash.hh"

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

// sub_6F7310
void igo_3d_pauseless_dialog_widget::clear() {
    igo_3d_dialog_widget::clear();

    shown_time = 0.0f;
    unk_240    = 0.0f;
    unk_249    = 0;
    unk_24c    = -1;
    unk_248    = 0;
}

// sub_6DEBC0
void igo_3d_pauseless_dialog_widget::close() { // as the dialog's, the event raised before the fade ends
    if (!is_dialog_shown())
        return;

    unk_071(false);

    fade_out(0.25f, 1.0f, 0.0f, 0.25f);

    unk_1e8.start(0.0f, 1.0f, 0.5f, 0.0f);

    event_manager::raise_event(references::dialog_3d_exited.read(), arch_base_vhandle());

    retail::sub_6D8800((i32)references::frontend.get().igo, 1); // ends the dialog background fade

    string_hash objectives_off;
    objectives_off.initialize(mash::ALLOCATED, "ui_objectives_off");

    retail::sub_900D60(objectives_off.source_hash_code, 0x10000000, 0); // post the sound event on the global game object
}
