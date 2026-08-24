#pragma once

#include <d3d9.h>

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace d3d9 { namespace shader_program_cache {
    template<typename T>
    struct program_node {
        program_node* next;
        program_node* previous;
        u32           crc;
        T*            program;
    };

    template<typename T>
    struct program_list {
        u32              allocator_state;
        program_node<T>* sentinel;
        u32              size;
    };

    template<typename T>
    struct bucket_vector {
        u32               allocator_state;
        program_node<T>** begin;
        program_node<T>** end;
        program_node<T>** capacity;
    };

    template<typename T>
    struct program_cache {
        u8               traits;
        u8               reserved_001[3];
        program_list<T>  programs;
        bucket_vector<T> buckets;
        u32              mask;
        u32              bucket_count;
    };

    using vertex_program_node  = program_node<IDirect3DVertexShader9>;
    using pixel_program_node   = program_node<IDirect3DPixelShader9>;
    using vertex_program_list  = program_list<IDirect3DVertexShader9>;
    using vertex_bucket_vector = bucket_vector<IDirect3DVertexShader9>;
    using vertex_program_cache = program_cache<IDirect3DVertexShader9>;
    using pixel_program_cache  = program_cache<IDirect3DPixelShader9>;

    HRESULT create_vertex_program(const DWORD* bytecode, IDirect3DVertexShader9** output);
    HRESULT create_pixel_program(const DWORD* bytecode, IDirect3DPixelShader9** output);

    namespace references {
        inline util::memory_reference<vertex_program_cache> vertex_programs { 0x01117100 };
        inline util::memory_reference<pixel_program_cache>  pixel_programs  { 0x01117128 };
    } // references

    ASSERT_SIZEOF  (vertex_program_node,          0x10);
    ASSERT_OFFSETOF(vertex_program_node, next,    0x00);
    ASSERT_OFFSETOF(vertex_program_node, previous, 0x04);
    ASSERT_OFFSETOF(vertex_program_node, crc,     0x08);
    ASSERT_OFFSETOF(vertex_program_node, program, 0x0C);

    ASSERT_SIZEOF  (pixel_program_node,          0x10);
    ASSERT_OFFSETOF(pixel_program_node, program, 0x0C);

    ASSERT_SIZEOF  (vertex_program_list,           0x0C);
    ASSERT_OFFSETOF(vertex_program_list, sentinel, 0x04);
    ASSERT_OFFSETOF(vertex_program_list, size,     0x08);

    ASSERT_SIZEOF  (vertex_bucket_vector,           0x10);
    ASSERT_OFFSETOF(vertex_bucket_vector, begin,    0x04);
    ASSERT_OFFSETOF(vertex_bucket_vector, end,      0x08);
    ASSERT_OFFSETOF(vertex_bucket_vector, capacity, 0x0C);

    ASSERT_SIZEOF  (vertex_program_cache,               0x28);
    ASSERT_OFFSETOF(vertex_program_cache, traits,       0x00);
    ASSERT_OFFSETOF(vertex_program_cache, programs,     0x04);
    ASSERT_OFFSETOF(vertex_program_cache, buckets,      0x10);
    ASSERT_OFFSETOF(vertex_program_cache, mask,         0x20);
    ASSERT_OFFSETOF(vertex_program_cache, bucket_count, 0x24);

    ASSERT_SIZEOF(pixel_program_cache, 0x28);
}}}} // treyarch::ngl::d3d9::shader_program_cache
