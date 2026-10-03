#pragma once

#include <array>
#include <windows.h>

#include "treyarch/nfl/driver.hh"
#include "treyarch/nfl/pool.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace nfl {
    using request_callback = void (__cdecl*)(i32 state, i32 request, void* user);

    // NFS_REQUEST_STATE_*
    enum e_internal_request_state : i32 {
        internal_request_state_waiting,
        internal_request_state_working,
        internal_request_state_workdone,
        internal_request_state_canceling,
        internal_request_state_canceled,
        internal_request_state_timeout,       // nothing sets it on pc
        internal_request_state_io_error,
        internal_request_state_io_error_waiting
    };

    enum e_file_flags : u32 {
        file_flag_read           = 0x01,
        file_flag_write          = 0x02,
        file_flag_create         = 0x04,
        file_flag_lazy_open      = 0x08,
        file_flag_deferred_close = 0x10  // close once the outstanding requests finish
    };

    struct request {
        u32              offset;
        i32              type;                  // 0 read, 1 write
        i32              file;
        nfl::driver*     driver;
        request_callback callback;
        void*            user;
        i32              start_time;
        i32              deadline;
        i32              priority;
        u8*              destination;
        i32              decompress;
        u32              output_size;
        u32              read_size;
        i32              state;
        i32              age;
        u32              bytes_read;
        u32              bytes_decompressed;
        u32              bytes_scheduled;       // output covered by chunk jobs issued so far
        i32              chunk_jobs;
        i32              holds_buffer[2];
        i32              buffer;
        i32              decompress_group;
        pool_node        node;
        u32              last_bytes_read;
        i32              last_progress_time;
    };

    struct chunk_job {
        i32       request;
        u8*       record;
        u8*       output;
        i32       state;
        u8        header[0x20];
        pool_node node;
    };

    struct completion {
        i32              state;
        request_callback callback;
        i32              request;
        void*            user;
        i32              release;
    };

    struct file {
        u32          size;
        u32          media;
        nfl::driver* driver;                    // the parent file id for a subfile
        u32          flags;
        i32          children;
        i32          pending_closes;
        char         path[0x100];
        pool_node    node;
        i32          type;                      // 0 native, 1 subfile
    };

    struct decompress_group {
        i32 group;
        i32 in_use;
    };

    u32 nfs_pre_allocate(u8* work_space);
    u8* nfs_pre_allocate_block(u32 size, u32 alignment);

    void lock();
    void unlock();

    i32 to_public_request_state(i32 state);

    request* get_request(i32 id);
    file*    get_file(i32 id);
    i32      resolve_file(i32 id);
    u8*      get_file_handle(i32 id);

    i32  nfs_open_file(u32 media_mask, const char* path, u32 flags, u32* size);
    void nfd_close_file(i32 id);
    void nfs_close_file(i32 id, bool deferred);
    void nfs_cancel_request(i32 id);
    i32  nfs_allocate_decompress_batch_job();
    bool nfs_validate_destination(u8* destination);

    namespace references {
        inline util::memory_reference<u8*> work_space { 0x01124644 };

        inline util::memory_reference<u32> work_space_used { 0x01124648 };
        inline util::memory_reference<u32> work_space_free { 0x0112464C };

        inline util::memory_reference<pool> request_pool   { 0x01124650 };
        inline util::memory_reference<pool> file_pool      { 0x01124678 };
        inline util::memory_reference<pool> chunk_job_pool { 0x011246A0 };

        inline util::memory_reference<request*>    requests     { 0x011246C8 };
        inline util::memory_reference<chunk_job*>  chunk_jobs   { 0x011246CC };
        inline util::memory_reference<completion*> completions  { 0x011246D8 };
        inline util::memory_reference<file*>       files        { 0x011246DC };
        inline util::memory_reference<u8*>         file_handles { 0x011246E0 };

        inline util::memory_reference<u32> file_handle_stride { 0x011246E4 };
        inline util::memory_reference<i32> completion_count   { 0x011246D4 };

        inline util::memory_reference<std::array<decompress_group, 8>> decompress_groups { 0x011246E8 };

        inline util::memory_reference<HANDLE> work_event   { 0x01124630 };
        inline util::memory_reference<HANDLE> thread       { 0x01124634 };
        inline util::memory_reference<HANDLE> mutex        { 0x01124638 };
        inline util::memory_reference<HANDLE> cancel_event { 0x0112463C };
    } // references

    ASSERT_SIZEOF  (request,                     0x70);
    ASSERT_OFFSETOF(request, destination,        0x24);
    ASSERT_OFFSETOF(request, state,              0x34);
    ASSERT_OFFSETOF(request, decompress_group,   0x58);
    ASSERT_OFFSETOF(request, node,               0x5C);
    ASSERT_OFFSETOF(request, last_progress_time, 0x6C);

    ASSERT_SIZEOF  (chunk_job,         0x3C);
    ASSERT_OFFSETOF(chunk_job, header, 0x10);
    ASSERT_OFFSETOF(chunk_job, node,   0x30);

    ASSERT_SIZEOF  (completion,          0x14);
    ASSERT_OFFSETOF(completion, release, 0x10);

    ASSERT_SIZEOF  (file,       0x128);
    ASSERT_OFFSETOF(file, path, 0x18);
    ASSERT_OFFSETOF(file, node, 0x118);
    ASSERT_OFFSETOF(file, type, 0x124);

    ASSERT_SIZEOF(decompress_group, 0x08);
}} // treyarch::nfl
