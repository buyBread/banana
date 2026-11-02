#pragma once

#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace nfl {
    // NFL_BUFFER_MODE_*
    enum e_buffer_mode : i32 {
        buffer_mode_nobuffer = 1,
        buffer_mode_unaligned,
        buffer_mode_all,
        buffer_mode_vm
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

    namespace driver {
        struct driver_t;

        // NFD_IO_STATE_*
        enum e_io_state : i32 {
            io_state_idle,
            io_state_working,
            io_state_workdone,
            io_state_canceling,
            io_state_canceled,
            io_state_error
        };

        // NFD_ERROR_*
        enum e_error : i32 {
            error_noerror           = 0,
            error_failure           = 1,
            error_unaligned_access  = 3,
            error_invalid_media     = 4,
            error_invalid_state     = 5,
            error_invalid_filename  = 7,
            error_invalid_arguments = 8
        };

        enum e_file_flags : u32 {
            file_flag_read           = 0x01,
            file_flag_write          = 0x02,
            file_flag_create         = 0x04,
            file_flag_lazy_open      = 0x08,
            file_flag_deferred_close = 0x10  // close once the outstanding requests finish
        };

        struct init_t {
            i32     buffer_mode;
            e_error (*initialize)(driver_t* driver);
            e_error (*shutdown)();
        };

        struct media_t {
            u32     media_mask;
            e_error (*bind)(i32 media, const char* path, char* bound_path, i32 bound_path_size);
        };

        struct file_info_t {
            u32 unk_00; // the win32 driver writes 0
            u32 size;
        };

        struct file_t {
            u32     handle_size;
            u32     handle_alignment;
            e_error (*open)(void* handle, const char* path, u32 flags, u32 size);
            e_error (*close)(void* handle);
            e_error (*status)(void* handle, file_info_t* info);
            e_error (*handle)(void* handle, void** file_handle, u32* size);
        };

        struct io_t {
            e_error (*execute)(driver_t* driver, void* file_handle, i32 type, u32 offset, void* buffer, u32 size);
            e_error (*cancel)(void* file_handle);
            e_error (*update)();
        };

        struct buffer_t {
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

        struct io_command_t {
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
            u32          aligned_offset;
            u32          bytes_completed;
            u32          aligned_size;
            io_command_t io_command;
        };

        struct driver_t {
            const char* name;
            init_t*     init;
            media_t*    binding;
            file_t*     file_operations;
            io_t*       io_operations;
            buffer_t*   buffers_object;
            i32         chunk_decompress_jobs;
            disk_read_t disk_read;
            e_io_state  io_state;
        };

        e_error io_execute(driver_t* driver, const io_command_t* command);
        void    io_complete(driver_t* driver, u32 bytes, i32 error);

        i32  find_unlocked_read_buffer(const driver_t* driver);
        void lock_read_buffer(driver_t* driver, i32 id, i32 slot);
        void unlock_read_buffer(driver_t* driver, i32 id, i32 slot);

        void close_file(i32 id);

        ASSERT_SIZEOF  (file_info_t,       0x08);
        ASSERT_OFFSETOF(file_info_t, size, 0x04);

        ASSERT_SIZEOF  (init_t,  0x0C);
        ASSERT_SIZEOF  (media_t, 0x08);
        ASSERT_SIZEOF  (file_t,  0x18);
        ASSERT_SIZEOF  (io_t,    0x0C);

        ASSERT_SIZEOF  (buffer_t,              0x20);
        ASSERT_OFFSETOF(buffer_t, buffer_size, 0x0C);
        ASSERT_OFFSETOF(buffer_t, buffers,     0x10);

        ASSERT_SIZEOF  (io_command_t,                   0x20);
        ASSERT_OFFSETOF(io_command_t, file_handle,      0x08);
        ASSERT_OFFSETOF(io_command_t, request,          0x0C);
        ASSERT_OFFSETOF(io_command_t, type,             0x18);
        ASSERT_OFFSETOF(io_command_t, read_buffer_slot, 0x1C);

        ASSERT_SIZEOF  (disk_read_t,                  0x2C);
        ASSERT_OFFSETOF(disk_read_t, bytes_completed, 0x04);
        ASSERT_OFFSETOF(disk_read_t, io_command,      0x0C);

        ASSERT_SIZEOF  (driver_t,                        0x4C);
        ASSERT_OFFSETOF(driver_t, init,                  0x04);
        ASSERT_OFFSETOF(driver_t, file_operations,       0x0C);
        ASSERT_OFFSETOF(driver_t, buffers_object,        0x14);
        ASSERT_OFFSETOF(driver_t, chunk_decompress_jobs, 0x18);
        ASSERT_OFFSETOF(driver_t, disk_read,             0x1C);
        ASSERT_OFFSETOF(driver_t, io_state,              0x48);
    } // driver

    namespace references {
        // the first of three rows, only this one is ever selected
        inline util::memory_reference<platform_settings>  default_platform_settings { 0x00FB8538 };
        inline util::memory_reference<platform_settings*> current_platform_settings { 0x01124728 };

        inline util::memory_reference<i32> alertable_wait { 0x01124640 };
    } // references

    ASSERT_SIZEOF  (platform_settings,              0x24);
    ASSERT_OFFSETOF(platform_settings, buffer_size, 0x1C);
}} // treyarch::nfl
