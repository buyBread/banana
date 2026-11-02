#include <cstring>

#include "treyarch/nfl/system.hh"
#include "treyarch/nfl/win32/driver.hh"
#include "treyarch/base/tl_system.hh"

using namespace treyarch;

// sub_A17940
nfl::e_nfd_error treyarch::nfl::nfd_win32_media_bind(i32 media, const char* path, char* bound_path, i32 bound_path_size) {
    if (media != 1 && media != 2)
        return nfd_error_invalid_media;

    bound_path[0] = 0;

    std::strcat(bound_path, path);

    return nfd_error_noerror;
}

// sub_A17D40
nfl::e_nfd_error treyarch::nfl::nfd_win32_initialize(nfd_driver* driver) {
    references::nfd_win32_io_work.get().io_event = CreateEventA(nullptr, FALSE, FALSE, nullptr);

    return nfd_error_noerror;
}

// sub_A17D60
nfl::e_nfd_error treyarch::nfl::nfd_win32_shutdown() {
    CloseHandle(references::nfd_win32_io_work.get().io_event);

    return nfd_error_noerror;
}

// sub_A17BD0
nfl::e_nfd_error treyarch::nfl::nfd_win32_file_open(void* handle, const char* path, u32 flags, u32 size) {
    HANDLE file = CreateFileA(path,
                              (flags & nfd_file_flag_read ? GENERIC_READ : 0) | (flags & nfd_file_flag_write ? GENERIC_WRITE : 0),
                              flags & (nfd_file_flag_read | nfd_file_flag_write),
                              nullptr,
                              flags & nfd_file_flag_create ? CREATE_ALWAYS : OPEN_EXISTING,
                              (flags & nfd_file_flag_write ? 0 : FILE_FLAG_NO_BUFFERING) | FILE_FLAG_OVERLAPPED,
                              nullptr);

    if (file == INVALID_HANDLE_VALUE)
        return nfd_error_failure;

    if (flags & nfd_file_flag_create) {
        LARGE_INTEGER distance;

        distance.QuadPart = size;

        SetFilePointerEx(file, distance, nullptr, FILE_BEGIN);
        SetEndOfFile(file);
        SetFilePointer(file, 0, nullptr, FILE_BEGIN);
    }

    *(HANDLE*)handle = file;

    return nfd_error_noerror;
}

// sub_A17C70
nfl::e_nfd_error treyarch::nfl::nfd_win32_file_close(void* handle) {
    return CloseHandle(*(HANDLE*)handle) ? nfd_error_noerror : nfd_error_failure;
}

// sub_A17C90
nfl::e_nfd_error treyarch::nfl::nfd_win32_file_status(void* handle, nfd_file_info* info) {
    info->size   = GetFileSize(*(HANDLE*)handle, nullptr);
    info->unk_00 = 0;

    return nfd_error_noerror;
}

// sub_A17CB0
nfl::e_nfd_error treyarch::nfl::nfd_win32_file_handle(void* handle, void** file_handle, u32* size) { // hands back the slot, not the HANDLE in it
    *file_handle = handle;
    *size        = 4;

    return nfd_error_noerror;
}

// sub_A17CD0
void __stdcall treyarch::nfl::nfd_win32_io_completion_routine(DWORD error_code, DWORD bytes_transferred, OVERLAPPED* overlapped) {
    nfd_win32_io_work_t &work = references::nfd_win32_io_work.get();

    nfd_io_complete(work.driver, bytes_transferred, error_code);

    work.overlapped.hEvent = nullptr;
}

// sub_A17D70
nfl::e_nfd_error treyarch::nfl::nfd_win32_io_execute(nfd_driver* driver, void* file_handle, i32 type, u32 offset, void* buffer, u32 size) {
    nfd_win32_io_work_t &work = references::nfd_win32_io_work.get();

    work.overlapped.hEvent     = work.io_event;
    work.driver                = driver;
    work.overlapped.Offset     = offset;
    work.overlapped.OffsetHigh = 0;

    BOOL ok;

    for (;;) {
        ok = type == 1 ?
            WriteFileEx(*(HANDLE*)file_handle, buffer, size, &work.overlapped, nfd_win32_io_completion_routine) :
            ReadFileEx (*(HANDLE*)file_handle, buffer, (size + 0x7FF) & ~0x7FF, &work.overlapped, nfd_win32_io_completion_routine);

        if (ok) {
            if (!references::always_report.read())
                return nfd_error_noerror;

            break;
        }

        if (GetLastError() != ERROR_HANDLE_EOF)
            break;

        // past the end of the file, ask again for what's left of it
        size = GetFileSize(*(HANDLE*)file_handle, nullptr) - offset;
    }

    tl_fatal("NFL: windows file error %d", GetLastError());

    return ok ? nfd_error_noerror : nfd_error_failure;
}

// sub_A17D00
nfl::e_nfd_error treyarch::nfl::nfd_win32_io_cancel(void* file_handle) {
    return CancelIo(*(HANDLE*)file_handle) ? nfd_error_noerror : nfd_error_failure;
}

// sub_A17D20
nfl::e_nfd_error treyarch::nfl::nfd_win32_io_update() { // completions run in here
    if (!references::alertable_wait.read())
        SleepEx(0, TRUE);

    return nfd_error_noerror;
}
