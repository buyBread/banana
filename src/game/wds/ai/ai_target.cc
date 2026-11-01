#include "retail.hh"
#include "game/wds/ai/ai_target.hh"

using namespace treyarch;

ai_target::ai_target() {
    ((ai_target* (__thiscall*)(ai_target*))retail::sub_470C80)(this);
}

void ai_target::set_target(arch_base_vhandle _target) {
    ((void (__thiscall*)(ai_target*, arch_base_vhandle))retail::sub_455490)(this, _target);
}

f32 ai_target::compute_score(      entity_base* attacker,
                             const vector3d    &pos,
                             const vector3d    &forward_vector,
                             const vector3d    &up_vector,
                                   f32          min_range,
                                   f32          max_range,
                                   f32          min_vert_range,
                                   f32          max_vert_range,
                                   f32          min_dot,
                                   f32          min_vert_dot,
                                   u32          flags,
                                   accept_func  aux_accept,
                                   i32          unk_13,
                                   i32          unk_14,
                                   i32          unk_15) {

    return (f32)((f64 (__thiscall*)(ai_target*,
                                    entity_base*,
                                    const vector3d*,
                                    const vector3d*,
                                    const vector3d*,
                                    f32,
                                    f32,
                                    f32,
                                    f32,
                                    f32,
                                    f32,
                                    u32,
                                    accept_func,
                                    i32,
                                    i32,
                                    i32))retail::sub_4828E0)(this,
                                                             attacker,
                                                             &pos,
                                                             &forward_vector,
                                                             &up_vector,
                                                             min_range,
                                                             max_range,
                                                             min_vert_range,
                                                             max_vert_range,
                                                             min_dot,
                                                             min_vert_dot,
                                                             flags,
                                                             aux_accept,
                                                             unk_13,
                                                             unk_14,
                                                             unk_15);
}
