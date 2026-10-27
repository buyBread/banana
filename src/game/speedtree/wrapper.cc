#include <algorithm>
#include <cstring>

#include "retail.hh"
#include "game/speedtree/wrapper.hh"
#include "treyarch/ngl/fx/parameters.hh"
#include "treyarch/ngl/fx/pass.hh"
#include "treyarch/ngl/fx/references.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/scene/scene.hh"
#include "game/hash/algo.hh"
#include "game/math/references.hh"
#include "treyarch/mm/heap.hh"

using namespace treyarch;

// sub_7B5290
void speed_tree_lighting::build(const light_source_data* light) {
    const sky_data* sky = light->sky;

    if (!sky) {
        light_direction = ngl::fx::references::lighting_default_value.read();
        diffuse         = ngl::fx::references::lighting_zero_value.read();
        ambient         = ngl::fx::references::lighting_zero_value.read();
        unk_30          = ngl::fx::references::lighting_zero_value.read();

        exposure        = 1.0f;
        diffuse_scale   = 0.0f;
        diffuse_maximum = 0.0f;
        diffuse_minimum = 0.0f;

        return;
    }

    f32 scale = references::speed_tree_sky_color_scale.read();

    diffuse = math::vector4((f32)((f64)sky->color_040.x * scale),
                            (f32)((f64)sky->color_040.y * scale),
                            (f32)((f64)sky->color_040.z * scale),
                            1.0f);

    ambient = math::vector4((f32)((f64)(f32)((f64)sky->unk_080.x * scale) * 0.5),
                            (f32)((f64)(f32)((f64)sky->unk_080.y * scale) * 0.5),
                            (f32)((f64)(f32)((f64)sky->unk_080.z * scale) * 0.5),
                            (f32)((f64)(f32)((f64)sky->unk_080.w * scale) * 0.5));

    unk_30 = math::vector4((f32)((f64)scale * sky->unk_0b0.x),
                           (f32)((f64)scale * sky->unk_0b0.y),
                           (f32)((f64)scale * sky->unk_0b0.z),
                           (f32)((f64)scale * sky->unk_0b0.w));

    const math::vector4 &axis = ngl::fx::references::lighting_horizon_axis.get();

    f32 facing = (f32)((f64)sky->direction_050.x * axis[0] +
                       (f64)sky->direction_050.y * axis[1] +
                       (f64)sky->direction_050.z * axis[2]);

    f32 away = (f32)(1.0 - facing);

    diffuse_minimum = (f32)((f64)away - sky->unk_0c8);
    diffuse_maximum = (f32)((f64)sky->unk_0c8 + away);
    diffuse_scale   = (f32)(1.0 / ((f64)sky->unk_0c8 * 2.0));

    // halved twice
    ambient = math::vector4((f32)((f64)ambient[0] * 0.5),
                            (f32)((f64)ambient[1] * 0.5),
                            (f32)((f64)ambient[2] * 0.5),
                            (f32)((f64)ambient[3] * 0.5));

    light_direction = sky->direction_050;
    exposure        = light->exposure;
    depth_fog_color = light->color_2d0;

    f32 fog_start = light->depth_fog_start;
    f32 fog_end   = light->depth_fog_end;

    if (fog_start == fog_end)
        fog_end = (f32)((f64)fog_end + 1.525879997643642e-05);

    f32 fog_scale = (f32)(1.0 / ((f64)fog_end - fog_start));

    depth_fog_control = math::vector4(fog_scale,
                                      (f32)((f64)-fog_start * fog_scale),
                                      light->unk_2e8,
                                      light->unk_2ec);
}

