#include "game/frontend/igo/igo_3d_boss_meter_system.hh"

using namespace treyarch;

// sub_6D0990
void igo_3d_boss_meter_system::draw() {
    for (auto* position = meters.begin(); position != meters.end(); position = position->next) {
        if (position->value)
            position->value->render();
    }
}

// sub_6DE440
void igo_3d_boss_meter_system::remove_all_meters() {
    for (auto* position = meters.begin(); position != meters.end();) {
        igo_3d_boss_meter_widget* meter = position->value;

        position = meters.erase(position);

        if (meter)
            meter->destroy();
    }
}

// sub_6DE3D0
void igo_3d_boss_meter_system::remove_meter(i32 id) {
    for (auto* position = meters.begin(); position != meters.end();) {
        igo_3d_boss_meter_widget* meter = position->value;

        if (meter && meter->id == id) {
            position = meters.erase(position);
            meter->destroy();
        } else
            position = position->next;
    }

    position_meters();
}

// sub_6D0A10
void igo_3d_boss_meter_system::position_meters() {
    f32 offset = 0.0f;

    for (auto* position = meters.begin(); position != meters.end(); position = position->next) {
        igo_3d_boss_meter_widget* meter = position->value;

        if (!meter || meter->unk_164)
            continue;

        f32 y = default_meter->get_position_y() + offset;
        f32 x = default_meter->get_position_x();

        meter->set_center_pos(x, y);

        offset = (f32)((f64)offset + 60.0);
    }
}

// sub_6DE390
f32 igo_3d_boss_meter_system::get_health(i32 id) {
    for (auto* position = meters.begin(); position != meters.end(); position = position->next) {
        igo_3d_boss_meter_widget* meter = position->value;

        if (meter && meter->id == id)
            return meter->health;
    }

    return -1.0f;
}

// sub_6D09C0
void igo_3d_boss_meter_system::set_health(i32 id, f32 health) {
    for (auto* position = meters.begin(); position != meters.end(); position = position->next) {
        igo_3d_boss_meter_widget* meter = position->value;

        if (!meter || meter->id != id)
            continue;

        if (meter->unk_165)
            meter->health = health;

        return;
    }
}
