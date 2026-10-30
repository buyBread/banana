#pragma once

#include <d3d9.h>

#include "vc8/hash_map.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace ngl { namespace d3d9 { namespace shader_program_cache {
    template<typename T>
    using program_cache = vc8::hash_map<u32, T*>;

    using vertex_program_cache = program_cache<IDirect3DVertexShader9>;
    using pixel_program_cache  = program_cache<IDirect3DPixelShader9>;

    HRESULT create_vertex_program(const DWORD* bytecode, IDirect3DVertexShader9** output);
    HRESULT create_pixel_program(const DWORD* bytecode, IDirect3DPixelShader9** output);

    namespace references {
        inline util::memory_reference<vertex_program_cache> vertex_programs { 0x01117100 };
        inline util::memory_reference<pixel_program_cache>  pixel_programs  { 0x01117128 };
    } // references

    ASSERT_SIZEOF(vertex_program_cache,       0x28);
    ASSERT_SIZEOF(vertex_program_cache::node, 0x10);

    ASSERT_SIZEOF(pixel_program_cache,       0x28);
    ASSERT_SIZEOF(pixel_program_cache::node, 0x10);
}}}} // treyarch::ngl::d3d9::shader_program_cache
