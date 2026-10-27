#pragma once

#include "treyarch/jq/full_queue.hh"
#include "treyarch/jq/jq.hh"
#include "treyarch/nal/nal.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // the nal pose job
    struct animation_pose_jq : jq::full_queue<animation_pose_jq> {
        static inline constexpr jq::descriptor descriptor = { nal::nal_jq_callback, "animation_pose_jqCallback" };

        static inline util::memory_reference<jq::batch> static_batch { 0x00E68298 };

        static inline util::memory_reference<i32> group         { 0x00FD4238 };
        static inline util::memory_reference<i32> batches_added { 0x00FD4240 };

        static inline util::memory_reference<jq::batch_callback> inline_callback { 0x00FD4244 };

        static inline util::memory_reference<bool> workers_awake     { 0x00FD4232 };
        static inline util::memory_reference<bool> started           { 0x00FD4233 };
        static inline util::memory_reference<bool> profiling         { 0x00FD423C };
        static inline util::memory_reference<bool> batch_pool_locked { 0x00FD423D };
        static inline util::memory_reference<bool> multi_threading   { 0x00FD423E };
    };
} // treyarch
