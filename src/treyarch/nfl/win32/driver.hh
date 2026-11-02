#pragma once

#include <windows.h>

#include "treyarch/nfl/driver.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace nfl {
    struct nfd_win32_io_work_t {
        OVERLAPPED  overlapped;
        HANDLE      io_event;
        nfd_driver* driver;
    };

    e_nfd_error nfd_win32_media_bind(i32 media, const char* path, char* bound_path, i32 bound_path_size);

    e_nfd_error nfd_win32_initialize(nfd_driver* driver);
    e_nfd_error nfd_win32_shutdown();

    e_nfd_error nfd_win32_file_open  (void* handle, const char* path, u32 flags, u32 size);
    e_nfd_error nfd_win32_file_close (void* handle);
    e_nfd_error nfd_win32_file_status(void* handle, nfd_file_info* info);
    e_nfd_error nfd_win32_file_handle(void* handle, void** file_handle, u32* size);

    e_nfd_error nfd_win32_io_execute(nfd_driver* driver, void* file_handle, i32 type, u32 offset, void* buffer, u32 size);
    e_nfd_error nfd_win32_io_cancel (void* file_handle);
    e_nfd_error nfd_win32_io_update ();

    void __stdcall nfd_win32_io_completion_routine(DWORD error_code, DWORD bytes_transferred, OVERLAPPED* overlapped);

    inline nfd_init nfd_win32_init = { 2, nfd_win32_initialize, nfd_win32_shutdown };

    inline nfd_media nfd_win32_media = { 3, nfd_win32_media_bind };

    inline nfd_file nfd_win32_file = { 4,
                                       4,
                                       nfd_win32_file_open,
                                       nfd_win32_file_close,
                                       nfd_win32_file_status,
                                       nfd_win32_file_handle };

    inline nfd_io nfd_win32_io = { nfd_win32_io_execute, nfd_win32_io_cancel, nfd_win32_io_update };

    inline nfd_buffer nfd_win32_buffer = { 4, 0x1000, 0x800, 0 };

    inline nfd_driver nfd_win32_driver = { "PC(Win32)",
                                           &nfd_win32_init,
                                           &nfd_win32_media,
                                           &nfd_win32_file,
                                           &nfd_win32_io,
                                           &nfd_win32_buffer };

    inline nfd_driver* nfs_win32_drivers[] = { &nfd_win32_driver };

    namespace references {
        inline util::memory_reference<nfd_win32_io_work_t> nfd_win32_io_work { 0x01124734 };

        inline util::memory_reference<bool> always_report { 0x00FC2F88 };
    } // references

    ASSERT_SIZEOF  (nfd_win32_io_work_t,           0x1C);
    ASSERT_OFFSETOF(nfd_win32_io_work_t, io_event, 0x14);
    ASSERT_OFFSETOF(nfd_win32_io_work_t, driver,   0x18);
}} // treyarch::nfl
