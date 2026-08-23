#pragma once

#include <d3d9.h>

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace d3d9 {
    struct scratch_buffers {
        IDirect3DVertexBuffer9* vertex_buffer_1;
        IDirect3DIndexBuffer9*  index_buffer_1;
        IDirect3DIndexBuffer9*  index_buffer_0;
        IDirect3DVertexBuffer9* vertex_buffer_0;
        u32                     maximum_index_count;
        u32                     vertex_buffer_size;
    };

    void provision_default_work_buffers();
    
    void replace_scratch_index_buffers(u32 maximum_index_count);
    void replace_scratch_vertex_buffers(u32 size);
    void replace_platform_work_buffer(u32 size);

    namespace references {
        inline util::memory_reference<scratch_buffers> scratch_buffers           { 0x011185A0 };
        inline util::memory_reference<void*>           platform_work_buffer      { 0x011187F8 };
        inline util::memory_reference<u32>             platform_work_buffer_size { 0x01118810 };
    } // references

    ASSERT_SIZEOF  (scratch_buffers,                      0x18);
    ASSERT_OFFSETOF(scratch_buffers, vertex_buffer_1,     0x00);
    ASSERT_OFFSETOF(scratch_buffers, index_buffer_1,      0x04);
    ASSERT_OFFSETOF(scratch_buffers, index_buffer_0,      0x08);
    ASSERT_OFFSETOF(scratch_buffers, vertex_buffer_0,     0x0C);
    ASSERT_OFFSETOF(scratch_buffers, maximum_index_count, 0x10);
    ASSERT_OFFSETOF(scratch_buffers, vertex_buffer_size,  0x14);
}}} // treyarch::ngl::d3d9
