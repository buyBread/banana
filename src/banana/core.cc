#include <thread>
#include <chrono>

#include "flags.hh"
#include "banana/core.hh"
#include "banana/logging.hh"
#include "banana/hooks/manager.hh"

#include "util/macros/debug.hh"

namespace banana {
    void thread() {
        if (core::init())
            core::spin();

        core::shutdown();
    }
} // banana

namespace banana { namespace core {
    bool init() {
        for (;;) { // wait for yay
            auto current = state::current();

            if (current == e_lifecycle::ready)
                break;

            if (current == e_lifecycle::failed || current == e_lifecycle::stopping)
                return false;

            state::poll();
        }

        hook_manager.install(HK_DEFAULT_CATEGORY);

        /*
            devel mode hooks go here too, just add clauses around purely debug mode code;
            otherwise doing some sort of debug / devel separation sounds like a structural headache...
        */
#ifndef NDEBUG
        hook_manager.install("debug");
#endif

        return true;
    }

    void spin() {
        for (;;) { // wait for die
            auto current = state::current();

            if (current == e_lifecycle::failed)
                FATAL_BREAKPOINT();

            if (current == e_lifecycle::stopping)
                return;

            state::poll();
        }
    }

    void shutdown() {
        hook_manager.shutdown();

        log.close();
    }
}} // banana::core