#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/work_buffers.hh"
#include "treyarch/ngl/list/arena.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/shared/memory/memory.hh"

using namespace treyarch;

void ngl::d3d9::replace_scratch_index_buffers(u32 maximum_index_count) {
    scratch_buffers &scratch = references::scratch_buffers.get();

    if (scratch.maximum_index_count) {
        scratch.index_buffer_0->Release();
        scratch.index_buffer_1->Release();
    }

    if (maximum_index_count) {
        IDirect3DDevice9* device = references::device.get();

        device->CreateIndexBuffer(maximum_index_count * sizeof(u16),
                                  0,
                                  D3DFMT_INDEX16,
                                  D3DPOOL_MANAGED,
                                  &scratch.index_buffer_0,
                                  nullptr);
        device->CreateIndexBuffer(maximum_index_count * sizeof(u16),
                                  0,
                                  D3DFMT_INDEX16,
                                  D3DPOOL_MANAGED,
                                  &scratch.index_buffer_1,
                                  nullptr);
    }

    scratch.maximum_index_count = maximum_index_count;
}

void ngl::d3d9::replace_scratch_vertex_buffers(u32 size) {
    scratch_buffers &scratch = references::scratch_buffers.get();

    if (scratch.vertex_buffer_size) {
        scratch.vertex_buffer_0->Release();
        scratch.vertex_buffer_1->Release();
    }

    if (size) {
        IDirect3DDevice9* device = references::device.get();

        device->CreateVertexBuffer(size,
                                   0,
                                   0,
                                   D3DPOOL_MANAGED,
                                   &scratch.vertex_buffer_0,
                                   nullptr);
        device->CreateVertexBuffer(size,
                                   0,
                                   0,
                                   D3DPOOL_MANAGED,
                                   &scratch.vertex_buffer_1,
                                   nullptr);
    }

    scratch.vertex_buffer_size = size;
}

void ngl::d3d9::replace_platform_work_buffer(u32 size) {
    void* &buffer      = references::platform_work_buffer.get();
    u32   &buffer_size = references::platform_work_buffer_size.get();

    if (buffer_size) {
        memory::free(buffer);

        buffer = nullptr;
    }

    if (size) {
        buffer = memory::allocate(size,
                                  0x80,
                                  memory::allocation_physical |
                                  memory::allocation_write_combine);
    }

    buffer_size = size;
}

void ngl::d3d9::provision_default_work_buffers() {
    list::arena_state &arena = list::references::arena.get();

    if (!arena.base)
        ngl::set_buffer_size(ngl::buffer_list_work, 0x40000, true, true);

    scratch_buffers &scratch = references::scratch_buffers.get();

    if (!scratch.maximum_index_count)
        ngl::set_buffer_size(ngl::buffer_scratch_index, 0x100000, true, true);

    if (!scratch.vertex_buffer_size)
        ngl::set_buffer_size(ngl::buffer_scratch_vertex, 0x800000, true, true);

    if (!references::platform_work_buffer_size.read())
        ngl::set_buffer_size(ngl::buffer_platform_work, 0, true, true);
}
