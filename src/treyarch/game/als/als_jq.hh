#pragma once

#include "treyarch/jq/full_queue.hh"
#include "treyarch/jq/jq.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace als {
    void als_jq_callback                             (jq::batch* batch);
    void als_mocomp_jq_callback                      (jq::batch* batch);
    void multi_als_mocomp_jq_callback                (jq::batch* batch);
    void animation_event_signal_jq_callback          (jq::batch* batch);
    void multi_animation_event_signal_jq_callback    (jq::batch* batch);
    void pre_animation_event_signal_jq_callback      (jq::batch* batch);
    void multi_pre_animation_event_signal_jq_callback(jq::batch* batch);

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

    // runs one queued motion compensator's post-animation action per batch
    struct als_mocomp_jq : jq::full_queue<als_mocomp_jq> {
        static inline constexpr jq::descriptor descriptor = { als_mocomp_jq_callback, "als_mocomp_jqCallback" };

        static inline util::memory_reference<jq::batch> static_batch { 0x00E683A8 };

        static inline util::memory_reference<i32> group         { 0x00FD4228 };
        static inline util::memory_reference<i32> batches_added { 0x00FD422C };

        static inline util::memory_reference<jq::batch_callback> inline_callback { 0x00FD4234 };

        static inline util::memory_reference<bool> workers_awake     { 0x00FD421D };
        static inline util::memory_reference<bool> started           { 0x00FD421E };
        static inline util::memory_reference<bool> profiling         { 0x00FD421F };
        static inline util::memory_reference<bool> batch_pool_locked { 0x00FD4230 };
        static inline util::memory_reference<bool> multi_threading   { 0x00FD4231 };
    };

    // registered, but nothing posts to it
    struct multi_als_mocomp_jq : jq::full_queue<multi_als_mocomp_jq> {
        static inline constexpr jq::descriptor descriptor = { multi_als_mocomp_jq_callback, "multi_als_mocomp_jqCallback" };

        static inline util::memory_reference<jq::batch> static_batch { 0x00E683D8 };

        static inline util::memory_reference<i32> group         { 0x00FD4294 };
        static inline util::memory_reference<i32> batches_added { 0x00FD4298 };

        static inline util::memory_reference<jq::batch_callback> inline_callback { 0x00FD429C };

        static inline util::memory_reference<bool> workers_awake     { 0x00FD4283 };
        static inline util::memory_reference<bool> started           { 0x00FD4290 };
        static inline util::memory_reference<bool> profiling         { 0x00FD4291 };
        static inline util::memory_reference<bool> batch_pool_locked { 0x00FD4292 };
        static inline util::memory_reference<bool> multi_threading   { 0x00FD4293 };
    };

    // raises a queued machine's state events for the frame's span
    struct animation_event_signal_jq : jq::full_queue<animation_event_signal_jq> {
        static inline constexpr jq::descriptor descriptor = { animation_event_signal_jq_callback, "animation_event_signal_jqCallback" };

        static inline util::memory_reference<jq::batch> static_batch { 0x00E68408 };

        static inline util::memory_reference<i32> group         { 0x00FD426C };
        static inline util::memory_reference<i32> batches_added { 0x00FD4270 };

        static inline util::memory_reference<jq::batch_callback> inline_callback { 0x00FD4278 };

        static inline util::memory_reference<bool> workers_awake     { 0x00FD4265 };
        static inline util::memory_reference<bool> started           { 0x00FD4266 };
        static inline util::memory_reference<bool> profiling         { 0x00FD4267 };
        static inline util::memory_reference<bool> batch_pool_locked { 0x00FD4274 };
        static inline util::memory_reference<bool> multi_threading   { 0x00FD4275 };
    };

    struct multi_animation_event_signal_jq : jq::full_queue<multi_animation_event_signal_jq> {
        static inline constexpr jq::descriptor descriptor = { multi_animation_event_signal_jq_callback, "multi_animation_event_signal_jqCallback" };

        static inline util::memory_reference<jq::batch> static_batch { 0x00E68438 };

        static inline util::memory_reference<i32> group         { 0x00FD42D4 };
        static inline util::memory_reference<i32> batches_added { 0x00FD42D8 };

        static inline util::memory_reference<jq::batch_callback> inline_callback { 0x00FD42E0 };

        static inline util::memory_reference<bool> workers_awake     { 0x00FD42D0 };
        static inline util::memory_reference<bool> started           { 0x00FD42D1 };
        static inline util::memory_reference<bool> profiling         { 0x00FD42D2 };
        static inline util::memory_reference<bool> batch_pool_locked { 0x00FD42D3 };
        static inline util::memory_reference<bool> multi_threading   { 0x00FD42DC };
    };

    // the same for the events before the animation advances
    struct pre_animation_event_signal_jq : jq::full_queue<pre_animation_event_signal_jq> {
        static inline constexpr jq::descriptor descriptor = { pre_animation_event_signal_jq_callback, "pre_animation_event_signal_jqCallback" };

        static inline util::memory_reference<jq::batch> static_batch { 0x00E68468 };

        static inline util::memory_reference<i32> group         { 0x00FD424C };
        static inline util::memory_reference<i32> batches_added { 0x00FD4250 };

        static inline util::memory_reference<jq::batch_callback> inline_callback { 0x00FD4254 };

        static inline util::memory_reference<bool> workers_awake     { 0x00FD423F };
        static inline util::memory_reference<bool> started           { 0x00FD4248 };
        static inline util::memory_reference<bool> profiling         { 0x00FD4249 };
        static inline util::memory_reference<bool> batch_pool_locked { 0x00FD424A };
        static inline util::memory_reference<bool> multi_threading   { 0x00FD424B };
    };

    struct multi_pre_animation_event_signal_jq : jq::full_queue<multi_pre_animation_event_signal_jq> {
        static inline constexpr jq::descriptor descriptor = { multi_pre_animation_event_signal_jq_callback, "multi_pre_animation_event_signal_jqCallback" };

        static inline util::memory_reference<jq::batch> static_batch { 0x00E68498 };

        static inline util::memory_reference<i32> group         { 0x00FD42A4 };
        static inline util::memory_reference<i32> batches_added { 0x00FD42A8 };

        static inline util::memory_reference<jq::batch_callback> inline_callback { 0x00FD42B0 };

        static inline util::memory_reference<bool> workers_awake     { 0x00FD42A0 };
        static inline util::memory_reference<bool> started           { 0x00FD42A1 };
        static inline util::memory_reference<bool> profiling         { 0x00FD42A2 };
        static inline util::memory_reference<bool> batch_pool_locked { 0x00FD42A3 };
        static inline util::memory_reference<bool> multi_threading   { 0x00FD42AC };
    };
}} // treyarch::als
