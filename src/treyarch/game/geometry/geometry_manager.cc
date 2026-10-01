#include "treyarch/app/app.hh"
#include "treyarch/game/geometry/geometry_manager.hh"
#include "treyarch/game/wds/camera/camera.hh"
#include "treyarch/game/wds/camera/references.hh"
#include "treyarch/ngl/display.hh"
#include "treyarch/shared/math/references.hh"

using namespace treyarch;

// sub_606650
// the milestone's geometry_manager::set_look_at; retail PC passes dest as the ECX receiver
void geometry_manager::set_look_at(      matrix4x4*  dest,
                                   const vector3    &from,
                                   const vector3    &look_at,
                                   const vector3    &up) {

    vector3 z_axis  = look_at;
            z_axis -= from;
    f32     length  = z_axis.length();

    if (length <= 1e-6f) {
        z_axis = math::references::zvec.get();
        length = 1.0f;
    }

    z_axis *= 1.0f / length;

    vector3 z_component  = z_axis;
            z_component *= dot(up, z_axis);
    vector3 y_axis       = up;
            y_axis      -= z_component;
    f32     y_length     = y_axis.length();

    // up is parallel to the view direction, orthogonalize the world y axis instead, then z
    if (y_length < 1e-6f) {
        y_axis = vector3(0.0f - z_axis.x * z_axis.y,
                         1.0f - z_axis.y * z_axis.y,
                         0.0f - z_axis.z * z_axis.y);
        y_length = y_axis.length();

        if (y_length < 1e-6f) {
            y_axis = vector3(0.0f - z_axis.x * z_axis.z,
                             0.0f - z_axis.y * z_axis.z,
                             1.0f - z_axis.z * z_axis.z);
            y_length = y_axis.length();
        }
    }

    y_axis *= 1.0f / y_length;

    vector3 x_axis = cross(y_axis, z_axis);

    *dest = matrix4x4( x_axis.x,              y_axis.x,              z_axis.x,             0.0f,
                       x_axis.y,              y_axis.y,              z_axis.y,             0.0f,
                       x_axis.z,              y_axis.z,              z_axis.z,             0.0f,
                      -dot(from, x_axis),    -dot(from, y_axis),    -dot(from, z_axis),    1.0f);
}

// sub_96AF70
void geometry_manager::set_view(const vector3 &from,
                                const vector3 &look_at,
                                const vector3 &up) {

    f32 field_of_view = treyarch::references::game.read()->get_current_view_camera()->get_fov();

    if (field_of_view == 0.0f)
        field_of_view = (f32)treyarch::references::default_fov_degrees.read();

    treyarch::references::camera_fov_radians.write(field_of_view * PI / 180.0f);

    matrix4x4 projection;
    projection.make_projection(treyarch::references::camera_fov_radians.read(),
                               treyarch::references::camera_aspect_ratio.read(),
                               treyarch::references::camera_near_plane.read(),
                               treyarch::references::camera_far_plane.read(),
                               0.0f);

    f32 half_width  = (f32)ngl::get_screen_width()  * 0.5f;
    f32 half_height = (f32)ngl::get_screen_height() * 0.5f;

    matrix4x4 projection_to_screen(half_width,  0.0f,        0.0f, 0.0f,
                                   0.0f,       -half_height, 0.0f, 0.0f,
                                   0.0f,        0.0f,        1.0f, 0.0f,
                                   half_width,  half_height, 0.0f, 1.0f);

    references::view_to_screen.write(projection * projection_to_screen);

    matrix4x4 view;
    set_look_at(&view, from, look_at, up);

    references::world_to_view.write(references::scene_analyzer_enabled.read() ?
        references::scene_analyzer.get() : view);

    const matrix4x4 &world_to_view = references::world_to_view.get();

    // retail builds the inverse inside the projection's storage and only writes xyz,
    // so the w column keeps the projection's (0, 0, 1, 0), its last lane negated with the translation
    projection.x_row() = vector3(world_to_view.x.x, world_to_view.y.x, world_to_view.z.x);
    projection.y_row() = vector3(world_to_view.x.y, world_to_view.y.y, world_to_view.z.y);
    projection.z_row() = vector3(world_to_view.x.z, world_to_view.y.z, world_to_view.z.z);
    projection.w       = -vector4(dot(world_to_view.w_row(), world_to_view.x_row()),
                                  dot(world_to_view.w_row(), world_to_view.y_row()),
                                  dot(world_to_view.w_row(), world_to_view.z_row()),
                                  projection.w.w);

    references::view_to_world.write(projection);
    references::world_to_screen.write(world_to_view * references::view_to_screen.get());
}
