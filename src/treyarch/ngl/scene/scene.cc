#include "treyarch/ngl/scene/scene.hh"
#include "treyarch/ngl/scene/references.hh"

using namespace treyarch;

ngl::scene* ngl::set_color_target(ngl::texture* target) {
    ngl::scene* current_scene = ngl::references::current_scene.read();

    current_scene->color_target  = target;
    current_scene->cube_map_face = 0;

    if (!target)
        current_scene->options = 0;

    current_scene->derived_matrices_dirty = 1;

    return current_scene;
}

ngl::scene* ngl::set_depth_target(ngl::texture* target) {
    ngl::scene* current_scene = ngl::references::current_scene.read();

    current_scene->depth_target = target;

    if (!target)
        current_scene->options = 0;

    return current_scene;
}

ngl::scene* ngl::set_auxiliary_target(ngl::texture* target) {
    ngl::scene* current_scene = ngl::references::current_scene.read();

    current_scene->auxiliary_target = target;

    if (!target)
        current_scene->options = 0;

    return current_scene;
}

ngl::scene* ngl::set_camera_matrix(const matrix4x4* camera_to_world) {
    ngl::scene* current_scene = ngl::references::current_scene.read();

    current_scene->world_to_view = camera_to_world->inverse_orthonormal();
    current_scene->derived_matrices_dirty = 1;
    
    return current_scene;
}

matrix4x4* ngl::set_world_to_view_matrix(const matrix4x4* world_to_view) {
    scene* current_scene = references::current_scene.read();

    current_scene->world_to_view = *world_to_view;
    current_scene->derived_matrices_dirty = 1;

    return &current_scene->world_to_view;
}

ngl::scene* ngl::set_clear_depth(f32 depth) {
    ngl::scene* current_scene = ngl::references::current_scene.read();

    current_scene->clear_depth = depth;

    return current_scene;
}

ngl::scene* ngl::set_aspect_ratio(f32 ratio) {
    ngl::scene* current_scene = ngl::references::current_scene.read();

    current_scene->aspect_ratio = ratio;
    current_scene->derived_matrices_dirty = 1;

    return current_scene;
}
ngl::scene* ngl::set_ortho_parameters(f32 ortho_width,
                                      f32 ortho_height,
                                      f32 near_plane,
                                      f32 far_plane) {
                                        
    ngl::scene* current_scene = ngl::references::current_scene.read();

    current_scene->field_of_view   = 0.0f;
    current_scene->ortho_width     = ortho_height;
    current_scene->ortho_height    = ortho_width;
    current_scene->near_plane      = near_plane;
    current_scene->projection_type = projection_orthographic;
    current_scene->far_plane       = far_plane;
    current_scene->derived_matrices_dirty = 1;

    return current_scene;
}
