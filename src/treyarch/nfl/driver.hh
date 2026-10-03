#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace nfl {
    struct driver;

    struct driver_initialization {
        i32 buffer_mode;
        i32 (__cdecl* initialize)(driver* driver);
        i32 (__cdecl* shutdown)();
    };

    struct driver_binding {
        u32 media_mask;
        i32 (__cdecl* bind)(i32 media, const char* path, char* bound_path, u32 bound_path_size);
    };

    struct driver_file_info {
        u32 unk_00; // the win32 driver writes 0
        u32 size;
    };

    struct driver_file_operations {
        u32   handle_size;
        u32   handle_alignment;
        i32   (__cdecl* open)(u8* handle, const char* path, u32 flags, u32 size); // 0 on success
        i32   (__cdecl* close)(u8* handle);
        i32   (__cdecl* get_info)(u8* handle, driver_file_info* info);            // 0 on success
        void* get_native_handle;
    };

    struct driver_io_operations {
        void* execute;
        void* cancel;
        void* poll;
    };

    struct driver_buffer {
        i32 locked_by; // request id
        u8* data;
    };

    struct driver_buffers_object {
        u32           address_alignment;
        u32           size_alignment;
        u32           offset_alignment;
        u32           buffer_size;
        driver_buffer buffers[2];
    };

    struct driver {
        const char*             name;
        driver_initialization*  initialization;
        driver_binding*         binding;
        driver_file_operations* file_operations;
        driver_io_operations*   io_operations;
        driver_buffers_object*  buffers_object;
        i32                     chunk_jobs;
        u8                      reserved_01c[0x2C];
        i32                     io_state;
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
        inline util::memory_reference<driver**> drivers      { 0x00FB8534 };
        inline util::memory_reference<i32>      driver_count { 0x00FB8530 };

        // the first of three rows, only this one is ever selected
        inline util::memory_reference<platform_settings>  default_platform_settings { 0x00FB8538 };
        inline util::memory_reference<platform_settings*> current_platform_settings { 0x01124728 };
    } // references

    ASSERT_SIZEOF  (driver_file_info,       0x08);
    ASSERT_OFFSETOF(driver_file_info, size, 0x04);

    ASSERT_SIZEOF  (driver_initialization,             0x0C);
    ASSERT_SIZEOF  (driver_binding,                    0x08);
    ASSERT_SIZEOF  (driver_file_operations,            0x18);
    ASSERT_SIZEOF  (driver_io_operations,              0x0C);

    ASSERT_SIZEOF  (driver_buffers_object,              0x20);
    ASSERT_OFFSETOF(driver_buffers_object, buffer_size, 0x0C);
    ASSERT_OFFSETOF(driver_buffers_object, buffers,     0x10);

    ASSERT_SIZEOF  (driver,                  0x4C);
    ASSERT_OFFSETOF(driver, file_operations, 0x0C);
    ASSERT_OFFSETOF(driver, buffers_object,  0x14);
    ASSERT_OFFSETOF(driver, chunk_jobs,      0x18);
    ASSERT_OFFSETOF(driver, io_state,        0x48);

    ASSERT_SIZEOF  (platform_settings,              0x24);
    ASSERT_OFFSETOF(platform_settings, buffer_size, 0x1C);
}} // treyarch::nfl