// sub_7C0270
void speed_tree_lighting::shade(const u8* color, math::vector4 (&result)[2]) const {
    f32 low  = (f32)((f64)color[0] / 255.0);
    f32 high = (f32)(2.0 - (f64)color[1] / 255.0);

    f32 minimum = diffuse_minimum;

    if (low > minimum)
        minimum = low;
    else if (minimum > high)
        minimum = high;

    f32 maximum = diffuse_maximum;

    if (low > maximum)
        maximum = low;
    else if (maximum > high)
        maximum = high;

    f32 range    = (f32)(((f64)maximum - minimum) * diffuse_scale);
    f32 strength = (f32)((f64)color[2] / 255.0);

    result[1] = math::vector4((f32)((f64)ambient[0] * strength),
                              (f32)((f64)ambient[1] * strength),
                              (f32)((f64)ambient[2] * strength),
                              (f32)((f64)ambient[3] * strength));

    result[0] = math::vector4((f32)((f64)(f32)((f64)diffuse[0] * range) + result[1][0]),
                              (f32)((f64)(f32)((f64)diffuse[1] * range) + result[1][1]),
                              (f32)((f64)(f32)((f64)diffuse[2] * range) + result[1][2]),
                              (f32)((f64)(f32)((f64)diffuse[3] * range) + result[1][3]));
}

// inlined @ sub_7CC710, sub_7CD8C0, sub_7CEA90, sub_7CFC10
bool speed_tree_instance::in_view(const math::mat43 &basis) const {
    vector3d transformed;

    return speed_tree_wrapper::sphere_in_view(basis, position, radius, transformed);
}

// inlined @ sub_7CC710, sub_7CD8C0, sub_7CEA90, sub_7CFC10, sub_7D8390
bool speed_tree_wrapper::sphere_in_view(const math::mat43 &basis,
                                        const vector3d    &position,
                                              f32          radius,
                                              vector3d    &transformed) {

    const math::dir3      &x = basis.get_x();
    const math::dir3      &y = basis.get_y();
    const math::dir3      &z = basis.get_z();
    const math::position3 &w = basis.get_w();

    transformed = vector3d((f32)((f64)x[0] * position.x + (f64)y[0] * position.y + (f64)z[0] * position.z + w[0]),
                           (f32)((f64)x[1] * position.x + (f64)y[1] * position.y + (f64)z[1] * position.z + w[1]),
                           (f32)((f64)x[2] * position.x + (f64)y[2] * position.y + (f64)z[2] * position.z + w[2]));

    ngl::scene* scene = ngl::references::current_scene.read();

    for (u32 index = 0; index < 6; ++index) {
        const math::vector4 &plane = scene->clip_planes[index];

        f32 distance = (f32)((f64)plane[0] * transformed.x +
                             (f64)plane[1] * transformed.y +
                             (f64)plane[2] * transformed.z +
                             (f64)plane[3] * 0.0);

        distance = (f32)((f64)distance - plane[3]);

        if (0.0 > (f64)distance + radius)
            return false;
    }

    return true;
}

// inlined @ sub_7CB940, sub_7CC710, sub_7CD8C0, sub_7CEA90, sub_7CFC10
math::mat43 speed_tree_wrapper::axis_basis() {
    return math::mat43(math::dir3(math::references::f4_1000.read()),
                       math::dir3(math::references::f4_0010.read()),
                       math::dir3(math::references::f4_0100.read()),
                       math::position3(ngl::fx::references::unk_01086f40.read()));
}

// inlined @ sub_7CB940, sub_7CC710, sub_7CD8C0, sub_7CEA90, sub_7CFC10
void speed_tree_wrapper::find_technique(u32                 &guard,
                                        u32                  bit,
                                        ngl::fx::technique* &technique,
                                        const char*          name) {

    if (guard & bit)
        return;

    guard |= bit;

    fixed_string technique_name;

    technique_name.hash = string_hash(hash::djb2(name));
    technique_name.set_text(name);

    technique = ngl::fx::find_technique(effect, technique_name);

    if (technique_name.text)
        memory::heap::free(technique_name.text);
}

