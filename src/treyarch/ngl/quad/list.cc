#include <cstring>

#include "treyarch/ngl/list/arena.hh"
#include "treyarch/ngl/quad/quad.hh"
#include "treyarch/ngl/scene/matrices.hh"
#include "treyarch/ngl/scene/references.hh"

using namespace treyarch;

void ngl::list_add_quad(const quad* value) {
    auto* node = (quad_renderer::node*)list::allocate(sizeof(quad_renderer::node), 16);

    if (node) {
        node->base.vtable = &quad_renderer::references::node_vtable.get();

        scene* current = references::current_scene.read();
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
    }
}
