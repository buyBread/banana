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

// sub_90D120
void swing_params::set_sweet_spot_distance(f32 dist) {
    sweet_spot_distance = dist;
}

// sub_548DF0
void swing_params::set_sweet_cone_angle_degrees(f32 angle) {
    sweet_cone_angle_cos = (f32)std::cos((f64)(f32)(angle * 0.01745329300562541));
}

// sub_548E20
void swing_params::set_max_swing_web_length(f32 len) {
    max_web_length_squared = (f32)((f64)len * (f64)len);
}

// sub_548E40
void swing_params::set_min_swing_web_length(f32 len) {
    min_web_length_squared = (f32)((f64)len * (f64)len);
}

// sub_5FF550
void swing_params::set_min_dynamic_sweet_length(f32 len) {
    min_dynamic_sweet_length = len;
}

// sub_548E60
void swing_params::set_max_dynamic_sweet_length(f32 len) {
    max_dynamic_sweet_length = len;
}

// sub_548E80
void swing_params::set_overdrive_sweet_length(f32 len) {
    max_high_speed_sweet_length = len;
}

// sub_548E70
void swing_params::set_max_no_anchor_sweet_length(f32 len) {
    max_no_anchor_sweet_length = len;
}

// sub_454FD0
void swing_params::set_swing_ground_clearance(f32 len) {
    ground_clearance = len;
}

// sub_4921D0
void swing_params::set_overdrive_velocity(f32 vel) {
    sweet_spot_overdrive_velocity = vel;
}

// sub_548E90
void swing_params::set_min_dynamic_sweet_angle(f32 angle) {
    min_dynamic_sweet_angle = angle;
}

// sub_548EA0
void swing_params::set_max_dynamic_sweet_angle(f32 angle) {
    max_dynamic_sweet_angle = angle;
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
