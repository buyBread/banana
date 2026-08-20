#pragma once

#include <windows.h>

#include "util/types.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch {
    struct engine_recursive_lock;
    class  engine_lock_scope;
    struct ref_counted_simple_mutex;
    class  ref_lock_scope;

    /*
        does any of this actually need to be volatile?
    */

    struct engine_recursive_lock {
        volatile u32 owner;
        volatile u32 state;
        volatile u32 depth;
                 u32 reserved;

        void acquire() {
            const u32 thread_id = GetCurrentThreadId();

            if (owner == thread_id && state == 0) {
                ++depth;

                return;
            }

            if (_InterlockedCompareExchange64((i64*)&owner, (i64)thread_id, 0) != 0)
                Sleep(0);
            
            depth = 1;
        }

        void release() {
            if (!--depth) {
                owner = 0;
                state = 0;
            }
        }
    };

    class engine_lock_scope {

        engine_recursive_lock* m_lock;

    public:

        explicit engine_lock_scope(engine_recursive_lock* lock) : m_lock(lock) {
            m_lock->acquire();
        }

        ~engine_lock_scope() {
            m_lock->release();
        }

        engine_lock_scope           (const engine_lock_scope&) = delete;
        engine_lock_scope &operator=(const engine_lock_scope&) = delete;
    };

    struct ref_counted_simple_mutex {
        engine_recursive_lock lock;
        volatile i32          ref_count;
                 u32          pad;

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

    // local convenience wrapper
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

    static_assert(sizeof(engine_recursive_lock)    == 0x10, ASSERT_FAIL_SANITY);
    static_assert(sizeof(ref_counted_simple_mutex) == 0x18, ASSERT_FAIL_SANITY);
} // treyarch