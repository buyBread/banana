#pragma once

#include "util/memory_reference.hh"
#include "util/types.hh"

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

    u32  __cdecl apply_frame_lock(e_frame_lock frame_lock);
    void __cdecl set_frame_lock(e_frame_lock frame_lock);

    namespace references {
        inline util::memory_reference<e_frame_lock> current_frame_lock   { 0x00F528D8 };
        inline util::memory_reference<e_frame_lock> requested_frame_lock { 0x00F528DC };
    } // references
}} // treyarch::ngl
