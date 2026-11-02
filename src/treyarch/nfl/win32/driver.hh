#pragma once

#include <windows.h>

#include "treyarch/nfl/driver.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace nfl {
    namespace driver {
        struct win32_io_work_t {
            OVERLAPPED overlapped;
            HANDLE     io_event;
            driver_t*  driver;
        };

        e_error win32_media_bind(i32 media, const char* path, char* bound_path, i32 bound_path_size);

        e_error win32_initialize(driver_t* driver);
        e_error win32_shutdown();

        e_error win32_file_open  (void* handle, const char* path, u32 flags, u32 size);
        e_error win32_file_close (void* handle);
        e_error win32_file_status(void* handle, file_info_t* info);
        e_error win32_file_handle(void* handle, void** file_handle, u32* size);

        e_error win32_io_execute(driver_t* driver, void* file_handle, i32 type, u32 offset, void* buffer, u32 size);
        e_error win32_io_cancel (void* file_handle);
        e_error win32_io_update ();

        void __stdcall win32_io_completion_routine(DWORD error_code, DWORD bytes_transferred, OVERLAPPED* overlapped);

        inline init_t win32_init = { buffer_mode_unaligned, win32_initialize, win32_shutdown };

        inline media_t win32_media = { 3, win32_media_bind };

        inline file_t win32_file = { 4,
                                     4,
                                     win32_file_open,
                                     win32_file_close,
                                     win32_file_status,
                                     win32_file_handle };

        inline io_t win32_io = { win32_io_execute, win32_io_cancel, win32_io_update };

        inline buffer_t win32_buffer = { 4, 0x1000, 0x800, 0 };

        inline driver_t win32_driver = { "PC(Win32)",
                                         &win32_init,
                                         &win32_media,
                                         &win32_file,
                                         &win32_io,
                                         &win32_buffer };

        inline win32_io_work_t win32_io_work;

        // nothing ever writes it
        inline bool always_report;

        ASSERT_SIZEOF  (win32_io_work_t,           0x1C);
        ASSERT_OFFSETOF(win32_io_work_t, io_event, 0x14);
        ASSERT_OFFSETOF(win32_io_work_t, driver,   0x18);
    } // driver

    namespace system {
        inline driver::driver_t* win32_drivers[] = { &driver::win32_driver };
    } // system
}} // treyarch::nfl
