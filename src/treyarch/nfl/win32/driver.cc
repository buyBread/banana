#include <cstring>

#include "treyarch/nfl/nfl.hh"
#include "treyarch/nfl/system.hh"
#include "treyarch/nfl/win32/driver.hh"
#include "treyarch/base/tl_system.hh"

using namespace treyarch;

// sub_A17940
nfl::driver::e_error treyarch::nfl::driver::win32_media_bind(i32 media, const char* path, char* bound_path, i32 bound_path_size) {
    if (media != 1 && media != 2)
        return error_invalid_media;

    bound_path[0] = 0;

    std::strcat(bound_path, path);

    return error_noerror;
}

// sub_A17D40
nfl::driver::e_error treyarch::nfl::driver::win32_initialize(driver_t* driver) {
    references::win32_io_work.get().io_event = CreateEventA(nullptr, FALSE, FALSE, nullptr);

    return error_noerror;
}

// sub_A17D60
nfl::driver::e_error treyarch::nfl::driver::win32_shutdown() {
    CloseHandle(references::win32_io_work.get().io_event);

    return error_noerror;
}

// sub_A17BD0
nfl::driver::e_error treyarch::nfl::driver::win32_file_open(void* handle, const char* path, u32 flags, u32 size) {
    HANDLE file = CreateFileA(path,
                              (flags & file_flag_read ? GENERIC_READ : 0) | (flags & file_flag_write ? GENERIC_WRITE : 0),
                              flags & (file_flag_read | file_flag_write),
                              nullptr,
                              flags & file_flag_create ? CREATE_ALWAYS : OPEN_EXISTING,
                              (flags & file_flag_write ? 0 : FILE_FLAG_NO_BUFFERING) | FILE_FLAG_OVERLAPPED,
                              nullptr);

    if (file == INVALID_HANDLE_VALUE) {
        nfl::log.err("failed to open \"{}\" -- win32 error {}", path, GetLastError());

        return error_failure;
    }

    if (flags & file_flag_create) {
        LARGE_INTEGER distance;

        distance.QuadPart = size;

        SetFilePointerEx(file, distance, nullptr, FILE_BEGIN);
        SetEndOfFile(file);
        SetFilePointer(file, 0, nullptr, FILE_BEGIN);
    }

    *(HANDLE*)handle = file;

    return error_noerror;
}

// sub_A17C70
nfl::driver::e_error treyarch::nfl::driver::win32_file_close(void* handle) {
    return CloseHandle(*(HANDLE*)handle) ? error_noerror : error_failure;
}

// sub_A17C90
nfl::driver::e_error treyarch::nfl::driver::win32_file_status(void* handle, file_info_t* info) {
    info->size   = GetFileSize(*(HANDLE*)handle, nullptr);
    info->unk_00 = 0;

    return error_noerror;
}

// sub_A17CB0
nfl::driver::e_error treyarch::nfl::driver::win32_file_handle(void* handle, void** file_handle, u32* size) { // hands back the slot, not the HANDLE in it
    *file_handle = handle;
    *size        = 4;

    return error_noerror;
}

// sub_A17CD0
void __stdcall treyarch::nfl::driver::win32_io_completion_routine(DWORD error_code, DWORD bytes_transferred, OVERLAPPED* overlapped) {
    win32_io_work_t &work = references::win32_io_work.get();

    io_complete(work.driver, bytes_transferred, error_code);

    work.overlapped.hEvent = nullptr;
}

// sub_A17D70
nfl::driver::e_error treyarch::nfl::driver::win32_io_execute(driver_t* driver, void* file_handle, i32 type, u32 offset, void* buffer, u32 size) {
    win32_io_work_t &work = references::win32_io_work.get();

    work.overlapped.hEvent     = work.io_event;
    work.driver                = driver;
    work.overlapped.Offset     = offset;
    work.overlapped.OffsetHigh = 0;

    BOOL ok;

    for (;;) {
        ok = type == 1 ?
            WriteFileEx(*(HANDLE*)file_handle, buffer, size, &work.overlapped, win32_io_completion_routine) :
            ReadFileEx (*(HANDLE*)file_handle, buffer, (size + 0x7FF) & ~0x7FF, &work.overlapped, win32_io_completion_routine);

        if (ok) {
            if (!nfl::references::always_report.read())
                return error_noerror;

            break;
        }

        if (GetLastError() != ERROR_HANDLE_EOF)
            break;

        // past the end of the file, ask again for what's left of it
        const u32 remaining = GetFileSize(*(HANDLE*)file_handle, nullptr) - offset;

        if (remaining != size)
            nfl::log.wrn("read past the end of \"{}\" -- offset 0x{:X}, size 0x{:X} -> 0x{:X}",
                         system::file_handle_path(file_handle),
                         offset,
                         size,
                         remaining);

        size = remaining;
    }

    // the log can clobber the last error
    const u32 last_error = GetLastError();

    nfl::log.err("read failed \"{}\" -- offset 0x{:X}, size 0x{:X}, win32 error {}",
                 system::file_handle_path(file_handle),
                 offset,
                 size,
                 last_error);

    tl_fatal("NFL: windows file error %d", last_error);

    return ok ? error_noerror : error_failure;
}

// sub_A17D00
nfl::driver::e_error treyarch::nfl::driver::win32_io_cancel(void* file_handle) {
    return CancelIo(*(HANDLE*)file_handle) ? error_noerror : error_failure;
}

// sub_A17D20
nfl::driver::e_error treyarch::nfl::driver::win32_io_update() { // completions run in here
    if (!nfl::references::alertable_wait.read())
        SleepEx(0, TRUE);

    return error_noerror;
}
