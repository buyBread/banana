#pragma once

#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // TL_MEMALLOC_*
    enum e_tl_memalloc_flags : u32 {
        tl_memalloc_temp         = 0x00000001,
        tl_memalloc_canfail      = 0x00000002,
        tl_memalloc_fixed        = 0x00000004,
        tl_memalloc_physical     = 0x00010000,
        tl_memalloc_writecombine = 0x00020000
    };

    struct tl_file_buf {
        u8* buf;
        u32 size;
        u32 user_data;
    };

    struct tl_system_callbacks {
        bool  (__cdecl* read_file)(const char* file_name, tl_file_buf* file, u32 align, u32 flags);
        void  (__cdecl* release_file)(tl_file_buf* file);
        void  (__cdecl* critical_error)(const char* text);
        void  (__cdecl* warning)(const char* text);
        void  (__cdecl* debug_print)(const char* text);
        void* (__cdecl* mem_alloc)(u32 size, u32 align, u32 flags);
        void* (__cdecl* mem_realloc)(void* ptr, u32 size, u32 align, u32 flags);
        void  (__cdecl* mem_free)(void* ptr);
    };

    void  tl_fatal(const char* format, ...);
    void* tl_mem_alloc(u32 size, u32 align, u32 flags);
    void  tl_mem_free(void* ptr);
    void  tl_stack_range_init();

    namespace references {
        inline util::memory_reference<tl_system_callbacks> tl_cur_system_callbacks { 0x01115A20 };

        inline util::memory_reference<i32> tl_mem_alloc_counter { 0x01115A40 };

        inline util::memory_reference<void*> tl_stack_begin { 0x01115B50 };
        inline util::memory_reference<void*> tl_stack_end   { 0x01115B54 };
    } // references

    ASSERT_SIZEOF  (tl_file_buf,            0x0C);
    ASSERT_OFFSETOF(tl_file_buf, user_data, 0x08);

    ASSERT_SIZEOF  (tl_system_callbacks,                 0x20);
    ASSERT_OFFSETOF(tl_system_callbacks, critical_error, 0x08);
    ASSERT_OFFSETOF(tl_system_callbacks, mem_alloc,      0x14);
    ASSERT_OFFSETOF(tl_system_callbacks, mem_free,       0x1C);
} // treyarch
