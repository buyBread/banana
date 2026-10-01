#pragma once

#include <d3d9.h>

#include "treyarch/shared/dinkumware/map.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    // name-hash keyed D3D9 program registry; the original class name is not recovered
    struct shader_resource_manager {
        dinkumware::map<u32, IDirect3DVertexShader9*> vertex_programs;
        dinkumware::map<u32, IDirect3DPixelShader9*>  pixel_programs;

        IDirect3DVertexShader9** find_vertex_program(const char* name);
        IDirect3DPixelShader9**  find_pixel_program(const char* name);
    };

    ASSERT_SIZEOF  (shader_resource_manager,                 0x18);
    ASSERT_OFFSETOF(shader_resource_manager, pixel_programs, 0x0C);

    namespace references {
        inline util::memory_reference<shader_resource_manager*> shader_resource_manager { 0x010FC5C0 };
    } // references
} // treyarch
