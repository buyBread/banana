#pragma once

#include <d3d9.h>

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace resources {
    struct program_node {
        program_node* left;
        program_node* parent;
        program_node* right;
        u32           name_hash;
        void*         program;
        u8            color;
        u8            is_nil;
    };

    struct program_tree {
        u32           reserved_000;
        program_node* sentinel;
        u32           size;
    };

    struct shader_resource_manager {
        program_tree vertex_programs;
        program_tree pixel_programs;
    };

    IDirect3DVertexShader9** find_vertex_program(const char* name);
    IDirect3DPixelShader9**  find_pixel_program(const char* name);

    ASSERT_OFFSETOF(program_node, left,      0x00);
    ASSERT_OFFSETOF(program_node, parent,    0x04);
    ASSERT_OFFSETOF(program_node, right,     0x08);
    ASSERT_OFFSETOF(program_node, name_hash, 0x0C);
    ASSERT_OFFSETOF(program_node, program,   0x10);
    ASSERT_OFFSETOF(program_node, is_nil,    0x15);

    ASSERT_SIZEOF  (program_tree,           0x0C);
    ASSERT_OFFSETOF(program_tree, sentinel, 0x04);

    ASSERT_OFFSETOF(shader_resource_manager, pixel_programs, 0x0C);
}}} // treyarch::ngl::resources
