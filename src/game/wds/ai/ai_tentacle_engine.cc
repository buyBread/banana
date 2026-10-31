#include <cmath>
#include <cstdlib>

#include "retail.hh"
#include "game/dangler.hh"
#include "game/math/references.hh"
#include "game/math/usefulmath.hh"
#include "game/wds/ai/ai_tentacle_engine.hh"
#include "game/wds/ai/ai_tentacle_info.hh"
#include "game/wds/entity/entity.hh"

using namespace treyarch;

// sub_4B9DB0
ai_tentacle_engine::ai_tentacle_engine(ai_tentacle_info* t) {
    vtable = (void**)&references::ai_tentacle_engine_vtable.get();

    simple_list_vars.initialize();

    {
        engine_lock_scope lock(&references::ai_tentacle_engine_id_lock.get());

        // 0 is never an id
        id = ++references::ai_tentacle_engine_id_counter.get();

        if (!id)
            id = ++references::ai_tentacle_engine_id_counter.get();
    }

    modifier = nullptr;
    tentacle = t;
}

// sub_4B9E20
ai_tentacle_strike::ai_tentacle_strike(ai_tentacle_info* t) : ai_tentacle_engine(t) {
    vtable = (void**)&references::ai_tentacle_strike_vtable.get();
}

// sub_491CC0
void ai_tentacle_strike::setup(const vector3d &_target, f32 _speed, i32 _max_pull) {
    target   = _target;
    speed    = _speed;
    max_pull = _max_pull;
}

// sub_4B9E90
ai_tentacle_swirl_strike::ai_tentacle_swirl_strike(ai_tentacle_info* t) : ai_tentacle_engine(t) {
    vtable = (void**)&references::ai_tentacle_swirl_strike_vtable.get();
}

// sub_491CF0
void ai_tentacle_swirl_strike::setup(const vector3d &_target, f32 _speed, f32 _accel, f32 _curve_rad) {
    target    = _target;
    speed     = _speed;
    accel     = _accel;
    curve_rad = _curve_rad;
    line_dist = 0.0f;
}

// sub_491DA0
void ai_tentacle_dangle_old::set_gravity(const vector3d &grav) {
    if (tentacle_dangler)
        tentacle_dangler->gravity = grav;
}

// sub_491E20
vector3d ai_tentacle_dangle_old::get_gravity() const {
    if (tentacle_dangler)
        return tentacle_dangler->gravity;

    return references::unk_00fc4a10.get();
}

// sub_491E70
void ai_tentacle_dangle_old::set_air_resistance(f32 coefficient) {
    if (tentacle_dangler)
        tentacle_dangler->air_resistance = coefficient;
}

// sub_491DC0
void ai_tentacle_dangle_old::set_elasticity(f32 coefficient) {
    if (tentacle_dangler)
        tentacle_dangler->elasticity = coefficient;
}

// sub_4B9ED0
ai_tentacle_dangle::ai_tentacle_dangle(ai_tentacle_info* t) : ai_tentacle_engine(t) {
    vtable = (void**)&references::ai_tentacle_dangle_vtable.get();

    tentacle_dangler = nullptr;
    update_end       = true;
    update_base      = false;
    copy_back        = true;
    unk_020          = 0.0f;

    const f32 low  = 0.0f;
    const f32 high = 2.0f * PI;

    unk_024 = (f32)((f64)(f32)std::rand() * (f64)(1.0f / 32767.0f) * ((f64)high - low) + low);
    unk_028 = std::rand() % 2 != 0;
}

// sub_4BC580
void ai_tentacle_follow::setup(f32                _slack_percent,
                               f32                _drag,
                               const vector3d    &grav,
                               arch_base_vhandle  _attach_ent,
                               bool               allow_coll,
                               bool               _update_end) {

    retail::sub_4A0520((i32)this, -1.0f, (i32)&references::unk_00fc4a10.get(), _update_end, true); // ai_tentacle_dangle_old::setup

    slack_percent = _slack_percent;

    if (0.0f > _slack_percent)
        slack_percent = 0.0f;

    drag = std::fabs(_drag);

    // the particle past the tentacle's last position
    tentacle_dangler->particles->at(tentacle->positions.size + 1).unk_34 = true;

    tentacle_dangler->air_resistance   = 5.0f;
    tentacle_dangler->elasticity       = 1000.0f;
    tentacle_dangler->gravity          = grav;
    tentacle_dangler->allow_collisions = allow_coll;
    tentacle_dangler->air_resistance   = drag;

    set_attach_ent(_attach_ent);
}

// sub_491EF0
void ai_tentacle_follow::set_attach_ent(arch_base_vhandle _attach_ent) { // the tentacle's end becomes the attach point
    attach_ent       = _attach_ent;
    local_attach_end = tentacle->end_pos;

    auto* attached = (entity*)attach_ent.resolve();

    if (!attached)
        return;

    const vector3d point = local_attach_end;

    po inverse;
    retail::sub_48E2C0((f32*)attached->my_abs_po, (u64*)&inverse); // po::inverse

    const matrix4x4 &m = inverse.get_matrix();

    local_attach_end = vector3d((f32)((f64)m.y.x * point.y + (f64)m.x.x * point.x + (f64)m.z.x * point.z + m.w.x),
                                (f32)((f64)m.y.y * point.y + (f64)m.x.y * point.x + (f64)m.z.y * point.z + m.w.y),
                                (f32)((f64)m.y.z * point.y + (f64)m.x.z * point.x + (f64)m.z.z * point.z + m.w.z));
}

// sub_492060
void ai_tentacle_follow::set_attach_ent(arch_base_vhandle _attach_ent, const vector3d &local_end) {
    attach_ent       = _attach_ent;
    local_attach_end = local_end;
}

// sub_492080
void ai_tentacle_follow::set_drag(f32 a) {
    drag = a;

    if (tentacle_dangler)
        tentacle_dangler->air_resistance = a;
}

// sub_4B9FA0
ai_tentacle_slack::ai_tentacle_slack(ai_tentacle_info* t) : ai_tentacle_follow(t) {
    vtable = (void**)&references::ai_tentacle_slack_vtable.get();
}

// sub_4BC690
void ai_tentacle_slack::setup(f32                _rope_len,
                              f32                _drag,
                              const vector3d    &gravity,
                              arch_base_vhandle  _attach_ent,
                              bool               allow_coll,
                              bool               _update_end) {

    rope_length = _rope_len;

    ai_tentacle_follow::setup(1.0f, _drag, gravity, _attach_ent, allow_coll, _update_end);
}
