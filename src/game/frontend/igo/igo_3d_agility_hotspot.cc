#include "retail.hh"
#include "game/frontend/igo/igo_3d_agility_hotspot.hh"

using namespace treyarch;

// sub_692140
void igo_3d_agility_hotspot::update(f32 time_inc) {
    hit_elapsed = hit ? (f32)((f64)time_inc + (f64)hit_elapsed) : 0.0f;

    if (hit_elapsed > 0.2f)
        hit_elapsed = 0.2f;
}

// sub_6C62A0
void igo_3d_agility_hotspot::spawn_entity() {
    igo_3d_widget::spawn_entity();

    string_hash member;

    member.initialize(mash::ALLOCATED, "ui_TOA_drop_glow_1");
    glow_1_member = get_mesh_index(member);

    member.initialize(mash::ALLOCATED, "ui_TOA_drop_glow_2");
    glow_2_member = get_mesh_index(member);

    member.initialize(mash::ALLOCATED, "ui_TOA_drop_glow_web");
    glow_web_member = get_mesh_index(member);

    member.initialize(mash::ALLOCATED, "ui_TOA_drop_rotator");
    rotator_member = get_mesh_index(member);
}

// slot 44
void igo_3d_agility_hotspot::render_congomerate_mesh(ngl::mesh*              mesh,
                                                     i32                     index,
                                                     const matrix4x4        &trans_matrix,
                                                     const matrix4x4        &rel_pos_matrix,
                                                     ngl::mesh_params*       mesh_params,
                                                     ngl::shader_param_set*  shader_params) {

    ((void (__thiscall*)(igo_3d_agility_hotspot*,
                         ngl::mesh*,
                         i32,
                         const matrix4x4&,
                         const matrix4x4&,
                         ngl::mesh_params*,
                         ngl::shader_param_set*))retail::sub_6C6440)(this,
                                                                     mesh,
                                                                     index,
                                                                     trans_matrix,
                                                                     rel_pos_matrix,
                                                                     mesh_params,
                                                                     shader_params);
}
