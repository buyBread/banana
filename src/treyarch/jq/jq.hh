#pragma once

#include "treyarch/shared/mutex.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace jq {
    struct batch;

    using batch_callback = void (__cdecl*)(batch*);

    struct descriptor {
        batch_callback callback;
        const char*    name;
    };

    struct batch {
        void*                 payload[3]; // the multi queues hand an array of count items in the first
        i32                   count;
        u32                   unk_10[2];
        void*                 tag;
        const jq::descriptor* descriptor;
        i32                   group;
        u32                   unk_24;
    };

    ASSERT_SIZEOF  (descriptor, 0x08);

    ASSERT_SIZEOF  (batch,             0x28);
    ASSERT_OFFSETOF(batch, count,      0x0C);
    ASSERT_OFFSETOF(batch, tag,        0x18);
    ASSERT_OFFSETOF(batch, descriptor, 0x1C);
    ASSERT_OFFSETOF(batch, group,      0x20);

    void jobqueue_init();

    namespace references {
        // every queue's batches carry it
        inline util::memory_reference<u8> unk_01088069 { 0x01088069 };

        // the milestone's InMultiThreadedCode(); full queues raise it while they run threaded
        inline util::memory_reference<i32> in_multi_threaded_code { 0x01126CF0 };

        // which thread waits on a profiled queue
        inline util::memory_reference<bool> tracking_active    { 0x01126CF4 };
        inline util::memory_reference<u32>  tracking_thread_id { 0x01126CF8 };
        inline util::memory_reference<u32>  unk_01126cfc       { 0x01126CFC };

        // zeroed with 0x01126D00 by jobqueue_init; the als takes it around a single-threaded run of its queues
        inline util::memory_reference<engine_recursive_lock> unk_01126d10 { 0x01126D10 };

        // a lite queue with no name of its own, set up at the end of jobqueue_init
        inline util::memory_reference<u8> unnamed_queue_active   { 0x00FD42C2 };
        inline util::memory_reference<u8> unnamed_queue_finished { 0x00FD42C3 };

        inline util::memory_reference<i32>            unnamed_queue_group    { 0x00FD42C8 };
        inline util::memory_reference<batch_callback> unnamed_queue_callback { 0x00FD42CC };
    } // references
}} // treyarch::jq
