#pragma once

#include <Windows.h>
#include <d3d9.h>

#include "treyarch/ngl/init_list/init_list.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace d3d9 { namespace geometry_stream {
    struct cursor {
        u32 buffer_index;
        u32 byte_offset;
        u32 capacity;
    };

    struct segment;
    using segment_callback = void (__cdecl*)(segment* value, void* user_data);

    struct segment {
        cursor           allocation_start;
        cursor           allocation_end;
        u8               reserved_018[0x18];
        void*            mapped_data;
        u32              byte_count;
        u8               reserved_038[0x08];
        segment_callback callback;
        void*            user_data;
        u32              reserved_048;
        bool             ready;
        u8               reserved_04d[3];
    };

    void init();
    void begin_submission();
    u32 bytes_used();

    namespace references {
        inline util::memory_reference<CRITICAL_SECTION>        critical_section     { 0x011188B0 };
        inline util::memory_reference<IDirect3DVertexBuffer9*> active_buffer        { 0x011188C8 };
        inline util::memory_reference<IDirect3DVertexBuffer9*> secondary_buffer     { 0x011188CC };
        inline util::memory_reference<IDirect3DVertexBuffer9*> primary_buffer       { 0x011188D0 };
        inline util::memory_reference<segment*>                segments             { 0x011188D8 };
        inline util::memory_reference<u32>                     bytes_allocated      { 0x011188DC };
        inline util::memory_reference<u32>                     active_buffer_index  { 0x011188E0 };
        inline util::memory_reference<u32>                     segment_write_index  { 0x011188E4 };
        inline util::memory_reference<u32>                     segment_submit_index { 0x011188E8 };
        inline util::memory_reference<u32>                     segment_retire_index { 0x011188EC };
        inline util::memory_reference<cursor>                  allocation_cursor    { 0x01118930 };
        inline util::memory_reference<cursor>                  retirement_cursor    { 0x0111893C };
        inline util::memory_reference<u32>                     segment_count        { 0x00F530D4 };
        inline util::memory_reference<u32>                     buffer_size          { 0x00F530D8 };
        inline util::memory_reference<init_list_function>      init_list            { 0x00F531A0 };
    } // references

    ASSERT_SIZEOF  (cursor,               0x0C);
    ASSERT_OFFSETOF(cursor, buffer_index, 0x00);
    ASSERT_OFFSETOF(cursor, byte_offset,  0x04);
    ASSERT_OFFSETOF(cursor, capacity,     0x08);

    ASSERT_SIZEOF  (segment,                   0x50);
    ASSERT_OFFSETOF(segment, allocation_start, 0x00);
    ASSERT_OFFSETOF(segment, allocation_end,   0x0C);
    ASSERT_OFFSETOF(segment, mapped_data,      0x30);
    ASSERT_OFFSETOF(segment, byte_count,       0x34);
    ASSERT_OFFSETOF(segment, callback,         0x40);
    ASSERT_OFFSETOF(segment, user_data,        0x44);
    ASSERT_OFFSETOF(segment, ready,            0x4C);

    ASSERT_SIZEOF(CRITICAL_SECTION, 0x18);
}}}} // treyarch::ngl::d3d9::geometry_stream
