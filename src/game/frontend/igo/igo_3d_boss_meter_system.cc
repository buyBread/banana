#include "retail.hh"
#include "game/frontend/igo/igo_3d_boss_meter_system.hh"
#include "treyarch/mm/mm.hh"

using namespace treyarch;

// sub_6D0990
void igo_3d_boss_meter_system::draw() {
    for (auto* position = meters.begin(); position != meters.end(); position = position->next) {
        if (position->value)
            position->value->render();
    }
}

// sub_70CEC0
i32 igo_3d_boss_meter_system::add_meter(arch_base_vhandle entity, i32 type) {
    for (auto* position = meters.begin(); position != meters.end(); position = position->next) {
        igo_3d_boss_meter_widget* meter = position->value;

        if (meter && meter->unk_16c == entity)
            return meter->id;
    }

    i32 id = -1;

    if (!type) {
        void* allocation = mm::allocate(sizeof(igo_3d_boss_meter_widget));

        igo_3d_boss_meter_widget* meter = nullptr;

        if (allocation)
            meter = (igo_3d_boss_meter_widget*)retail::sub_704F80((u32*)allocation);

        if (meter) {
            id = ++last_meter_id;

            meter->init();
            meter->id = id;

            retail::sub_6A2000((char*)meter, entity.value, type); // igo_3d_boss_meter_widget::enable_boss_meter

            copy_default_placement(meter);

            meters.push_back(meter);
        }
    }

    position_meters();

    return id;
}

// sub_70D060
i32 igo_3d_boss_meter_system::add_meter(i32 type) {
    i32 id = -1;

    if (!type) {
        void* allocation = mm::allocate(sizeof(igo_3d_boss_meter_widget));

        igo_3d_boss_meter_widget* meter = nullptr;

        if (allocation)
            meter = (igo_3d_boss_meter_widget*)retail::sub_704F80((u32*)allocation);

        if (meter) {
            id = ++last_meter_id;

            meter->init();
            meter->id = id;

            retail::sub_6A2000((char*)meter, 0, type); // igo_3d_boss_meter_widget::enable_boss_meter

            copy_default_placement(meter);

            meters.push_back(meter);
        }
    }

    position_meters();

    return id;
}

// inlined @ sub_70CEC0
// inlined @ sub_70D060
void igo_3d_boss_meter_system::copy_default_placement(igo_3d_boss_meter_widget* meter) {
    f32 y = default_meter->get_position_y();
    f32 x = default_meter->get_position_x();

    meter->set_center_pos(x, y);

    meter->set_scale(default_meter->get_scale());
    meter->set_tilt_angle(default_meter->get_tilt_angle());
    meter->set_spin_angle(default_meter->get_spin_angle());
    meter->set_rotation_angle(default_meter->get_rotation_angle());
    meter->set_widescreen_alignment(default_meter->get_widescreen_alignment());
}

// sub_6DE440
void igo_3d_boss_meter_system::remove_all_meters() {
    for (auto* position = meters.begin(); position != meters.end();) {
        igo_3d_boss_meter_widget* meter = position->value;

        position = meters.erase(position);

        if (meter)
            delete meter;
    }
}

// sub_6DE3D0
void igo_3d_boss_meter_system::remove_meter(i32 id) {
    for (auto* position = meters.begin(); position != meters.end();) {
        igo_3d_boss_meter_widget* meter = position->value;

        if (meter && meter->id == id) {
            position = meters.erase(position);
            delete meter;
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
