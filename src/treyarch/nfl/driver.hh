#pragma once

#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace nfl {
    struct nfd_driver;

    // NFD_IO_STATE_*
    enum e_nfd_io_state : i32 {
        nfd_io_state_idle,
        nfd_io_state_working,
        nfd_io_state_workdone,
        nfd_io_state_canceling,
        nfd_io_state_canceled,
        nfd_io_state_error
    };

    struct nfd_init {
        i32 buffer_mode;
        i32 (__cdecl* initialize)(nfd_driver* driver);
        i32 (__cdecl* shutdown)();
    };

    struct nfd_media {
        u32 media_mask;
        i32 (__cdecl* bind)(i32 media, const char* path, char* bound_path, u32 bound_path_size);
    };

    struct nfd_file_info {
        u32 unk_00; // the win32 driver writes 0
        u32 size;
    };

    struct nfd_file {
        u32   handle_size;
        u32   handle_alignment;
        i32   (__cdecl* open)(u8* handle, const char* path, u32 flags, u32 size); // 0 on success
        i32   (__cdecl* close)(u8* handle);
        i32   (__cdecl* get_info)(u8* handle, nfd_file_info* info);               // 0 on success
        void* get_native_handle;
    };

    struct nfd_io {
        void* execute;
        void* cancel;
        void* poll;
    };

    struct nfd_buffer {
        struct buffer {
            i32 locked_by; // request id
            u8* data;
        };

        u32    address_alignment;
        u32    size_alignment;
        u32    offset_alignment;
        u32    buffer_size;
        buffer buffers[2];
    };

    struct nfd_driver {
        const char*    name;
        nfd_init*      init;
        nfd_media*     binding;
        nfd_file*      file_operations;
        nfd_io*        io_operations;
        nfd_buffer*    buffer;
        i32            chunk_jobs;
        u8             reserved_01c[0x2C];
        e_nfd_io_state io_state;
    };

    struct platform_settings {
        u32 unk_00;
        u32 unk_04;
        u32 unk_08;
        u32 unk_0c;
        u32 unk_10;
        u32 unk_14;
        u32 unk_18;
        u32 buffer_size;
        u32 unk_20;
    };

    namespace references {
        inline util::memory_reference<nfd_driver**> nfs_drivers       { 0x00FB8534 };
        inline util::memory_reference<i32>          nfs_drivers_count { 0x00FB8530 };

        // the first of three rows, only this one is ever selected
        inline util::memory_reference<platform_settings>  default_platform_settings { 0x00FB8538 };
        inline util::memory_reference<platform_settings*> current_platform_settings { 0x01124728 };
    } // references

    ASSERT_SIZEOF  (nfd_file_info,       0x08);
    ASSERT_OFFSETOF(nfd_file_info, size, 0x04);

    ASSERT_SIZEOF  (nfd_init,  0x0C);
    ASSERT_SIZEOF  (nfd_media, 0x08);
    ASSERT_SIZEOF  (nfd_file,  0x18);
    ASSERT_SIZEOF  (nfd_io,    0x0C);

    ASSERT_SIZEOF  (nfd_buffer,              0x20);
    ASSERT_OFFSETOF(nfd_buffer, buffer_size, 0x0C);
    ASSERT_OFFSETOF(nfd_buffer, buffers,     0x10);

    ASSERT_SIZEOF  (nfd_driver,                  0x4C);
    ASSERT_OFFSETOF(nfd_driver, init,            0x04);
    ASSERT_OFFSETOF(nfd_driver, file_operations, 0x0C);
    ASSERT_OFFSETOF(nfd_driver, buffer,          0x14);
    ASSERT_OFFSETOF(nfd_driver, chunk_jobs,      0x18);
    ASSERT_OFFSETOF(nfd_driver, io_state,        0x48);

    ASSERT_SIZEOF  (platform_settings,              0x24);
    ASSERT_OFFSETOF(platform_settings, buffer_size, 0x1C);
}} // treyarch::nfl
