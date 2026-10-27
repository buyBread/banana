#pragma once

#include <array>
#include <windows.h>

#include "treyarch/nfl/driver.hh"
#include "treyarch/tx/slot_pool.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace nfl {
    using request_callback = void (__cdecl*)(i32 state, i32 request, void* user);

    // NFS_REQUEST_STATE_*
    enum e_nfs_request_state : i32 {
        nfs_request_state_waiting,
        nfs_request_state_working,
        nfs_request_state_workdone,
        nfs_request_state_canceling,
        nfs_request_state_canceled,
        nfs_request_state_timeout,       // nothing sets it on pc
        nfs_request_state_io_error,
        nfs_request_state_io_error_waiting
    };

    // NFS_FILE_TYPE_*
    enum e_nfs_file_type : i32 {
        nfs_file_type_native,
        nfs_file_type_subfile
    };

    enum e_nfd_file_flags : u32 {
        nfd_file_flag_read           = 0x01,
        nfd_file_flag_write          = 0x02,
        nfd_file_flag_create         = 0x04,
        nfd_file_flag_lazy_open      = 0x08,
        nfd_file_flag_deferred_close = 0x10  // close once the outstanding requests finish
    };

    struct nfs_request {
        u32              offset;
        i32              type;                  // 0 read, 1 write
        i32              file;
        nfd_driver*      driver;
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
        tx::slot_entry   entry;
        u32              last_bytes_read;
        i32              last_progress_time;
    };

    struct chunk_job {
        i32            request;
        u8*            record;
        u8*            output;
        i32            state;
        u8             header[0x20];
        tx::slot_entry entry;
    };

    struct nfs_request_cb {
        i32              state;
        request_callback callback;
        i32              request;
        void*            user;
        i32              release;
    };

    struct nfs_file {
        u32 size;

        union {
            struct {
                u32         media;
                nfd_driver* driver;
                u32         flags;
                i32         child_count;
            } native;

            struct {
                u32 unk_04;
                i32 parent;
            } subfile;
        } as;

        i32             pending_closes;
        char            path[0x100];
        tx::slot_entry  entry;
        e_nfs_file_type file_type;
    };

    struct decompress_group {
        i32 group;
        i32 in_use;
    };

    u32 nfs_pre_allocate_work_space(u8* work_space);
    u8* nfs_pre_allocate(u32 size, u32 alignment);

    void nfs_lock();
    void nfs_unlock();

    i32 nfs_export_request_state(i32 state);

    nfs_request* nfs_get_request(i32 id);
    nfs_file*    nfs_get_file(i32 id);
    i32          nfs_get_native_file_id(i32 id);
    u8*          nfs_get_file_handle(i32 id);

    i32  nfs_open_file(u32 media_mask, const char* path, u32 flags, u32* size);
    void nfd_close_file(i32 id);
    void nfs_close_file(i32 id, bool deferred);
    void nfs_cancel_request(i32 id);
    i32  nfs_allocate_decompress_batch_job();
    bool nfs_validate_destination(u8* destination);

    namespace references {
        inline util::memory_reference<u8*> nfs_mem { 0x01124644 };

        inline util::memory_reference<u32> nfs_mem_used { 0x01124648 };
        inline util::memory_reference<u32> nfs_mem_free { 0x0112464C };

        inline util::memory_reference<tx::slot_pool> nfs_requests_pool { 0x01124650 };
        inline util::memory_reference<tx::slot_pool> nfs_files_pool    { 0x01124678 };
        inline util::memory_reference<tx::slot_pool> chunk_job_pool    { 0x011246A0 };

        inline util::memory_reference<nfs_request*>    nfs_requests      { 0x011246C8 };
        inline util::memory_reference<chunk_job*>      chunk_jobs        { 0x011246CC };
        inline util::memory_reference<nfs_request_cb*> nfs_call_requests { 0x011246D8 };
        inline util::memory_reference<nfs_file*>       nfs_files         { 0x011246DC };
        inline util::memory_reference<u8*>             nfs_file_handles  { 0x011246E0 };

        inline util::memory_reference<u32> nfs_file_handle_size    { 0x011246E4 };
        inline util::memory_reference<i32> nfs_call_requests_count { 0x011246D4 };

        inline util::memory_reference<std::array<decompress_group, 8>> decompress_groups { 0x011246E8 };

        inline util::memory_reference<HANDLE> nfs_event    { 0x01124630 };
        inline util::memory_reference<HANDLE> nfs_thread   { 0x01124634 };
        inline util::memory_reference<HANDLE> nfs_mutex    { 0x01124638 };
        inline util::memory_reference<HANDLE> cancel_event { 0x0112463C };
    } // references

    ASSERT_SIZEOF  (nfs_request,                     0x70);
    ASSERT_OFFSETOF(nfs_request, destination,        0x24);
    ASSERT_OFFSETOF(nfs_request, state,              0x34);
    ASSERT_OFFSETOF(nfs_request, decompress_group,   0x58);
    ASSERT_OFFSETOF(nfs_request, entry,              0x5C);
    ASSERT_OFFSETOF(nfs_request, last_progress_time, 0x6C);

    ASSERT_SIZEOF  (chunk_job,         0x3C);
    ASSERT_OFFSETOF(chunk_job, header, 0x10);
    ASSERT_OFFSETOF(chunk_job, entry,  0x30);

    ASSERT_SIZEOF  (nfs_request_cb,          0x14);
    ASSERT_OFFSETOF(nfs_request_cb, release, 0x10);

    ASSERT_SIZEOF  (nfs_file,                        0x128);
    ASSERT_OFFSETOF(nfs_file, as.native.driver,      0x08);
    ASSERT_OFFSETOF(nfs_file, as.native.child_count, 0x10);
    ASSERT_OFFSETOF(nfs_file, as.subfile.parent,     0x08);
    ASSERT_OFFSETOF(nfs_file, pending_closes,        0x14);
    ASSERT_OFFSETOF(nfs_file, path,                  0x18);
    ASSERT_OFFSETOF(nfs_file, entry,                 0x118);
    ASSERT_OFFSETOF(nfs_file, file_type,             0x124);

    ASSERT_SIZEOF(decompress_group, 0x08);
}} // treyarch::nfl
