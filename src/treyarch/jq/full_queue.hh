#pragma once

#include <windows.h>

#include "retail.hh"
#include "treyarch/app/app.hh"
#include "treyarch/jq/jq.hh"
#include "util/types.hh"

namespace treyarch { namespace jq {
    // T holds the queue's statics and its descriptor
    template <typename T>
    struct full_queue {
        // sub_5D41B0
        static void init() {
            i32 group = retail::sub_A144D0(); // jq_alloc_batch_group

            T::group.write(group);

            batch& static_batch = T::static_batch.get();

            static_batch.group      = group;
            static_batch.descriptor = &T::descriptor;
            static_batch.payload[0] = nullptr;
            static_batch.payload[1] = nullptr;
            static_batch.tag        = &references::unk_01088069.get();

            T::multi_threading  .write(false);
            T::started          .write(false);
            T::batch_pool_locked.write(false);
            T::profiling        .write(false);
            T::workers_awake    .write(false);
            T::batches_added    .write(0);
            T::inline_callback  .write(T::descriptor.callback);
        }

        // inlined; the queue writes its payload between this and lock_batch_pool
        static void start(bool threaded = true) {
            bool multi_threading = threaded && references::in_multi_threaded_code.read() <= 0 && treyarch::references::master_clock_is_up.read();

            if (multi_threading)
                references::in_multi_threaded_code.write(references::in_multi_threaded_code.read() + 1);

            T::multi_threading.write(multi_threading);
            T::profiling      .write(false);
            T::started        .write(true);

            keep_workers_awake();
        }

        // inlined
        static void keep_workers_awake() {
            if (!T::workers_awake.read() && T::multi_threading.read()) {
                retail::sub_A14240(); // jq_keep_workers_awake

                T::workers_awake.write(true);
            }
        }

        // inlined
        static void let_workers_sleep() {
            if (T::workers_awake.read()) {
                retail::sub_A140C0(); // jq_let_workers_sleep

                T::workers_awake.write(false);
            }
        }

        // inlined
        static void lock_batch_pool() {
            if (!T::batch_pool_locked.read() && T::multi_threading.read()) {
                retail::sub_A140A0(); // jq_lock_batch_pool

                T::batch_pool_locked.write(true);
            }
        }

        // inlined
        static void unlock_batch_pool() {
            if (T::batch_pool_locked.read()) {
                T::batch_pool_locked.write(false);

                retail::sub_A14220(); // jq_unlock_batch_pool
            }
        }

        // sub_5AD430
        // sub_5AD740
        // sub_5ADA50
        static void add_batch(i32 priority) {
            if (!T::multi_threading.read()) {
                T::inline_callback.read()(&T::static_batch.get());

                return;
            }

            if (retail::sub_61B4B0() < 1) // jq_get_remaining_batch_pool_size
                wait_on_threads(false, false);

            retail::sub_A143E0((u64*)&T::static_batch.get(), priority); // jq_add_batch

            T::batches_added.write(T::batches_added.read() + 1);
        }

        // sub_5AD350
        // sub_5AD660
        // sub_5AD970
        // sub_5ADE90
        // sub_5AF420
        // sub_5AF6E0
        static void wait_on_threads(bool set_work, bool set_unk) {
            if (T::batches_added.read() <= 0)
                return;

            if (T::profiling.read()) {
                references::tracking_thread_id.write(0);
                references::unk_01126cfc      .write(0);
            }

            bool pool_was_locked = T::batch_pool_locked.read();

            if (pool_was_locked) {
                T::batch_pool_locked.write(false);

                retail::sub_A14220(); // jq_unlock_batch_pool
            }

            bool workers_were_awake = T::workers_awake.read();

            if (workers_were_awake) {
                retail::sub_A140C0(); // jq_let_workers_sleep

                T::workers_awake.write(false);
            }

            retail::sub_A148D0(T::group.read(), set_work, set_unk); // jq_flush

            if (workers_were_awake && !T::workers_awake.read() && T::multi_threading.read()) {
                retail::sub_A14240(); // jq_keep_workers_awake

                T::workers_awake.write(true);
            }

            if (pool_was_locked && !T::batch_pool_locked.read() && T::multi_threading.read()) {
                retail::sub_A140A0(); // jq_lock_batch_pool

                T::batch_pool_locked.write(true);
            }

            if (T::profiling.read()) {
                references::tracking_thread_id.write(GetCurrentThreadId());
                references::unk_01126cfc      .write(0);
            }

            T::batches_added.write(0);
        }

        // sub_5AD480
        // sub_5AD790
        // sub_5ADAA0
        // sub_5ADF70
        // sub_5AF500
        // sub_5AF7C0
        static void finish(bool set_work) {
            unlock_batch_pool();
            wait_on_threads(set_work, false);
            let_workers_sleep();

            if (T::multi_threading.read()) {
                if (T::profiling.read()) {
                    references::tracking_active   .write(false);
                    references::tracking_thread_id.write(0);
                    references::unk_01126cfc      .write(0);
                }

                references::in_multi_threaded_code.write(references::in_multi_threaded_code.read() - 1);
            }

            T::multi_threading.write(false);
            T::profiling      .write(false);
            T::started        .write(false);
        }
    };
}} // treyarch::jq
