#pragma once

#include "treyarch/nfl/system.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace nfl {
    struct init_params {
        i32 file_count;
        i32 request_count;
        i32 chunk_job_count;
        i32 buffer_mode;    // -1 keeps each driver's own mode
        i32 thread_mode;    // 1 runs the drivers on a background thread
    };

    // NFL_REQUEST_STATE_*, what callbacks receive
    enum e_request_state : i32 {
        request_state_invalid = -1,
        request_state_completed,
        request_state_canceled,
        request_state_timeout,
        request_state_error,
        request_state_active
    };

    struct request_params {
        i32              file;
        request_callback callback;
        i32              type;        // 0 read, 1 write
        i32              priority;    // higher goes first
        u32              offset;
        u8*              destination;
        i32              decompress;  // NCH/LZO1X chunks
        u32              output_size;
        u32              read_size;
        u32              timeout;     // ms, 0 for none
        void*            user;
    };

    u32  init(const init_params* params);
    void start(void* work_space);
    void update();

    i32  open_file(u32 media_mask, const char* path);
    void close_file(i32 id);

    i32 add_request(const request_params* params);

    i32 media_status();

    namespace references {
        inline util::memory_reference<init_params> params { 0x00FB84B0 };

        inline util::memory_reference<i32> thread_priority { 0x00FB84A4 };
    } // references

    ASSERT_SIZEOF  (init_params,                  0x14);
    ASSERT_OFFSETOF(init_params, request_count,   0x04);
    ASSERT_OFFSETOF(init_params, chunk_job_count, 0x08);
    ASSERT_OFFSETOF(init_params, buffer_mode,     0x0C);
    ASSERT_OFFSETOF(init_params, thread_mode,     0x10);

    ASSERT_SIZEOF  (request_params,             0x2C);
    ASSERT_OFFSETOF(request_params, offset,     0x10);
    ASSERT_OFFSETOF(request_params, decompress, 0x18);
    ASSERT_OFFSETOF(request_params, timeout,    0x24);
    ASSERT_OFFSETOF(request_params, user,       0x28);
}} // treyarch::nfl
