#pragma once

#include <cstdint>

#include "../util.hh"

namespace banana { namespace NGL {
namespace internals { namespace mutex {
    inline auto fn_acquire_lock = (int(__thiscall*)(void*))0x00401930;

    struct engine_recursive_lock {
        volatile uint32_t owner;
        volatile uint32_t state;
        volatile uint32_t depth;

        void acquire() {
            fn_acquire_lock(this);
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

    struct engine_activity_guard {
        engine_recursive_lock lock;
        uint32_t              unk_0c;
        volatile uint32_t     activity;

        void acquire() {
            lock.acquire();
            ++activity;
        }

        void release() {
            uint32_t count = activity;

            if (!count)
                return;

            bool clear = lock.depth-- == 1;
            activity = count - 1;

            if (clear) {
                lock.owner = 0;
                lock.state = 0;
            }
        }
    };

    class engine_activity_scope {

        engine_activity_guard* m_guard;

    public:

        explicit engine_activity_scope(engine_activity_guard* guard) : m_guard(guard) {
            m_guard->acquire();
        }

        ~engine_activity_scope() {
            m_guard->release();
        }

        engine_activity_scope           (const engine_activity_scope&) = delete;
        engine_activity_scope &operator=(const engine_activity_scope&) = delete;
    };

    static_assert(sizeof(engine_recursive_lock) == 0x0C, ASSERT_FAIL_SANITY);
    static_assert(sizeof(engine_activity_guard) == 0x14, ASSERT_FAIL_SANITY);

    static_assert(offsetof(engine_activity_guard, activity) == 0x10, ASSERT_FAIL_SANITY);
} } // internals::mutex
} } // banana::NGL