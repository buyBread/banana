#include <cmath>

#include "game/wds/ai/states/plr_loco_swing_params.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/interface/player_interface.hh"

using namespace treyarch;

// sub_574560
void swing_params::frame_advance(actor* my_actor, f32 delta_t) {
    player_interface* plr_ifc = my_actor->player_ifc();

    if (plr_ifc->mode.current == 3) {
        speed = plr_ifc->get_als_lateral_speed();
        update_swing_angles(my_actor);
    } else {
        main_angle      = 270.0f;
        side_angle      = 0.0f;
        to_anchor_angle = 0.0f;
    }
}

// sub_54E900
void swing_params::update_swing_angles(actor* my_actor) {
    player_interface* plr_ifc = my_actor->player_ifc();

    main_angle      = (f32)(plr_ifc->compute_swing_angle() * 57.29577791868205);
    side_angle      = (f32)(plr_ifc->compute_swing_side_angle() * 57.29577791868205);
    to_anchor_angle = (f32)(plr_ifc->compute_swing_to_anchor_angle() * 57.29577791868205);
}

// sub_548DB0
void swing_params::set_sweet_spot_incline_angle_degrees(f32 angle) {
    f32 radians = (f32)(angle * 0.01745329300562541);

    sweet_spot_incline_angle_cos = (f32)std::cos((f64)radians);
    sweet_spot_incline_angle_sin = (f32)std::sin((f64)radians);
}

// sub_548DF0
void swing_params::set_sweet_cone_angle_degrees(f32 angle) {
    sweet_cone_angle_cos = (f32)std::cos((f64)(f32)(angle * 0.01745329300562541));
}

// sub_548EA0
void swing_params::set_max_dynamic_sweet_angle(f32 angle) {
    max_dynamic_sweet_angle = angle;
}

// sub_454FD0
void swing_params::set_swing_ground_clearance(f32 len) {
    ground_clearance = len;
}

// sub_548EB0
void swing_params::set_min_reel_in_rate(f32 rate) {
    min_reel_in_rate = rate;
}

// sub_548EC0
void swing_params::set_max_reel_in_rate(f32 rate) {
    max_reel_in_rate = rate;
}

// sub_548ED0
void swing_params::set_unk_78(f32 value) {
    unk_78 = value;
}

// sub_548EE0
void swing_params::set_unk_7c(f32 value) {
    unk_7c = value;
}

// sub_548EF0
void swing_params::set_unk_80(f32 value) {
    unk_80 = value;
}