// sub_7BFAD0
void speed_tree_wrapper::upload_wind_matrices() {
    math::mat44 matrices[7];

    for (u32 index = 0; index < 7; ++index) {
        auto* matrix = (math::mat44*)retail::sub_A43580((u32*)&references::speed_tree_wind.get(), index);

        if (matrix) {
            matrices[index] = *matrix;

            continue;
        }

        matrices[index] = math::mat44(math::mat33(math::dir3(1.0f, 0.0f, 0.0f),
                                                  math::dir3(0.0f, 1.0f, 0.0f),
                                                  math::dir3(0.0f, 0.0f, 1.0f)));
    }

    // array parameters keep their values 16 bytes in
    std::memcpy((u8*)parameters.wind_matrices->data + 16, matrices, sizeof(matrices));

    ngl::fx::set_float(parameters.self_shadow_motion, 0.0f);
}

// sub_7BFC00
void speed_tree_wrapper::upload_lighting(light_source_data* light) {
    lighting.build(light);

    ngl::fx::set_float(parameters.exposure, lighting.exposure);
    ngl::fx::set_vector(parameters.light_direction, math::vector4(math::dir3(lighting.light_direction)));
    ngl::fx::set_vector(parameters.depth_fog_color, lighting.depth_fog_color);
    ngl::fx::set_vector(parameters.depth_fog_control, lighting.depth_fog_control);

    ngl::scene* scene = ngl::references::current_scene.read();

    ngl::fx::set_vector(parameters.camera_position, math::vector4(scene->view_position));
}

// sub_7CB850
void speed_tree_wrapper::upload_color(const u8* color) {
    math::vector4 shaded[2];

    lighting.shade(color, shaded);

    f32 diffuse_level = std::max(shaded[0][1], 2.0f);
    f32 ambient_level = std::max(shaded[1][1], 0.35f);

    ngl::fx::set_vector(parameters.diffuse, math::vector4(diffuse_level, diffuse_level, diffuse_level, 1.0f));
    ngl::fx::set_vector(parameters.ambient, math::vector4(ambient_level, ambient_level, ambient_level, 0.0f));
}

// inlined @ sub_7CB940, sub_7CC710, sub_7CD8C0, sub_7CEA90
void speed_tree_wrapper::upload_transforms(const math::mat43 &basis) {
    ngl::scene* scene = ngl::references::current_scene.read();

    ngl::fx::set_matrix(parameters.model_view_projection, math::mat44(basis) * scene->world_to_screen);
    ngl::fx::set_matrix(parameters.local_to_world, math::mat44(basis));
    ngl::fx::set_matrix(parameters.model_view, basis * math::mat43(scene->world_to_view));
}

// inlined @ sub_7CB940, sub_7CC710
f32 speed_tree_wrapper::upload_billboard_images() { // returns the special azimuth
    ngl::scene* scene = ngl::references::current_scene.read();

    if (scene->color_target) {
        ngl::fx::set_texture(parameters.billboard_diffuse_map, textures[16]);
        ngl::fx::set_texture(parameters.billboard_normal_map, textures[18]);
    }

    std::memcpy((u8*)parameters.texcoords_360->data + 16, texcoords_360, image_count_360 * sizeof(math::mat44));

    ngl::fx::set_int(parameters.image_count_360, image_count_360);
    ngl::fx::set_vector(parameters.constants_360, math::vector4((f32)(6.2831854820251465 / (f32)image_count_360), 0.0f, 0.0f, 0.0f));
    ngl::fx::set_float(parameters.billboard_light_adjust, 1.0f);

    f32 azimuth = 0.0f;
    f32 pitch   = 0.0f;

    if (scene->color_target)
        retail::sub_A3C920((u32*)&azimuth, (u32*)&pitch);

    ngl::fx::set_vector(parameters.camera_angles,
                        math::vector4((f32)((f64)azimuth * 3.1415927410125732 / 180.0),
                                      (f32)((f64)pitch * 3.1415927410125732 / 180.0),
                                      0.0f,
                                      0.0f));

    azimuth = (f32)((f64)azimuth + 180.0);

    if (azimuth > 360.0f)
        azimuth = (f32)((f64)azimuth - 360.0);

    return (f32)(azimuth * 0.017453293005625408);
}
