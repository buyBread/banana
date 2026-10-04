#pragma once

#include "treyarch/game/game.hh"
#include "treyarch/shared/mutex.hh"
#include "util/types.hh"
#include "util/memory_reference.hh"
#include "util/singleton_external.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch {
    // app::tick counts it down through sub_734610
    struct callback_timer_set {
        engine_recursive_lock lock;
        f32                   timers[4]; // counted down to 0
        u32                   unk_20[4];
        u32                   unk_30;
        u32                   unk_34;
        i32                   unk_38;
        u32                   unk_3c;
        f32                   unk_40;    // counted up
    };

    namespace references {
        // "pack"/"repack" command-line mode, set by sub_429C40
        inline util::memory_reference<u8>    pack_mode            { 0x00FC2F82 };
        inline util::memory_reference<game*> game                 { 0x00FC2F84 };

        // inline instance used by the engine's asynchronous callback timers
        inline util::memory_reference<callback_timer_set> callback_timers { 0x00E79688 };

        inline util::memory_reference<u8> master_clock_is_up { 0x00FBF230 };

        inline util::memory_reference<void*> app_vtable           { 0x00B88974 };
        inline util::memory_reference<void*> app_arch_base_vtable { 0x00B8889C };
    } // references

    ASSERT_OFFSETOF(callback_timer_set, timers, 0x10);
    ASSERT_OFFSETOF(callback_timer_set, unk_30, 0x30);
    ASSERT_OFFSETOF(callback_timer_set, unk_38, 0x38);
    ASSERT_OFFSETOF(callback_timer_set, unk_40, 0x40);

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
        app();

        static void create_inst();

        void tick();

        inline game* get_game() {
            return this->the_game;
        }

        inline void* get_arch_base() {
            return &this->arch_base_vtable;
        }

        void skip_some_frames(i32 count) {
            if (count >= 0)
                frames_to_skip += count;
        }
    };

    ASSERT_SIZEOF(treyarch::app, 0x20);
}
