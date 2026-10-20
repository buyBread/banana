#pragma once

#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace jq {
    struct batch;

    using batch_callback = void (__cdecl*)(batch*);

    void jobqueue_init();

    namespace references {
        // a lite queue with no name of its own, set up at the end of jobqueue_init
        inline util::memory_reference<u8> unnamed_queue_active   { 0x00FD42C2 };
        inline util::memory_reference<u8> unnamed_queue_finished { 0x00FD42C3 };

        inline util::memory_reference<i32>            unnamed_queue_group    { 0x00FD42C8 };
        inline util::memory_reference<batch_callback> unnamed_queue_callback { 0x00FD42CC };
    } // references
}} // treyarch::jq
