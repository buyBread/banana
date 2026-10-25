#pragma once

#include "treyarch/jq/full_queue.hh"
#include "treyarch/jq/jq.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    void animation_controller_jq_callback      (jq::batch* batch);
    void multi_animation_controller_jq_callback(jq::batch* batch);
    void animation_signal_jq_callback          (jq::batch* batch);
    void multi_animation_signal_jq_callback    (jq::batch* batch);

    // advances one controller's playback per batch
    struct animation_controller_jq : jq::full_queue<animation_controller_jq> {
        static inline constexpr jq::descriptor descriptor = { animation_controller_jq_callback, "animation_controller_jqCallback" };

        static inline util::memory_reference<jq::batch> static_batch { 0x00E681D8 };

        static inline util::memory_reference<i32> group         { 0x00FD42E8 };
        static inline util::memory_reference<i32> batches_added { 0x00FD42EC };

        static inline util::memory_reference<jq::batch_callback> inline_callback { 0x00FD42F4 };

        static inline util::memory_reference<bool> workers_awake     { 0x00FD42DD };
        static inline util::memory_reference<bool> started           { 0x00FD42DE };
        static inline util::memory_reference<bool> profiling         { 0x00FD42DF };
        static inline util::memory_reference<bool> batch_pool_locked { 0x00FD42F0 };
        static inline util::memory_reference<bool> multi_threading   { 0x00FD42F1 };
    };

    // the same over up to ten queued controllers per batch
    struct multi_animation_controller_jq : jq::full_queue<multi_animation_controller_jq> {
        static inline constexpr jq::descriptor descriptor = { multi_animation_controller_jq_callback, "multi_animation_controller_jqCallback" };

        static inline util::memory_reference<jq::batch> static_batch { 0x00E68208 };

        static inline util::memory_reference<i32> group         { 0x00FD42B8 };
        static inline util::memory_reference<i32> batches_added { 0x00FD42BC };

        static inline util::memory_reference<jq::batch_callback> inline_callback { 0x00FD42C4 };

        static inline util::memory_reference<bool> workers_awake     { 0x00FD42AD };
        static inline util::memory_reference<bool> started           { 0x00FD42AE };
        static inline util::memory_reference<bool> profiling         { 0x00FD42AF };
        static inline util::memory_reference<bool> batch_pool_locked { 0x00FD42C0 };
        static inline util::memory_reference<bool> multi_threading   { 0x00FD42C1 };
    };

    // raises the transport signals a layer's newest record crossed this frame
    struct animation_signal_jq : jq::full_queue<animation_signal_jq> {
        static inline constexpr jq::descriptor descriptor = { animation_signal_jq_callback, "animation_signal_jqCallback" };

        static inline util::memory_reference<jq::batch> static_batch { 0x00E68238 };

        static inline util::memory_reference<i32> group         { 0x00FD4214 };
        static inline util::memory_reference<i32> batches_added { 0x00FD4218 };

        static inline util::memory_reference<jq::batch_callback> inline_callback { 0x00FD4220 };

        static inline util::memory_reference<bool> workers_awake     { 0x00FD4210 };
        static inline util::memory_reference<bool> started           { 0x00FD4211 };
        static inline util::memory_reference<bool> profiling         { 0x00FD4212 };
        static inline util::memory_reference<bool> batch_pool_locked { 0x00FD4213 };
        static inline util::memory_reference<bool> multi_threading   { 0x00FD421C };
    };

    struct multi_animation_signal_jq : jq::full_queue<multi_animation_signal_jq> {
        static inline constexpr jq::descriptor descriptor = { multi_animation_signal_jq_callback, "multi_animation_signal_jqCallback" };

        static inline util::memory_reference<jq::batch> static_batch { 0x00E68268 };

        static inline util::memory_reference<i32> group         { 0x00FD425C };
        static inline util::memory_reference<i32> batches_added { 0x00FD4260 };

        static inline util::memory_reference<jq::batch_callback> inline_callback { 0x00FD4268 };

        static inline util::memory_reference<bool> workers_awake     { 0x00FD4258 };
        static inline util::memory_reference<bool> started           { 0x00FD4259 };
        static inline util::memory_reference<bool> profiling         { 0x00FD425A };
        static inline util::memory_reference<bool> batch_pool_locked { 0x00FD425B };
        static inline util::memory_reference<bool> multi_threading   { 0x00FD4264 };
    };
} // treyarch
