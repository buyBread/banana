#include "retail.hh"
#include "treyarch/game/input/game_button.hh"

using namespace treyarch;

game_button::game_button(i32 control) {
    retail::sub_698490((char*)this, control);
}

void game_button::update(f32 t, bool unk_1, bool unk_2) {
    ((void (__thiscall*)(game_button*, f32, bool, bool))retail::sub_74A4E0)(this, t, unk_1, unk_2);
}
