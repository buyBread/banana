#include "treyarch/game/wds/entity/interface/time_interface.hh"
#include "treyarch/game/wds/references.hh"

using namespace treyarch;

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

// sub_636A80
void time_interface::frame_advance_all_time_interfaces(f32 t) {
    dinkumware::vector<time_interface*>* interfaces = references::all_time_interfaces.read();

    if (!interfaces)
        return;

    for (time_interface* ifc : *interfaces)
        ifc->frame_advance(t);
}
