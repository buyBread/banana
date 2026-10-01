#include "treyarch/game/shadow/shadow.hh"
#include "treyarch/ngl/d3d9/framebuffer.hh"
#include "treyarch/ngl/scene/scene.hh"
#include "treyarch/ngl/texture/runtime.hh"
#include "treyarch/shared/four_cc.hh"

using namespace treyarch;

// sub_7719A0
void shadow::create_device_resources() {
    if (references::initialized.read())
        return;

    references::projection_to_texture.write(matrix4x4(0.5f,  0.0f, 0.0f, 0.0f,
                                                      0.0f, -0.5f, 0.0f, 0.0f,
                                                      0.0f,  0.0f, 1.0f, 0.0f,
                                                      0.5f,  0.5f, 0.0f, 1.0f));
    references::initialized.write(1);

    device_resource_state &targets    = references::device_resources.get();
    target_dimensions     &dimensions = references::dimensions.get();
    D3DFORMAT              format     = ngl::d3d9::references::framebuffers.get().depth_texture_format;

    targets.depth_targets[0] = nullptr;
    targets.color_targets[0] = nullptr;
    targets.depth_targets[1] = nullptr;
    targets.color_targets[1] = nullptr;

    if (format == (D3DFORMAT)four_cc('R', 'A', 'W', 'Z'))
        references::active.write(0);

    for (u32 index = 0; index < 2; ++index) {
        targets.depth_targets[index] = ngl::create_runtime_texture(ngl::runtime_texture_surface_only | ngl::runtime_texture_surface_level | 0x10,
                                                                   format,
                                                                   dimensions.widths[index],
                                                                   dimensions.heights[index],
                                                                   1);

        if (references::create_color_targets.read()) {
            targets.color_targets[index] = ngl::create_runtime_texture(ngl::runtime_texture_render_target | 0x10,
                                                                       D3DFMT_A8R8G8B8,
                                                                       dimensions.widths[index],
                                                                       dimensions.heights[index],
                                                                       1);
        }
    }
}

// sub_771B10
void shadow::release_device_resources() {
    if (!references::initialized.read())
        return;

    references::initialized.write(0);

    device_resource_state &targets = references::device_resources.get();

    for (u32 index = 0; index < 2; ++index) {
        ngl::release_texture(targets.depth_targets[index]);
        ngl::release_texture(targets.color_targets[index]);
    }
}

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
