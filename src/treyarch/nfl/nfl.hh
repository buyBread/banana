#pragma once

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

    u32  init(const init_params* params);
    void start(void* work_space);

    namespace references {
        inline util::memory_reference<init_params> params { 0x00FB84B0 };

        inline util::memory_reference<i32> thread_priority { 0x00FB84A4 };
    } // references

    ASSERT_SIZEOF  (init_params,                  0x14);
    ASSERT_OFFSETOF(init_params, request_count,   0x04);
    ASSERT_OFFSETOF(init_params, chunk_job_count, 0x08);
    ASSERT_OFFSETOF(init_params, buffer_mode,     0x0C);
    ASSERT_OFFSETOF(init_params, thread_mode,     0x10);
}} // treyarch::nfl
