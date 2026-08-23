#include <cstring>

#include "treyarch/ngl/list/arena.hh"
#include "treyarch/ngl/quad/quad.hh"
#include "treyarch/ngl/scene/matrices.hh"
#include "treyarch/ngl/scene/references.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"

using namespace treyarch;

struct ngl_quad_node {
    ngl::render_node base;
    u32              pad_00c;
    ngl::quad        value;
};

ASSERT_SIZEOF  (ngl_quad_node,        0x78);
ASSERT_OFFSETOF(ngl_quad_node, value, 0x10);

static util::memory_reference<void*> quad_node_vtable { 0x00DB8F54 };

void ngl::list_add_quad(const quad* value) {
    ngl_quad_node* node = (ngl_quad_node*)list::allocate(sizeof(ngl_quad_node), 16);

    if (node) {
        node->base.vtable = (void*)&quad_node_vtable.get();

        scene* current = references::current_scene.read();
        validate_matrices(current);

        std::memcpy(&node->value, value, sizeof(quad));

        if ((value->blend_mode & 0x00800000) != 0) {
            node->base.sort_key.floating = value->z;
            node->base.next = current->translucent_render_list;
            
            ++current->translucent_render_list_count;
            current->translucent_render_list = &node->base;
        }
        else {
            node->base.sort_key.integer = (u32)value->texture_data;
            node->base.next = current->opaque_render_list;
            
            ++current->opaque_render_list_count;
            current->opaque_render_list = &node->base;
        }
    }
}
