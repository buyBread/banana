#pragma once

#include "util/types.hh"
#include "util/memory_reference.hh"
#include "util/singleton_external.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch {
    // impl (sadly mandatory)
    class game;

    // impl?
    // SM3 .ii used ppc time base register, retail PC uses QueryPerformanceCounter
    class hires_clock_t {
        u64 last_reset_ticks;
    };

    struct game_frame_timing {
        f32 total_delta;
        f32 flip_delta;
        f32 limit_delta;
    };

    namespace references {
        inline util::memory_reference<game*> game            { 0x00FC2F84 };
        // inline instance used by the engine's asynchronous callback timers
        inline util::memory_reference<u8>    callback_timers { 0x00E79688 };
    } // references

    class app : public util::singleton_external<app, 0x00FC2FCC> {
        // also inherited from arch_base; impl?

        void* singleton_vtable; // 0x00
        void* arch_base_vtable; // 0x04
        u32   arch_handle;      // 0x08

        game*         the_game; // who gave a thumbs up at this name?
        hires_clock_t real_clock;
        i32           frames_to_skip;
        u32           padding_1c;

    public:
        void tick();
    };

    ASSERT_SIZEOF(treyarch::hires_clock_t, 0x08);
    ASSERT_SIZEOF(treyarch::game_frame_timing, 0x0C);
    ASSERT_SIZEOF(treyarch::app, 0x20);
}
