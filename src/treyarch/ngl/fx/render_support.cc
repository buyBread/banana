#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "treyarch/ngl/scene/scene.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "treyarch/ngl/fx/parameters.hh"
#include "treyarch/ngl/fx/references.hh"
#include "treyarch/ngl/fx/render_support.hh"
#include "game/light/lighting.hh"

using namespace treyarch;

// sub_884C70
i32 gather_point_lights(      ngl::fx::mesh_node_data* node_data,
                        const ngl::mesh_section*       section) {

    const math::vector4   &local_sphere = section->sphere;
    const math::dir3      &x            = node_data->local_to_world.get_x();
    const math::dir3      &y            = node_data->local_to_world.get_y();
    const math::dir3      &z            = node_data->local_to_world.get_z();
    const math::position3 &w            = node_data->local_to_world.get_w();

    // every lane of the rows, the unused fourth one included
    math::vector4 sphere((f32)((f64)x[0] * (f64)local_sphere[0] +
                               (f64)y[0] * (f64)local_sphere[1] +
                               (f64)z[0] * (f64)local_sphere[2] +
                               (f64)w[0]),
                         (f32)((f64)x[1] * (f64)local_sphere[0] +
                               (f64)y[1] * (f64)local_sphere[1] +
                               (f64)z[1] * (f64)local_sphere[2] +
                               (f64)w[1]),
                         (f32)((f64)x[2] * (f64)local_sphere[0] +
                               (f64)y[2] * (f64)local_sphere[1] +
                               (f64)z[2] * (f64)local_sphere[2] +
                               (f64)w[2]),
                         (f32)((f64)x[3] * (f64)local_sphere[0] +
                               (f64)y[3] * (f64)local_sphere[1] +
                               (f64)z[3] * (f64)local_sphere[2] +
                               (f64)w[3]));

    f32 scale = (node_data->node_info[0] & 2) ? node_data->scale : 1.0f;

    point_light_set lights;

    return lights.gather(node_data, section, sphere, scale);
}

// sub_9DF800
ngl::fx::effect* ngl::fx::shader_node::select_effect() {
    gather_point_lights(mesh_node, section);

    effect* base = effect_data;
    effect_runtime* runtime = base->runtime;
    u8 count = mesh_node->point_light_count;

    if (!count && runtime->zero_point_lights)
        return runtime->zero_point_lights;

    if (count <= 2 && runtime->two_point_lights)
        return runtime->two_point_lights;

    if (count <= 4 && runtime->four_point_lights)
        return runtime->four_point_lights;

    return base;
}

// inlined @ sub_9E0420
// inlined @ sub_9E0A50
void ngl::fx::prepare_effect_scene(effect* value) {
    u32 frame_epoch = ngl::references::frame.read();

    if (value->current_frame_epoch != frame_epoch) {
        value->current_frame_epoch = frame_epoch;
        value->current_scene       = nullptr;
        value->current_state_02c   = 0;
        value->current_material    = nullptr;
    }

    scene* build_scene = ngl::references::build_scene.read();

    if (value->current_scene != build_scene) {
        update_scene_parameters(value);
        value->current_scene = build_scene;
    }
}

// inlined @ sub_9E0420
// inlined @ sub_9E0A50
void ngl::fx::prepare_effect_material(effect* value, material* material_data) {
    if (value->current_material == material_data)
        return;

    if (material_data->bound_effect != value)
        bind_material(material_data, value);

    copy_material_parameters(material_data);
    value->current_material = material_data;
}

// inlined @ sub_9E0420
// inlined @ sub_9E0A50
void ngl::fx::prepare_animated_textures(effect* effect_data, shader_node* node) {
    material*  material_data = node->material_data;
    parameter* sentinel      = (parameter*)&references::parameter_chain_sentinel.get();

    if (material_data->animated_texture_parameter_chain == sentinel)
        return;

    if (!effect_data->animated_texture_parameter_chain)
        build_animated_texture_parameter_chain(effect_data);

    if (!material_data->animated_texture_parameter_chain)
        build_animated_texture_parameter_chain(effect_data, material_data);

    node->set_ifl_frame();
}
