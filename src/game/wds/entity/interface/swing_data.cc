#include <new>

#include "game/wds/entity/interface/swing_data.hh"
#include "game/math/references.hh"
#include "game/polytube.hh"
#include "game/wds/ai/ai_tentacle_engine.hh"
#include "game/wds/entity/entity_base.hh"
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
