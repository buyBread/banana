#include "retail.hh"
#include "game/wds/ai/ai_vehicle.hh"
#include "game/wds/entity/vehicle.hh"
#include "game/mutex.hh"
#include "treyarch/jq/jq.hh"

using namespace treyarch;

// sub_5870D0
void ai_vehicle_state::set_steer_angle(f32 angle) {
    if (-1.0 > angle)
        angle = -1.0f;

    if (angle > 1.0)
        angle = 1.0f;

    my_steer_angle = angle;
}

// sub_5881D0
void ai_vehicle_state::set_gas_pedal(f32 pedal) {
    if (0.0 > pedal)
        pedal = 0.0f;

    if (pedal > 1.0)
        pedal = 1.0f;

    if (pedal != 0.0f && 0.1f > pedal)
        pedal = 0.0f;

    my_gas_pedal = pedal;
}

// sub_587120
void ai_vehicle_state::set_brake_pedal(f32 pedal) {
    if (0.0 > pedal)
        pedal = 0.0f;

    if (pedal > 1.0)
        pedal = 1.0f;

    if (pedal != 0.0f && 0.1f > pedal)
        pedal = 0.0f;

    my_brake_pedal = pedal;
}

// sub_585630
void ai_vehicle::set_flags(u32 flags) {
    m_inode->my_flags = flags;
}

void ai_vehicle::set_purpose(e_ai_vehicle_purpose purpose) {
    ((void (__thiscall*)(ai_vehicle*, e_ai_vehicle_purpose))retail::sub_5A6BF0)(this, purpose);
}

void ai_vehicle::set_purpose(e_ai_vehicle_purpose purpose, arch_base_vhandle pe) {
    ((void (__thiscall*)(ai_vehicle*, e_ai_vehicle_purpose, arch_base_vhandle))retail::sub_5A6C80)(this, purpose, pe);
}

bool ai_vehicle::set_purpose(e_ai_vehicle_purpose purpose, vector3d pos, f32 radius) {
    return ((bool (__thiscall*)(ai_vehicle*, e_ai_vehicle_purpose, vector3d, f32))retail::sub_5A6D00)(this, purpose, pos, radius);
}

void ai_vehicle::set_purpose(e_ai_vehicle_purpose          purpose,
                             const vc8::vector<vector3d>  &positions,
                             const vc8::vector<f32>       &radii) {

    ((void (__thiscall*)(ai_vehicle*, e_ai_vehicle_purpose, const vc8::vector<vector3d>*, const vc8::vector<f32>*))retail::sub_5A6DC0)(this, purpose, &positions, &radii);
}

// sub_585640
void ai_vehicle::set_sleeps_on_stop(bool sleeps) {
    engine_lock_scope lock(&jq::references::unk_01126d10.get());

    sleeps_on_stop = sleeps;

    m_inode->my_vehicle->enable_physics(!sleeps || m_inode->get_purpose() != ai_vehicle_purpose_stop);
}
