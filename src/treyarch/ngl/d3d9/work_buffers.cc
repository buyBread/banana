#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/work_buffers.hh"
#include "treyarch/ngl/list/arena.hh"
#include "treyarch/shared/memory/memory.hh"

using namespace treyarch;

void ngl::d3d9::initialize_work_buffers() {
    list::arena_state &list = list::references::arena.get();

    if (!list.base) {
        wait_for_rendering();

        list.base     = (u8*)memory::allocate(0x40000, 8, 0);
        list.capacity = 0x40000;
    }

    scratch_buffers   &scratch = references::scratch_buffers.get();
    IDirect3DDevice9*  device  = references::device.get();

    if (!scratch.maximum_index_count) {
        wait_for_rendering();

        device->CreateIndexBuffer(0x200000,
                                  0,
                                  D3DFMT_INDEX16,
                                  D3DPOOL_MANAGED,
                                  &scratch.index_buffer_0,
                                  nullptr);
        device->CreateIndexBuffer(0x200000,
                                  0,
                                  D3DFMT_INDEX16,
                                  D3DPOOL_MANAGED,
                                  &scratch.index_buffer_1,
                                  nullptr);

        scratch.maximum_index_count = 0x100000;
    }

    if (!scratch.vertex_buffer_size) {
        wait_for_rendering();

        device->CreateVertexBuffer(0x800000,
                                   0,
                                   0,
                                   D3DPOOL_MANAGED,
                                   &scratch.vertex_buffer_0,
                                   nullptr);
        device->CreateVertexBuffer(0x800000,
                                   0,
                                   0,
                                   D3DPOOL_MANAGED,
                                   &scratch.vertex_buffer_1,
                                   nullptr);

        scratch.vertex_buffer_size = 0x800000;
    }

    if (!references::scratch_mesh_size.read()) {
        wait_for_rendering();

        references::scratch_mesh_size.write(0);
    }
}
