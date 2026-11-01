#include "retail.hh"
#include "game/wds/entity/interface/time_interface.hh"
#include "game/wds/references.hh"

using namespace treyarch;

combat_dilation_info::combat_dilation_info() {
    ((combat_dilation_info* (__thiscall*)(combat_dilation_info*))retail::sub_607C70)(this);
}

// sub_5FD510
f32 time_interface::calc_time_dilation() {
    f64 dilation = is_combat_dilated() ? (f64)combat_dilation * (f64)attack_dilation : (f64)attack_dilation;

    switch (time_mode) {
        case time_mode_world: {
            f32 world_dilation = references::g_world_ptr.read()->time_mgr.get_time_dilation_factor(ignore_reflex_dilation);

            return (f32)((f64)world_dilation * dilation);
        }

        case time_mode_absolute:
            return (f32)((f64)time_dilation * dilation);

        case time_mode_relative: {
            f32 world_dilation = references::g_world_ptr.read()->time_mgr.get_time_dilation_factor(ignore_reflex_dilation);

            return (f32)((f64)time_dilation * (f64)world_dilation * dilation);
        }

        default:
            return (f32)dilation;
    }
}

void time_interface::set_combat_dilation(const combat_dilation_info &info) {
    ((void (__thiscall*)(time_interface*, const combat_dilation_info*))retail::sub_5FD400)(this, &info);
}

void time_interface::set_attack_dilation(f32 d) {
    ((void (__thiscall*)(time_interface*, f32))retail::sub_5FD960)(this, d);
}

// slot 19
void time_interface::frame_advance(f32 t) {
    ((void (__thiscall*)(time_interface*, f32))retail::sub_5FD3E0)(this, t);
}

// sub_636A80
void time_interface::frame_advance_all_time_interfaces(f32 t) {
    vc8::vector<time_interface*>* interfaces = references::all_time_interfaces.read();

    if (!interfaces)
        return;

    for (time_interface* ifc : *interfaces)
        ifc->frame_advance(t);
}
