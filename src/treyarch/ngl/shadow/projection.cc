#include "treyarch/ngl/scene/scene.hh"
#include "treyarch/ngl/shadow/device_resources.hh"
#include "treyarch/ngl/shadow/projection.hh"

using namespace treyarch;

namespace treyarch { namespace ngl { namespace shadow {
namespace projection {
    bool contains_sphere(const scene*   value,
                         const vector4 &sphere) {

        if (!value)
            return false;

        for (u32 index = 0; index < 6; ++index) {
            const vector4 &plane = value->clip_planes[index];

            f32 distance = plane.x * sphere.x +
                           plane.y * sphere.y +
                           plane.z * sphere.z - plane.w;

            if (distance + sphere.w < 0.0f)
                return false;
        }

        return true;
    }
} // projection
}}} // treyarch::ngl::shadow

u32 ngl::shadow::build_projection_matrices(      matrix4x4*  output,
                                           const matrix4x4  &local_to_world,
                                           const vector4    &local_sphere) {

    if (!references::active.read())
        return 0;

    vector4 world_sphere(local_sphere.x * local_to_world[0][0] + local_sphere.y * local_to_world[1][0] + local_sphere.z * local_to_world[2][0] + local_to_world[3][0],
                         local_sphere.x * local_to_world[0][1] + local_sphere.y * local_to_world[1][1] + local_sphere.z * local_to_world[2][1] + local_to_world[3][1],
                         local_sphere.x * local_to_world[0][2] + local_sphere.y * local_to_world[1][2] + local_sphere.z * local_to_world[2][2] + local_to_world[3][2],
                         local_sphere.w);

    bool in_first  = projection::contains_sphere(references::scene_0.read(), world_sphere);
    bool in_second = projection::contains_sphere(references::scene_1.read(), world_sphere);

    const matrix4x4 &texture = references::projection_to_texture.get();

    if (in_first && in_second) {
        output[0] = local_to_world.affine() * references::matrix_0.get() * texture;
        output[1] = local_to_world.affine() * references::matrix_1.get() * texture;

        return 2;
    }

    if (in_first) {
        output[0] = local_to_world.affine() * references::matrix_0.get() * texture;

        return 1;
    }

    if (in_second) {
        output[0] = local_to_world.affine() * references::matrix_1.get() * texture;

        return 1;
    }

    return 0;
}
