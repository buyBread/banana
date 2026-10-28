#include "retail.hh"
#include "game/input/game_button.hh"

using namespace treyarch;

// sub_744E50
game_button::game_button() {
    initialize(mash::ALLOCATED);
}

game_button::game_button(i32 control) {
    retail::sub_698490((char*)this, control);
}

void game_button::initialize(mash::allocation_scope scope) {
    retail::sub_744360((char*)this, scope);
}

void game_button::finalize(mash::allocation_scope scope) {
    retail::sub_7373C0((i32)this, scope);
}

void game_button::set_control(i32 ctrl) {
    retail::sub_737430((i32)this, ctrl);
}

void game_button::update(f32 t, bool unk_1, bool unk_2) {
    ((void (__thiscall*)(game_button*, f32, bool, bool))retail::sub_74A4E0)(this, t, unk_1, unk_2);
}

void game_button::update_state(f32 t, f32 state, f32 unk_state) {
    ((void (__thiscall*)(game_button*, f32, f32, f32))retail::sub_745090)(this, t, state, unk_state);
}
