#include <cstring>

#include "treyarch/ngl/debug/debug.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/list/arena.hh"
#include "treyarch/ngl/material/material.hh"
#include "treyarch/ngl/mesh/submission.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/shaders/shader.hh"

using namespace treyarch;

// sub_9E6F50
i32 ngl::cull_mesh_sphere(const math::position3 &center, f32 radius, u8 flags) {
    if (flags & 0x40)
        return 0;

    scene* current_scene = references::current_scene.read();

    for (u32 index = 0; index < 6; ++index) {
        const math::vector4 &plane = current_scene->clip_planes[index];

        if (plane[1] * center[1] +
            plane[0] * center[0] +
            plane[2] * center[2] -
            plane[3] + radius < 0.0f)
            
            return -1;
    }

    return 0;
}

// sub_9DB830
ngl::mesh_node* ngl::create_transient_mesh_node(      mesh*        value,
                                                const math::mat43* local_to_world) {

    if (!value || !value->section_count)
        return nullptr;

    const math::dir3      &x      = local_to_world->get_x();
    const math::dir3      &y      = local_to_world->get_y();
    const math::dir3      &z      = local_to_world->get_z();
    const math::position3 &w      = local_to_world->get_w();
    const math::vector4   &sphere = value->sphere;

    // every lane of the rows, the unused fourth one included
    math::position3 center(math::float4(x[0] * sphere[0] + y[0] * sphere[1] + z[0] * sphere[2] + w[0],
                                        x[1] * sphere[0] + y[1] * sphere[1] + z[1] * sphere[2] + w[1],
                                        x[2] * sphere[0] + y[2] * sphere[1] + z[2] * sphere[2] + w[2],
                                        x[3] * sphere[0] + y[3] * sphere[1] + z[3] * sphere[2] + w[3]));

    if (cull_mesh_sphere(center, sphere.get_w(), 0) == -1)
        return nullptr;

    mesh_node* node = (mesh_node*)list::allocate(0xB0, 64);
    node->mesh_data = (u8*)value;
    node->local_to_world = *local_to_world;

    scene* current_scene = references::current_scene.read();

    node->local_to_screen = ~(math::mat44(*local_to_world) * current_scene->world_to_screen);

    node->scale             = 1.0f;
    node->node_info         = &references::default_node_info.get();
    node->parameters        = references::default_parameters.read();
    node->point_light_count = 0;

    return node;
}

// sub_9DC110
ngl::mesh_node* ngl::list_add_transient_mesh(      mesh*        value,
                                             const math::mat43* local_to_world) {

    mesh_node* node = create_transient_mesh_node(value, local_to_world);

    if (!node)
        return nullptr;

    node->reserved_090[0] = 0;
    node->reserved_090[1] = 0;
    node->reserved_090[2] = 0;
    node->reserved_090[3] = 0;
    node->render_flags    = 0;

    for (u32 index = 0; index < value->section_count; ++index) {
        mesh_section* section = value->sections[index].section;

        if (!section)
            continue;

        material* section_material = section->material_data;
        shader* section_shader = section_material->shader_data;

        if (!section_shader->disabled)
            section_shader->add_node(node, section, section_material);
    }

    references::performance.get().total_polygons += value->polygon_count;

    u32 frame_epoch = references::frame_epoch.read();
    value->last_frame_reference = frame_epoch;

    if (value->owner_file)
        value->owner_file->last_frame_reference = frame_epoch;

    return node;
}
