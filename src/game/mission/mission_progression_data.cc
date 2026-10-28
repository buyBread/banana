#include "retail.hh"
#include "game/mission/mission_progression_data.hh"

using namespace treyarch;

// sub_733C80
void mission_progression_data::set_required(bool on) {
    if (on)
        flags |= mission_progression_flag_required;
    else
        flags &= ~mission_progression_flag_required;
}

// sub_733CA0
void mission_progression_data::set_displayable(bool on) {
    if (on)
        flags |= mission_progression_flag_displayable;
    else
        flags &= ~mission_progression_flag_displayable;
}

// sub_733CC0
void mission_progression_data::set_autoactivated(bool on) {
    if (on)
        flags |= mission_progression_flag_autoactivated;
    else
        flags &= ~mission_progression_flag_autoactivated;
}

// sub_733CE0
void mission_progression_data::set_autocompleted(bool on) {
    if (on)
        flags |= mission_progression_flag_autocompleted;
    else
        flags &= ~mission_progression_flag_autocompleted;
}

// sub_733FA0
void mission_stage_data::set_restart_data(const vector3d* position, const vector3d* facing) {
    facing_direction = *facing;
    restart_position = *position;
}

// sub_76B230
void mission_objective_data::set_actual_score(f32 score) {
    actual_score = score;

    retail::sub_768750(retail::sub_7685B0()); // mission_progression_registry::inst()->notify_state_change
}

// sub_733D00
void mission_objective_data::init_tracking_scores(f32 start, f32 target) {
    initial_score = start;
    actual_score  = start;
    target_score  = target;
}

// sub_733D20
void mission_objective_data::set_awarded_points(f32 awarded, f32 unk_b, bool unk_c) {
    points  = awarded;
    unk_038 = unk_b;
    unk_03c = unk_c;
}
