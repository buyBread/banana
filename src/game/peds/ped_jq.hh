#pragma once

#include "treyarch/jq/full_queue.hh"
#include "treyarch/jq/jq.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    void ped_jq_callback(jq::batch* batch);

    // advances one ped per batch
    struct ped_jq : jq::full_queue<ped_jq> {
        static inline constexpr jq::descriptor descriptor = { ped_jq_callback, "ped_jqCallback" };

        static inline util::memory_reference<jq::batch> static_batch { 0x00E795C8 };

        static inline util::memory_reference<i32> group         { 0x0102FDF8 };
        static inline util::memory_reference<i32> batches_added { 0x0102FE00 };

        static inline util::memory_reference<jq::batch_callback> inline_callback { 0x0102FE04 };

        static inline util::memory_reference<bool> workers_awake     { 0x0102FDE6 };
        static inline util::memory_reference<bool> started           { 0x0102FDE7 };
        static inline util::memory_reference<bool> profiling         { 0x0102FDFC };
        static inline util::memory_reference<bool> batch_pool_locked { 0x0102FDFD };
        static inline util::memory_reference<bool> multi_threading   { 0x0102FDFE };
    };

    // advances one vendor per batch; its init and callback are still retail's
    struct vendor_jq : jq::full_queue<vendor_jq> {
        static inline util::memory_reference<jq::batch> static_batch { 0x00E795F8 };

        static inline util::memory_reference<i32> group         { 0x0102FD7C };
        static inline util::memory_reference<i32> batches_added { 0x0102FD84 };

        static inline util::memory_reference<jq::batch_callback> inline_callback { 0x0102FD88 };

        static inline util::memory_reference<bool> workers_awake     { 0x0102FD72 };
        static inline util::memory_reference<bool> started           { 0x0102FD73 };
        static inline util::memory_reference<bool> profiling         { 0x0102FD80 };
        static inline util::memory_reference<bool> batch_pool_locked { 0x0102FD81 };
        static inline util::memory_reference<bool> multi_threading   { 0x0102FD82 };
    };

    // lite queues; the pedestrians frame only waits on them
    struct ped_spawn_pos_search_jq {
        static inline util::memory_reference<i32>  group    { 0x0102FD6C };
        static inline util::memory_reference<bool> finished { 0x0102FD71 };
    };

    struct ped_spawn_quad_search_jq {
        static inline util::memory_reference<i32>  group    { 0x0102FD98 };
        static inline util::memory_reference<bool> finished { 0x0102FD9C };
    };
} // treyarch
