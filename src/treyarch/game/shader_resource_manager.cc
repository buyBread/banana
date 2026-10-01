#include "treyarch/game/shader_resource_manager.hh"
#include "treyarch/shared/hash/algo.hh"

using namespace treyarch;

// sub_757BD0
IDirect3DVertexShader9** shader_resource_manager::find_vertex_program(const char* name) {
    auto* node = vertex_programs.find(hash::djb2(name));

    return node == vertex_programs.end() ?
        nullptr : &node->value.second;
}

// sub_757C20
IDirect3DPixelShader9** shader_resource_manager::find_pixel_program(const char* name) {
    auto* node = pixel_programs.find(hash::djb2(name));

    return node == pixel_programs.end() ?
        nullptr : &node->value.second;
}
