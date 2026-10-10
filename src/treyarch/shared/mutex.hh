#pragma once

#include <windows.h>

#include "flags.hh"
#include "util/types.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch {
    struct engine_recursive_lock;
    class  engine_lock_scope;
    class  engine_reference_scope;
    struct ref_counted_simple_mutex;
    class  ref_lock_scope;

    struct engine_recursive_lock {
        volatile u32 owner;
        volatile u32 state;
        volatile u32 depth;
                 u32 reserved;

        bool try_acquire(i64 owner_state) {
            return _InterlockedCompareExchange64((volatile i64*)&owner, owner_state, 0) == 0;
        }

        /*
            this either did or didn't cause issues for me when trying to recreate the event manager.
            either way, it seems to work all the same now, but i'll add a flag to switch between the two anyways.
            (future debugging qol, yay)

            2026-09-13: performance wise, it's a mixed bag. some situations one is better and in others it's the opposite.
                        attempting to improve the situation or debate whether it even should be improved shouldn't be done at the moment.
        */
#if SPINLOCK_MUTEX
        void acquire_contended(i64 owner_state) {
            u32 pause_count = 1;

            while (!try_acquire(owner_state)) {
                for (u32 i = 0; i < pause_count; ++i)
                    _mm_pause();

                if (pause_count < 1024) {
                    pause_count <<= 1;

                    continue;
                }

                if (!SwitchToThread())
                    Sleep(1);
            }
        }
#else
        // sub_A6C860
        void acquire_contended(i64 owner_state) {
            while (!try_acquire(owner_state))
                Sleep(0);
        }
#endif

        // sub_401930
        void acquire() {
            const u32 thread_id = GetCurrentThreadId();

            if (owner == thread_id && state == 0) {
                ++depth;

                return;
            }

            if (!try_acquire((i64)thread_id))
                acquire_contended((i64)thread_id);
            
            depth = 1;
        }

        // sub_4019B0
        void release() {
            if (!--depth) {
                owner = 0;
                state = 0;
            }
        }

        // sub_44CED0
        void acquire_reference() {
            const i64 thread_id = (i64)GetCurrentThreadId();

            if (_InterlockedCompareExchange64((volatile i64*)&owner, thread_id, thread_id) == thread_id) {
                _InterlockedIncrement((volatile long*)&depth);

                return;
            }

            while (!try_acquire(thread_id))
                Sleep(0);

            _InterlockedIncrement((volatile long*)&depth);

            while (_InterlockedCompareExchange64((volatile i64*)&owner, 0, thread_id) != thread_id);
        }

        void release_reference() {
            _InterlockedDecrement((volatile long*)&depth);
        }
    };

    class engine_lock_scope {

        engine_recursive_lock* m_lock;

    public:

        explicit engine_lock_scope(engine_recursive_lock* lock) : m_lock(lock) {
            m_lock->acquire();
        }

        // sub_4019D0
        ~engine_lock_scope() {
            m_lock->release();
        }

        engine_lock_scope           (const engine_lock_scope&) = delete;
        engine_lock_scope &operator=(const engine_lock_scope&) = delete;
    };

    class engine_reference_scope {

        engine_recursive_lock* m_lock;

    public:

        explicit engine_reference_scope(engine_recursive_lock* lock) : m_lock(lock) {
            m_lock->acquire_reference();
        }

        ~engine_reference_scope() {
            m_lock->release_reference();
        }

        engine_reference_scope           (const engine_reference_scope&) = delete;
        engine_reference_scope &operator=(const engine_reference_scope&) = delete;
    };

    struct ref_counted_simple_mutex {
        engine_recursive_lock lock;
        volatile i32          ref_count;
                 u32          pad;

        void reset() {
            lock.owner = 0;
            lock.state = 0;
            lock.depth = 0;
            ref_count  = 0;
        }

        void lock_ref() {
            lock.acquire();
            
            ++ref_count;
        }

        void unlock_ref() {
            i32 count = ref_count;

            if (count <= 0)
                return;

            ref_count = count - 1;

            if (lock.depth-- == 1) {
                lock.owner = 0;
                lock.state = 0;
            }
        }
    };

    class ref_lock_scope {

        ref_counted_simple_mutex* m_lock;

    public:

        explicit ref_lock_scope(ref_counted_simple_mutex* lock) : m_lock(lock) {
            m_lock->lock_ref();
        }

        ~ref_lock_scope() {
            m_lock->unlock_ref();
        }

        ref_lock_scope           (const ref_lock_scope&) = delete;
        ref_lock_scope &operator=(const ref_lock_scope&) = delete;
    };

    ASSERT_SIZEOF(engine_recursive_lock,    0x10);
    ASSERT_SIZEOF(ref_counted_simple_mutex, 0x18);
} // treyarch
