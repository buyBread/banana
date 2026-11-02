#pragma once

#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace nfl {
    struct nfd_driver;

    // NFL_BUFFER_MODE_*
    enum e_buffer_mode : i32 {
        buffer_mode_nobuffer = 1,
        buffer_mode_unaligned,
        buffer_mode_all,
        buffer_mode_vm
    };

    // NFD_IO_STATE_*
    enum e_nfd_io_state : i32 {
        nfd_io_state_idle,
        nfd_io_state_working,
        nfd_io_state_workdone,
        nfd_io_state_canceling,
        nfd_io_state_canceled,
        nfd_io_state_error
    };

    // NFD_ERROR_*
    enum e_nfd_error : i32 {
        nfd_error_noerror           = 0,
        nfd_error_failure           = 1,
        nfd_error_unaligned_access  = 3,
        nfd_error_invalid_media     = 4,
        nfd_error_invalid_state     = 5,
        nfd_error_invalid_filename  = 7,
        nfd_error_invalid_arguments = 8
    };

    struct nfd_init {
        i32         buffer_mode;
        e_nfd_error (*initialize)(nfd_driver* driver);
        e_nfd_error (*shutdown)();
    };

    struct nfd_media {
        u32         media_mask;
        e_nfd_error (*bind)(i32 media, const char* path, char* bound_path, i32 bound_path_size);
    };

    struct nfd_file_info {
        u32 unk_00; // the win32 driver writes 0
        u32 size;
    };

    struct nfd_file {
        u32         handle_size;
        u32         handle_alignment;
        e_nfd_error (*open)(void* handle, const char* path, u32 flags, u32 size);
        e_nfd_error (*close)(void* handle);
        e_nfd_error (*status)(void* handle, nfd_file_info* info);
        e_nfd_error (*handle)(void* handle, void** file_handle, u32* size);
    };

    struct nfd_io {
        e_nfd_error (*execute)(nfd_driver* driver, void* file_handle, i32 type, u32 offset, void* buffer, u32 size);
        e_nfd_error (*cancel)(void* file_handle);
        e_nfd_error (*update)();
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

    struct nfd_io_command {
        u32   file_size;
        u32   offset;
        void* file_handle;      // the file's slot in the handle space
        i32   request;
        void* buffer;
        u32   size;
        i32   type;             // 0 read, 1 write
        i32   read_buffer_slot;
    };

    struct disk_read_t {
        u32            aligned_offset;
        u32            bytes_completed;
        u32            aligned_size;
        nfd_io_command io_command;
    };

    struct nfd_driver {
        const char*    name;
        nfd_init*      init;
        nfd_media*     binding;
        nfd_file*      file_operations;
        nfd_io*        io_operations;
        nfd_buffer*    buffers_object;
        i32            chunk_decompress_jobs;
        disk_read_t    disk_read;
        e_nfd_io_state io_state;
    };

    struct platform_settings {
        u32 compression_type; // NFL_COMPRESSION_TYPE_LZO is 0
        u32 unk_04;
        u32 unk_08;
        u32 unk_0c;
        u32 unk_10;
        u32 unk_14;
        u32 unk_18;
        u32 buffer_size;
        u32 unk_20;
    };

    e_nfd_error nfd_io_execute(nfd_driver* driver, const nfd_io_command* command);
    void        nfd_io_complete(nfd_driver* driver, u32 bytes, i32 error);

    i32  nfd_find_unlocked_read_buffer(const nfd_driver* driver);
    void nfd_lock_read_buffer(nfd_driver* driver, i32 id, i32 slot);
    void nfd_unlock_read_buffer(nfd_driver* driver, i32 id, i32 slot);

    namespace references {
        inline util::memory_reference<nfd_driver**> nfs_drivers       { 0x00FB8534 };
        inline util::memory_reference<i32>          nfs_drivers_count { 0x00FB8530 };

        // the first of three rows, only this one is ever selected
        inline util::memory_reference<platform_settings>  default_platform_settings { 0x00FB8538 };
        inline util::memory_reference<platform_settings*> current_platform_settings { 0x01124728 };

        inline util::memory_reference<i32> alertable_wait { 0x01124640 };
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

    ASSERT_SIZEOF  (nfd_io_command,                   0x20);
    ASSERT_OFFSETOF(nfd_io_command, file_handle,      0x08);
    ASSERT_OFFSETOF(nfd_io_command, request,          0x0C);
    ASSERT_OFFSETOF(nfd_io_command, type,             0x18);
    ASSERT_OFFSETOF(nfd_io_command, read_buffer_slot, 0x1C);

    ASSERT_SIZEOF  (disk_read_t,                  0x2C);
    ASSERT_OFFSETOF(disk_read_t, bytes_completed, 0x04);
    ASSERT_OFFSETOF(disk_read_t, io_command,      0x0C);

    ASSERT_SIZEOF  (nfd_driver,                        0x4C);
    ASSERT_OFFSETOF(nfd_driver, init,                  0x04);
    ASSERT_OFFSETOF(nfd_driver, file_operations,       0x0C);
    ASSERT_OFFSETOF(nfd_driver, buffers_object,        0x14);
    ASSERT_OFFSETOF(nfd_driver, chunk_decompress_jobs, 0x18);
    ASSERT_OFFSETOF(nfd_driver, disk_read,             0x1C);
    ASSERT_OFFSETOF(nfd_driver, io_state,              0x48);

    ASSERT_SIZEOF  (platform_settings,             0x24);
    ASSERT_OFFSETOF(platform_settings, buffer_size, 0x1C);
}} // treyarch::nfl
