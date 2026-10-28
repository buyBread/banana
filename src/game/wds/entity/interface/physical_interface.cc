#include "retail.hh"
#include "game/wds/entity/interface/physical_interface.hh"
#include "game/physics/physics_system.hh"
#include "game/physics/rigid_body_prop_system.hh"

using namespace treyarch;

// sub_6007A0
void physical_interface::enable(bool val) {
    set_flag(physical_interface_flag_enabled, val);
}

// sub_5FFD60
void physical_interface::set_gravity(bool val) {
    if (is_gravity() == val)
        return;

    set_flag(physical_interface_flag_gravity, val);

    gravity_delay_timer = 0.0f;
}

void physical_interface::set_velocity(const vector3d &v) {
    retail::sub_6006A0((u32**)this, (i32)&v);
}

vector3d physical_interface::get_velocity() const {
    vector3d result;
    retail::sub_60F130((i32)this, (i32)&result);

    return result;
}

void physical_interface::add_velocity_increment(const vector3d &v, bool allow_limbo) {
    retail::sub_659810((i32)this, (f32*)&v, allow_limbo);
}

void physical_interface::add_velocity_increment_at_pos(const vector3d &v, const vector3d &pos, f32 torque_mult, bool allow_limbo) {
    retail::sub_659A60((i32)this, (f32*)&v, (i32)&pos, torque_mult, allow_limbo);
}

void physical_interface::cancel_all_velocity(const vector3d &dir, bool both_dir) {
    retail::sub_6253F0((i32)this, (f32*)&dir, both_dir);
}

void physical_interface::stop_biped_physics(bool on_destroyed) {
    retail::sub_67A440((i32)this, on_destroyed);
}

// sub_600680
biped_system* physical_interface::get_biped_system() const {
    if (is_flagged(physical_interface_flag_biped_running))
        return m_bp_info;

    return nullptr;
}

void physical_interface::set_biped_physics_is_weapon(bool is_weapon, f32 unk_1, f32 unk_2) {
    retail::sub_67A290((u32*)this, is_weapon, unk_1, unk_2);
}

void physical_interface::start_prop_physics(const vector3d &force,
                                            f32             random,
                                            bool            lightweight,
                                            bool            kill_when_stable,
                                            bool            traverse_children,
                                            bool            in_collision) {

    retail::sub_65A400((i32)this, (i32)&force, random, lightweight, kill_when_stable, traverse_children, in_collision);
}

void physical_interface::stop_prop_physics(bool on_destroyed) {
    retail::sub_63F150((i32)this, on_destroyed);
}

// sub_6005E0
bool physical_interface::is_prop_physics_running() const {
    return m_rbph != nullptr;
}

// sub_6005F0
bool physical_interface::is_prop_physics_stable(bool full_stable) const {
    if (!m_rbph)
        return true;

    if (!full_stable && low_vel_time > 1.0f)
        return true;

    return prop_system::is_stable(m_rbph);
}

void physical_interface::set_auto_prop_physics(bool i) {
    retail::sub_60D580((u32*)this, i);
}

// sub_6008C0
void physical_interface::set_physics_ballistic_target(const vector3d &pos) {
    if (m_rbph) {
        rigid_body* body = m_rbph->m_first_rbpi ? m_rbph->m_first_rbpi->m_rb : nullptr;

        vector4d target(pos.x, pos.y, pos.z, 0.0f);
        f32      dist_sq;

        nuge::apply_ballistic_target(&body, 1, target, &dist_sq);

        return;
    }

    if (is_flagged(physical_interface_flag_biped_running)) {
        vector4d target(pos.x, pos.y, pos.z, 0.0f);

        retail::sub_9F0B40((i32)m_bp_info, (u64*)&target); // biped_system
    }
}
