#pragma once

#include "retail.hh"
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
    };
}} // treyarch::jq
