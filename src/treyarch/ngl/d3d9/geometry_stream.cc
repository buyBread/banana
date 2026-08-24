#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/geometry_stream.hh"
#include "treyarch/shared/memory/memory.hh"

using namespace treyarch;

void ngl::d3d9::geometry_stream::init() {
    u32 buffer_size = references::buffer_size.read();

    references::active_buffer_index.write(0);

    IDirect3DDevice9* device = d3d9::references::device.read();

    device->CreateVertexBuffer(buffer_size,
                               0,
                               0,
                               D3DPOOL_MANAGED,
                               &references::primary_buffer.get(),
                               nullptr);

    device->CreateVertexBuffer(buffer_size,
                               0,
                               0,
                               D3DPOOL_MANAGED,
                               &references::secondary_buffer.get(),
                               nullptr);

    cursor &allocation_cursor = references::allocation_cursor.get();
    allocation_cursor.buffer_index = 0;
    allocation_cursor.byte_offset = 0;
    allocation_cursor.capacity = buffer_size;

    cursor &retirement_cursor = references::retirement_cursor.get();
    retirement_cursor.buffer_index = 1;
    retirement_cursor.byte_offset = 0;
    retirement_cursor.capacity = buffer_size;

    references::active_buffer.write(references::primary_buffer.read());

    u32 segment_count = references::segment_count.read();
    references::segments.write((segment*)memory::allocate(sizeof(segment) * segment_count, 0x80, 0));

    references::segment_write_index.write(0);
    references::segment_submit_index.write(0);
    references::segment_retire_index.write(0);

    InitializeCriticalSection(&references::critical_section.get());
}

void ngl::d3d9::geometry_stream::begin_submission() {
    references::bytes_allocated.write(0);

    u32 active_buffer_index = references::active_buffer_index.read() ^ 1;

    references::active_buffer_index.write(active_buffer_index);

    references::active_buffer.write(active_buffer_index ?
        references::secondary_buffer.read() : references::primary_buffer.read());

    references::segment_write_index.write(0);
    references::segment_submit_index.write(0);
    references::segment_retire_index.write(0);

    u32 buffer_size = references::buffer_size.read();

    cursor &allocation_cursor = references::allocation_cursor.get();
    allocation_cursor.buffer_index = 0;
    allocation_cursor.byte_offset = 0;
    allocation_cursor.capacity = buffer_size;

    cursor &retirement_cursor = references::retirement_cursor.get();
    retirement_cursor.buffer_index = 1;
    retirement_cursor.byte_offset = 0;
    retirement_cursor.capacity = buffer_size;
}

u32 ngl::d3d9::geometry_stream::bytes_used() {
    return references::allocation_cursor.get().byte_offset;
}
