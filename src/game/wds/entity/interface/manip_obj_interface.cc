#include "retail.hh"
#include "game/wds/entity/interface/manip_obj_interface.hh"
#include "treyarch/jq/native_locks.hh"

using namespace treyarch;

void manip_obj_interface::throw_it(entity*         thrower,
                                   const vector3d &target_pos,
                                   entity*         target,
                                   bool            use_rotation,
                                   const vector3d &rotation) {

    retail::sub_680030((i32)this, (i32*)thrower, (f32*)&target_pos, (u32*)target, use_rotation, (i32)&rotation);
}

void manip_obj_interface::throw_it_velocity(entity* thrower, const vector3d &force, const vector3d &rotation, bool unk) {
    retail::sub_680710((i32)this, (i32*)thrower, (f32*)&force, (i64*)&rotation, unk);
}

void manip_obj_interface::throw_it_with_speed(entity*         thrower,
                                              f32             speed,
                                              entity*         target,
                                              const vector3d &rotation,
                                              f32             gravity_scale,
                                              f32             max_reach) {

    retail::sub_680A90((i32)this, (i32*)thrower, speed, (u32*)target, (i32)&rotation, gravity_scale, max_reach);
}

void manip_obj_interface::disarm() {
    retail::sub_666940((i32)this);
}

arch_base_vhandle manip_obj_interface::update_for_grab(entity* grabber) {
    arch_base_vhandle result;
    retail::sub_660450((u32*)this, (u32*)&result, (i32)grabber);

    return result;
}

// sub_611790
bool manip_obj_interface::is_active() const {
    return references::active_manip_objs.get().contains((manip_obj_interface*)this);
}

// sub_642BF0
// inlined @ sub_645FD0
void manip_obj_interface::force_status_update() {
    engine_lock_scope lock(&jq::references::unk_01003498.get());

    references::unk_00ffeb18.write(1);

    retail::sub_63F8A0((u32*)this); // manip_obj_interface::remove_from_lists

    u32 inserted;
    retail::sub_6353A0((u32*)&references::update_manip_objs.get(), &inserted, (u32*)this); // simple_list::push_back

    references::unk_00ffeb18.write(0);
}
