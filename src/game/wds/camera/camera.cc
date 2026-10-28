#include "retail.hh"
#include "game/wds/camera/camera.hh"

using namespace treyarch;

vector3d marky_camera::get_orbit_pos(const vector3d &center, f32 range, f32 theta, f32 psi) {
    vector3d position;
    retail::sub_78BE90((i32)&position, (i32)&center, range, theta, psi);

    return position;
}
