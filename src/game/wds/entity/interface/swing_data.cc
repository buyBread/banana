#include <cmath>
#include <new>

#include "retail.hh"
#include "game/wds/entity/interface/swing_data.hh"
#include "game/als/mocomp.hh"
#include "game/line_info.hh"
#include "game/local_collision.hh"
#include "game/math/references.hh"
#include "game/math/usefulmath.hh"
#include "game/polytube.hh"
#include "game/wds/ai/ai_tentacle_engine.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/conglomerate.hh"
#include "game/wds/entity/entity_base.hh"
#include "game/wds/entity/interface/spiderman_rvb_player_interface.hh"
#include "treyarch/mm/mm.hh"

using namespace treyarch;

// sub_9875D0
void swing_data::anim_started() {
    status = swinger_animating;
}

// sub_9875E0
void swing_data::fire(f32 speed, f32 accel, f32 curve_rad, bool zip) {
    if (fire_engine_id) {
        web.kill_engine(fire_engine_id);

        fire_engine_id = 0;
    }

    for_zip = zip;

    void* memory = mm::allocate(sizeof(ai_tentacle_swirl_strike));

    auto* strike = memory ? new (memory) ai_tentacle_swirl_strike(&web) : nullptr;

    strike->setup(unk_54, speed, accel, curve_rad);

    fire_engine_id = web.push_engine(strike);

    web  .set_visible(true);
    curly.set_visible(false);

    attach_pendulum.enable(!for_zip);

    base_length = -1.0f;
    status      = swinger_launching;
}

// sub_9876E0
void swing_data::hide() {
    web  .set_visible(false);
    curly.set_visible(false);

    attach_pendulum.enable(false);

    for_zip = false;
    status  = swinger_inactive;
}

// sub_993670
bool swing_data::frame_advance(player_interface* plr_ifc, f32 delta_t, i32 index) {
    if (for_zip)
        return false;

    if (!attach_pendulum.enabled) {
        if (status != swinger_animating)
            hide();

        return false;
    }

    vector3d pivot = attach_pendulum.get_pivot_abs_pos();
    actor*   owner = plr_ifc->my_actor;

    vector3d dir = pivot - owner->my_abs_po->get_position();

    dir.normalize();

    // static geometry only, stopping just short of the pivot
    line_info lif(owner->my_abs_po->get_position(), pivot - dir * 0.01f);

    bool blocked = false;
    bool stuck   = false;

    if (retail::sub_798F50((char*)&lif, (i32)references::entfilter_reject_all.read(), (i32)references::obbfilter_lineseg_test.read(), nullptr, 0)) { // line_info::check_collision
        f32 owner_distance = (owner->my_abs_po->get_position() - pivot).length_float();
        f32 hit_distance   = (lif.hit_pos - pivot).length_float();
        f32 ratio          = hit_distance / owner_distance;

        // hits near the pivot don't count
        blocked = !(hit_distance < 5.0f) && !(ratio < 0.3f);
    }

    if (!blocked)
        unk_68 = 0.0f;
    else {
        unk_68 += delta_t;

        if (unk_68 > plr_ifc->unk_416() && plr_ifc->mode.current != 9) {
            f32 dist_sq = (lif.hit_pos - owner->my_abs_po->get_position()).length2_float();

            // close enough: the web sticks where it's blocked
            if (plr_ifc->get_swing_collision_stick_length() *
                plr_ifc->get_swing_collision_stick_length() > dist_sq) {

                vector3d away = owner->my_abs_po->get_position() - lif.hit_pos;

                if (plr_ifc->unk_077() == 1) {
                    away.y = 0.0f;

                    if (away.length_float() < 0.0001f)
                        away += lif.hit_norm;
                }

                away.normalize();

                pivot = lif.hit_pos + away * 0.01f;
                stuck = true;

                attach_pendulum.set_vector3d_pivot(pivot);

                unk_54 = pivot;

                web.set_end_position(pivot);

                f32 reach  = (f32)std::sqrt((f64)dist_sq);
                f32 ground = spiderman_rvb_player_interface::compute_ground_level_at_sweet_spot_position(lif.hit_pos, reach, owner->get_primary_region());
                f32 height = lif.hit_pos.y - ground - plr_ifc->get_swing_ground_clearance();
                f32 limit  = reach > height ? height : reach;

                if (goal_constraint > limit)
                    goal_constraint = limit;

                if (plr_ifc->get_swing_collision_break_length() > goal_constraint) {
                    retail::sub_798F10((u8*)&lif); // line_info::~line_info

                    return true;
                }
            } else if (plr_ifc->get_swing_collision_break_length() *
                       plr_ifc->get_swing_collision_break_length() > dist_sq) {

                retail::sub_798F10((u8*)&lif); // line_info::~line_info

                return true;
            }
        }
    }

    f32      constraint = attach_pendulum.constraint;
    vector3d span       = pivot - owner->my_abs_po->get_position();

    // a slack rope shortens to the owner's distance
    if (!plr_ifc->unk_202() && (f64)constraint - (f64)1.0f > (f64)span.length()) {
        if (stuck)
            constraint = span.length_float();
        else {
            f32 blend = (f32)((f64)delta_t * (f64)20.0f);

            clamp_value(&blend, 0.0f, 1.0f);

            constraint = constraint + (span.length_float() - constraint) * blend;
        }

        if (plr_ifc->get_swing_collision_break_length() > constraint)
            constraint = (f32)(plr_ifc->get_swing_collision_break_length() + (f64)0.0001f);
    }

    if (constraint < 1.0f)
        constraint = 1.0f;

    if (!attach_pendulum.has_a_moving_anchor())
        attach_pendulum.set_constraint(constraint);

    f32 web_time = (f32)((f64)plr_ifc->get_time_on_web() * (f64)0.5f);

    clamp_value(&web_time, 0.0f, 1.0f);

    attach_pendulum.adjustment_factor = (f32)((f64)0.9f - (f64)web_time * (f64)0.4f);

    bool broke = constraint < 3.0f;

    retail::sub_798F10((u8*)&lif); // line_info::~line_info

    return broke;
}

