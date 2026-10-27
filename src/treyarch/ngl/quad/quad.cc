#include <cstring>

#include "treyarch/ngl/list/list.hh"
#include "treyarch/ngl/quad/quad.hh"
#include "treyarch/ngl/scene/scene.hh"

using namespace treyarch;

// sub_9E4A50
void ngl::init_quad(quad* value) {
    std::memset(value, 0, sizeof(quad));

    value->vertices[0].color = 0xFFFFFFFF;
    value->vertices[1].color = 0xFFFFFFFF;
    value->vertices[2].color = 0xFFFFFFFF;
    value->vertices[3].color = 0xFFFFFFFF;

    value->vertices[0].u = 0.0f;
    value->vertices[0].v = 0.0f;
    value->vertices[1].u = 1.0f;
    value->vertices[1].v = 0.0f;
    value->vertices[2].u = 0.0f;
    value->vertices[2].v = 1.0f;
    value->vertices[3].u = 1.0f;
    value->vertices[3].v = 1.0f;

    value->map_flags  = 49;
    value->blend_mode = 0x0000000506C10000ULL;
}

// sub_9E4B80
void ngl::set_quad_rect(quad* value,
                        f32   left,
                        f32   top,
                        f32   right,
                        f32   bottom) {
                            
    value->vertices[0].x = left;
    value->vertices[0].y = top;
    value->vertices[1].x = right;
    value->vertices[1].y = top;
    value->vertices[2].x = left;
    value->vertices[2].y = bottom;
    value->vertices[3].x = right;
    value->vertices[3].y = bottom;
}

// sub_9E4B60
void ngl::set_quad_color(quad* value, u32 color) {
    value->vertices[0].color = color;
    value->vertices[1].color = color;
    value->vertices[2].color = color;
    value->vertices[3].color = color;
}

// sub_9E4BD0
void ngl::set_quad_z(quad* value, f32 z) {
    value->z = z;
}

// sub_9E4AC0
void ngl::set_quad_tex(quad* value, texture* texture_data) {
    value->texture_data = texture_data;
}

// sub_9E4AE0
void ngl::set_quad_map_flags(quad* value, u32 map_flags) {
    value->map_flags = map_flags;
}

// sub_9E4AF0
void ngl::set_quad_blend(quad* value, u32 mode, u32 source) {
    value->blend_mode = ((u64)source << 32) | mode;
}

// sub_9E4B10
void ngl::set_quad_uv(quad* value,
                      f32   left,
                      f32   top,
                      f32   right,
                      f32   bottom) {

    value->vertices[0].u = left;
    value->vertices[2].u = left;
    value->vertices[0].v = top;
    value->vertices[1].u = right;
    value->vertices[1].v = top;
    value->vertices[2].v = bottom;
    value->vertices[3].u = right;
    value->vertices[3].v = bottom;
}

// sub_9E4C20
ngl::scene* ngl::list_add_quad(const quad* value) {
    auto* node = (quad_renderer::node*)list_alloc(sizeof(quad_renderer::node), 16);

    if (!node)
        return nullptr;

    node->base.vtable = &quad_renderer::references::node_vtable.get();

    scene* current = references::build_scene.read();
    validate_matrices(current);

    std::memcpy(&node->value, value, sizeof(quad));

    if ((value->blend_mode & 0x00800000) != 0) {
        node->base.sort_key.floating = value->z;
        node->base.next = current->translucent_render_list;

        ++current->translucent_render_list_count;
        current->translucent_render_list = &node->base;
    } else {
        node->base.sort_key.integer = (u32)value->texture_data;
        node->base.next = current->opaque_render_list;

        ++current->opaque_render_list_count;
        current->opaque_render_list = &node->base;
    }

    return current;
}
