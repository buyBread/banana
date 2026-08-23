#include "treyarch/ngl/d3d9/geometry_stream.hh"

using namespace treyarch;

void ngl::d3d9::geometry_stream::begin_submission() {
    u32 buffer_index = references::buffer_index.read() ^ 1;

    references::cursor.write(0);

    references::buffer_index.write(buffer_index);

    references::active_buffer.write(buffer_index ?
        references::buffer_0.read() : references::buffer_1.read());

    references::allocation_count.write(0);
    
    references::vertex_cursor.write(0);
    references::index_cursor .write(0);

    references::streamed_bytes       .write(0);
    references::stream_limit         .write(references::default_limit.read());
    references::stream_sequence      .write(1);
    references::stream_sequence_limit.write(references::default_limit.read());
}

u32 ngl::d3d9::geometry_stream::bytes_used() {
    return (u32)(references::streamed_bytes.read() >> 32);
}
