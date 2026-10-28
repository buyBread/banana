#include "retail.hh"
#include "game/wds/entity/interface/player_interface.hh"

using namespace treyarch;

// sub_783750
void player_interface::clear_buttons() { // every button but the first of each array
    for (i32 ctrl = 1; ctrl < 25; ++ctrl)
        buttons[ctrl].clear();

    for (i32 cmd = 1; cmd < 89; ++cmd)
        command_buttons[cmd].clear();
}

void player_interface::clear_controller(bool unk) {
    retail::sub_991FC0((i32*)this, unk);
}

// sub_83D9E0
void player_interface::override_button(i32 ctrl, f32 value, f32 t) {
    buttons[ctrl].update_state(t, value, value);
}

// sub_83DA10
void player_interface::override_command(i32 cmd, f32 value, f32 t) {
    command_buttons[cmd].update_state(t, value, value);
}

// sub_83DA40
void player_interface::set_button(i32 ctrl, f32 value) {
    buttons[ctrl].state  = value;
    buttons[ctrl].unk_1c = value;
}

// sub_83DA70
void player_interface::set_axis(i32 ctrl, bool ud, f32 value) {
    game_button* button = ud ? control_axis[ctrl]->axis_ud : control_axis[ctrl]->axis_lr;

    button->state  = value;
    button->unk_1c = value;
}
