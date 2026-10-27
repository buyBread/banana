#pragma once

#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace ngl {
    enum e_frame_lock : u32 {
        frame_lock_none,
        frame_lock_one,
        frame_lock_two,
        frame_lock_one_or_immediate,
        frame_lock_two_or_immediate,
        frame_lock_three,
        frame_lock_three_or_immediate
    };

    void set_frame_lock(e_frame_lock frame_lock);

    namespace d3d9 {
        u32 set_frame_lock_params(e_frame_lock frame_lock);
    } // d3d9

    namespace references {
        inline util::memory_reference<e_frame_lock> frame_lock      { 0x00F528D8 };
        inline util::memory_reference<e_frame_lock> prev_frame_lock { 0x00F528DC };
    } // references
}} // treyarch::ngl
