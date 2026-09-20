#include "treyarch/ngl/resources/references.hh"
#include "treyarch/ngl/resources/shader_resource_manager.hh"
#include "treyarch/shared/hash/algo.hh"

using namespace treyarch;

ngl::resources::program_node* find_program_node(const ngl::resources::program_tree &tree,
                                                      u32                           name_hash) {

    ngl::resources::program_node* sentinel  = tree.sentinel;
    ngl::resources::program_node* candidate = sentinel;
    ngl::resources::program_node* current   = sentinel->parent;

    while (!current->is_nil) {
        if (current->name_hash >= name_hash) {
            candidate = current;
            current = current->left;
        } else
            current = current->right;
    }

    if (candidate == sentinel || name_hash < candidate->name_hash)
        return nullptr;

    return candidate;
}

IDirect3DVertexShader9** ngl::resources::find_vertex_program(const char* name) {
    auto* manager = (shader_resource_manager*)references::shader_resource_manager.read();

    program_node* node = find_program_node(manager->vertex_programs, hash::djb2(name));

    return node ? (IDirect3DVertexShader9**)&node->program : nullptr;
}

IDirect3DPixelShader9** ngl::resources::find_pixel_program(const char* name) {
    auto* manager = (shader_resource_manager*)references::shader_resource_manager.read();

    program_node* node = find_program_node(manager->pixel_programs, hash::djb2(name));

    return node ? (IDirect3DPixelShader9**)&node->program : nullptr;
}