// sub_993F50
void swing_data::post_process(player_interface* plr_ifc, f32 delta_t, i32 index) {
    if (!attach_pendulum.enabled && !for_zip) {
        if (status != swinger_animating)
            hide();

        return;
    }

    actor* owner = plr_ifc->my_actor;

    vector3d hand = owner->my_abs_po->get_position();

    const string_hash &hand_name = index ? als::references::bip01_prop_l_hand.get() : als::references::bip01_prop_r_hand.get();

    retail::sub_62FA60((i32)owner, ((conglomerate*)owner)->get_member_index(hand_name), (f32*)&hand, 0); // conglomerate::get_member_abs_position

    vector3d target = unk_54;

    web  .set_base_position(hand);
    curly.set_base_position(hand);

    if (fire_engine_id) {
        auto* strike = (ai_tentacle_swirl_strike*)web.get_engine(fire_engine_id);

        if (!strike) {
            fire_engine_id = 0;

            curly_engine->reset_curl();
            curly.set_visible(!for_zip);

            curly_engine->set_length(plr_ifc->mode.current == 11 ? 0.1f : 1.0f);
        } else
            strike->target = target;
    }

    if (!fire_engine_id) {
        status = swinger_attached;

        unk_2d4->set_length((unk_54 - hand).length());
        unk_2d4->set_elasticity(plr_ifc->swing_both_hand() ? 1000.0f : 1000.0f); // both hands / one hand
    }

    web  .frame_advance(delta_t);
    curly.frame_advance(delta_t);
    web  .update_spline(false);
    curly.update_spline(false);
}

// sub_994200
void swing_leftover::frame_advance(f32 delta_t) {
    if (!web.is_visible())
        return;

    if (web.tentacle->is_in_limbo()) {
        web.stop_dangle(-1.0f);
        web.set_visible(false);

        return;
    }

    if (!life_time.is_ready()) {
        life_time.frame_advance(delta_t);

        return;
    }

    if (fade_time.frame_advance(delta_t)) {
        hide();

        return;
    }

    color32 color = web.tentacle->get_render_color();

    color.set_alpha_float(1.0f - fade_time.charge_percent());

    web.tentacle->set_render_color(color);
}

// sub_987730
void swing_leftover::post_process(f32 delta_t) {
    if (!web.is_visible())
        return;

    web.set_base_position(attach_pendulum.get_pivot_abs_pos());
    web.frame_advance(delta_t);
    web.update_spline(false);
}

// sub_996980
void swing_leftover::copy(const swing_data* data) { // takes over the swinger's web where it hangs
    web.stop_dangle(-1.0f);
    web.set_visible(false);

    attach_pendulum.set_entity_base_pivot((entity_base*)data->attach_pendulum.entity_base_pivot.resolve(), data->unk_54);

    web.set_base_position(data->unk_54);
    web.create_line(data->web.get_base_position(), nullptr);

    vector3d base = web.get_base_position();

    web.start_dangle((web.get_end_position() - base).length(), references::unk_01111bf0.get());
    web.set_visible(true);

    web.tentacle->set_render_color(color32(0xFF, 0xFF, 0xFF, 0xFF));

    life_time.reset();
    fade_time.reset();
}

// sub_987710
void swing_leftover::hide() {
    web.stop_dangle(-1.0f);
    web.set_visible(false);
}
