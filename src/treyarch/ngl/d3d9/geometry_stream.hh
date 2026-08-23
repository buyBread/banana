#pragma once

#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace d3d9 { namespace geometry_stream {
    void begin_submission();
    u32  bytes_used();

    namespace references {
        inline util::memory_reference<u32> active_buffer        { 0x011188C8 };
        inline util::memory_reference<u32> buffer_0             { 0x011188CC };
        inline util::memory_reference<u32> buffer_1             { 0x011188D0 };
        inline util::memory_reference<u32> cursor               { 0x011188DC };
        inline util::memory_reference<u32> buffer_index         { 0x011188E0 };
        inline util::memory_reference<u32> allocation_count     { 0x011188E4 };
        inline util::memory_reference<u32> vertex_cursor        { 0x011188E8 };
        inline util::memory_reference<u32> index_cursor         { 0x011188EC };
        inline util::memory_reference<u64> streamed_bytes       { 0x01118930 };
        inline util::memory_reference<u32> stream_limit         { 0x01118938 };
        inline util::memory_reference<u64> stream_sequence      { 0x0111893C };
        inline util::memory_reference<u32> stream_sequence_limit { 0x01118944 };
        inline util::memory_reference<u32> default_limit        { 0x00F530D8 };
    } // references
}}}} // treyarch::ngl::d3d9::geometry_stream
