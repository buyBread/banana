#pragma once

#include <d3d9.h>

#include "treyarch/shared/dinkumware/hash_map.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace d3d9 { namespace shader_program_cache {
    struct shader_crc_traits {
        u32 operator()(const u32 &crc) const noexcept;

        bool less(const u32 &left, const u32 &right) const noexcept {
            return left < right;
        }
    };

    template<typename T>
    using program_pair = dinkumware::hash_pair<u32, T*>;

    template<typename T>
    using program_node = dinkumware::list_node<program_pair<T>>;

    template<typename T>
    using program_cache = dinkumware::hash_map<u32, T*, shader_crc_traits>;

    template<typename T>
    struct program_cache_layout {
        u8                traits;
        u8                reserved_001[3];
        u32               list_allocator_state;
        program_node<T>*  sentinel;
        u32               size;
        u32               vector_allocator_state;
        program_node<T>** bucket_begin;
        program_node<T>** bucket_end;
        program_node<T>** bucket_capacity;
        u32               mask;
        u32               bucket_count;
    };

    using vertex_program_pair  = program_pair<IDirect3DVertexShader9>;
    using vertex_program_node  = program_node<IDirect3DVertexShader9>;
    using vertex_program_cache = program_cache<IDirect3DVertexShader9>;
    using vertex_cache_layout  = program_cache_layout<IDirect3DVertexShader9>;

    using pixel_program_node  = program_node<IDirect3DPixelShader9>;
    using pixel_program_cache = program_cache<IDirect3DPixelShader9>;

    HRESULT create_vertex_program(const DWORD* bytecode, IDirect3DVertexShader9** output);
    HRESULT create_pixel_program(const DWORD* bytecode, IDirect3DPixelShader9** output);

    namespace references {
        inline util::memory_reference<vertex_program_cache> vertex_programs { 0x01117100 };
        inline util::memory_reference<pixel_program_cache>  pixel_programs  { 0x01117128 };
    } // references

    ASSERT_SIZEOF  (vertex_program_pair,        0x08);
    ASSERT_OFFSETOF(vertex_program_pair, key,   0x00);
    ASSERT_OFFSETOF(vertex_program_pair, value, 0x04);

    ASSERT_SIZEOF  (vertex_program_node,           0x10);
    ASSERT_OFFSETOF(vertex_program_node, next,     0x00);
    ASSERT_OFFSETOF(vertex_program_node, previous, 0x04);
    ASSERT_OFFSETOF(vertex_program_node, value,    0x08);

    ASSERT_SIZEOF(vertex_program_cache, 0x28);

    ASSERT_SIZEOF  (vertex_cache_layout,                         0x28);
    ASSERT_OFFSETOF(vertex_cache_layout, traits,                 0x00);
    ASSERT_OFFSETOF(vertex_cache_layout, list_allocator_state,   0x04);
    ASSERT_OFFSETOF(vertex_cache_layout, sentinel,               0x08);
    ASSERT_OFFSETOF(vertex_cache_layout, size,                   0x0C);
    ASSERT_OFFSETOF(vertex_cache_layout, vector_allocator_state, 0x10);
    ASSERT_OFFSETOF(vertex_cache_layout, bucket_begin,           0x14);
    ASSERT_OFFSETOF(vertex_cache_layout, bucket_end,             0x18);
    ASSERT_OFFSETOF(vertex_cache_layout, bucket_capacity,        0x1C);
    ASSERT_OFFSETOF(vertex_cache_layout, mask,                   0x20);
    ASSERT_OFFSETOF(vertex_cache_layout, bucket_count,           0x24);

    ASSERT_SIZEOF(pixel_program_node, 0x10);

    ASSERT_SIZEOF(pixel_program_cache, 0x28);

    ASSERT_ALIGNOF(vertex_program_cache, vertex_cache_layout);
}}}} // treyarch::ngl::d3d9::shader_program_cache
