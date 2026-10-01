#include "treyarch/game/shadow/shadow.hh"
#include "treyarch/ngl/scene/scene.hh"

using namespace treyarch;

// inlined into sub_771C10
bool sphere_inside_clip_planes(const ngl::scene* value,
                               const vector3    &center,
                                     f32         radius) {

    for (u32 index = 0; index < 6; ++index) {
        const vector4 &plane = value->clip_planes[index];

        f32 dot = (f32)((f64)plane.y * (f64)center.y +
                        (f64)plane.x * (f64)center.x +
                        (f64)plane.z * (f64)center.z +
                        (f64)plane.w * 0.0);

        f32 distance = (f32)((f64)dot - (f64)plane.w);

        if ((f64)distance + (f64)radius < 0.0)
            return false;
    }

    return true;
}

// sub_771C10
u32 shadow::get_cascade_mask(const vector3 &center, f32 radius) {
    ngl::scene* scene_0 = references::scene_0.read();
    ngl::scene* scene_1 = references::scene_1.read();

    if (!references::active.read() || !scene_0 || !scene_1)
        return 0;

    u32 mask = sphere_inside_clip_planes(scene_0, center, radius) ? 1 : 0;

    if (sphere_inside_clip_planes(scene_1, center, radius))
        mask |= 2;

    return mask;
}
