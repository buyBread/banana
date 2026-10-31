#pragma once

#include "treyarch/jq/full_queue.hh"
#include "treyarch/jq/jq.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    void ai_core_jq_callback(jq::batch* batch);

    // advances one core per batch
    struct ai_core_jq : jq::full_queue<ai_core_jq> {
        static inline constexpr jq::descriptor descriptor = { ai_core_jq_callback, "ai_core_jqCallback" };

        static inline util::memory_reference<jq::batch> static_batch { 0x00E623F4 };

        static inline util::memory_reference<i32> group         { 0x00FC5264 };
        static inline util::memory_reference<i32> batches_added { 0x00FC5268 };

        static inline util::memory_reference<jq::batch_callback> inline_callback { 0x00FC5274 };

        static inline util::memory_reference<bool> workers_awake     { 0x00FC5260 };
        static inline util::memory_reference<bool> started           { 0x00FC5261 };
        static inline util::memory_reference<bool> profiling         { 0x00FC5262 };
        static inline util::memory_reference<bool> batch_pool_locked { 0x00FC5263 };
        static inline util::memory_reference<bool> multi_threading   { 0x00FC526C };

        static inline util::memory_reference<f32> delta_t { 0x00FC5270 }; // the frame's, before each actor's dilation
    };
} // treyarch
