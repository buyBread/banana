#include "treyarch/ngl/d3d9/scene_state.hh"
#include "treyarch/ngl/scene/scene.hh"
#include "treyarch/ngl/scene/references.hh"

using namespace treyarch;

// sub_9D54C0
ngl::scene* ngl::set_color_target(ngl::texture* target) {
    ngl::scene* current_scene = d3d9::set_color_target(target);

    ngl::references::current_scene.read()->derived_matrices_dirty = 1;

    return current_scene;
}

// sub_9D3810
ngl::scene* ngl::set_depth_target(ngl::texture* target) {
    return d3d9::set_depth_target(target);
}

// sub_9D7D30
ngl::scene* ngl::set_camera_matrix(const matrix4x4* camera_to_world) {
    ngl::scene* current_scene = ngl::references::current_scene.read();

    current_scene->world_to_view = camera_to_world->inverse_orthonormal();
    current_scene->derived_matrices_dirty = 1;
    
    return current_scene;
}

// sub_9D7D00
matrix4x4* ngl::set_world_to_view_matrix(const matrix4x4* world_to_view) {
    scene* current_scene = references::current_scene.read();

    current_scene->world_to_view = *world_to_view;
    current_scene->derived_matrices_dirty = 1;

    return &current_scene->world_to_view;
}

// sub_9D3820
u32 ngl::set_clear_stencil(u32 stencil) {
    ngl::references::current_scene.read()->clear_stencil = stencil;

    return stencil;
}

// sub_9D38C0
ngl::scene* ngl::set_clear_depth(f32 depth) {
    ngl::scene* current_scene = ngl::references::current_scene.read();

    current_scene->clear_depth = depth;

    return current_scene;
}

// sub_9D7C10
ngl::scene* ngl::set_aspect_ratio(f32 ratio) {
    ngl::scene* current_scene = ngl::references::current_scene.read();

    current_scene->aspect_ratio = ratio;
    current_scene->derived_matrices_dirty = 1;

    return current_scene;
}

// sub_9D7C40
ngl::scene* ngl::set_perspective_parameters(f32 field_of_view,
                                            f32 near_plane,
                                            f32 far_plane) {

    ngl::scene* current_scene = ngl::references::current_scene.read();

    current_scene->field_of_view          = field_of_view;
    current_scene->ortho_width            = 0.0f;
    current_scene->ortho_height           = 0.0f;
    current_scene->near_plane             = near_plane;
    current_scene->projection_type        = projection_perspective;
    current_scene->far_plane              = far_plane;
    current_scene->derived_matrices_dirty = 1;

    return current_scene;
}

// sub_9D7CA0
ngl::scene* ngl::set_ortho_parameters(f32 ortho_width,
                                      f32 ortho_height,
                                      f32 near_plane,
                                      f32 far_plane) {
                                        
    ngl::scene* current_scene = ngl::references::current_scene.read();

    current_scene->field_of_view   = 0.0f;
    current_scene->ortho_width     = ortho_width;
    current_scene->ortho_height    = ortho_height;
    current_scene->near_plane      = near_plane;
    current_scene->projection_type = projection_orthographic;
    current_scene->far_plane       = far_plane;
    current_scene->derived_matrices_dirty = 1;

    return current_scene;
}
