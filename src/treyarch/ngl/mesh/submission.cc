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
i32 ngl::cull_mesh_sphere(const vector4 &center, f32 radius, u8 flags) {
    if (flags & 0x40)
        return 0;

    scene* current_scene = references::current_scene.read();

    for (u32 index = 0; index < 6; ++index) {
        const vector4 &plane = current_scene->clip_planes[index];

        if (plane.y * center.y +
            plane.x * center.x +
            plane.z * center.z - 
            plane.w + radius < 0.0f)
            
            return -1;
    }

    return 0;
}

// sub_9DB830
ngl::mesh_node* ngl::create_transient_mesh_node(      mesh*      value,
                                                const matrix4x4* local_to_world) {

    if (!value || !value->section_count)
        return nullptr;

    vector4 center {
        local_to_world->x.x * value->sphere.x +
        local_to_world->y.x * value->sphere.y +
        local_to_world->z.x * value->sphere.z + local_to_world->w.x,
        local_to_world->x.y * value->sphere.x +
        local_to_world->y.y * value->sphere.y +
        local_to_world->z.y * value->sphere.z + local_to_world->w.y,
        local_to_world->x.z * value->sphere.x +
        local_to_world->y.z * value->sphere.y +
        local_to_world->z.z * value->sphere.z + local_to_world->w.z,
        local_to_world->x.w * value->sphere.x +
        local_to_world->y.w * value->sphere.y +
        local_to_world->z.w * value->sphere.z + local_to_world->w.w
    };

    if (cull_mesh_sphere(center, value->sphere.w, 0) == -1)
        return nullptr;

    mesh_node* node = (mesh_node*)list::allocate(0xB0, 64);
    node->mesh_data = (u8*)value;
    node->local_to_world = *local_to_world;

    scene* current_scene = references::current_scene.read();
    matrix4x4 local_to_screen = (local_to_world->affine() *
                                 current_scene->world_to_screen).transpose();

    std::memcpy(node->reserved_040, &local_to_screen, sizeof(local_to_screen));

    node->scale             = 1.0f;
    node->node_info         = &references::default_node_info.get();
    node->parameters        = references::default_parameters.read();
    node->point_light_count = 0;

    return node;
}

// sub_9DC110
ngl::mesh_node* ngl::list_add_transient_mesh(      mesh*      value,
                                             const matrix4x4* local_to_world) {

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
