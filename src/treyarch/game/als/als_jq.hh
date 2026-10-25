#pragma once

#include "treyarch/jq/full_queue.hh"
#include "treyarch/jq/jq.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace als {
    void als_jq_callback(jq::batch* batch);

    // advances one als per batch
    struct als_jq : jq::full_queue<als_jq> {
        static inline constexpr jq::descriptor descriptor = { als_jq_callback, "als_jqCallback" };

        static inline util::memory_reference<jq::batch> static_batch { 0x00E682E4 };

        static inline util::memory_reference<i32> group         { 0x00FD427C };
        static inline util::memory_reference<i32> batches_added { 0x00FD4284 };

        static inline util::memory_reference<jq::batch_callback> inline_callback { 0x00FD428C };

        static inline util::memory_reference<bool> workers_awake     { 0x00FD4276 };
        static inline util::memory_reference<bool> started           { 0x00FD4277 };
        static inline util::memory_reference<bool> profiling         { 0x00FD4280 };
        static inline util::memory_reference<bool> batch_pool_locked { 0x00FD4281 };
        static inline util::memory_reference<bool> multi_threading   { 0x00FD4282 };

        static inline util::memory_reference<f32> delta_t { 0x00FD4288 }; // the frame's, before each actor's dilation
    };
}} // treyarch::als
