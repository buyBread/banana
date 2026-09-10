#pragma once

#include "treyarch/game/game.hh"
#include "util/types.hh"
#include "util/memory_reference.hh"
#include "util/singleton_external.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch {
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

    ASSERT_SIZEOF(treyarch::app, 0x20);
}
