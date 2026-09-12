#include <thread>
#include <chrono>

#include "flags.hh"
#include "banana/core.hh"
#include "banana/lifecycle.hh"
#include "banana/logging.hh"
#include "banana/hooks/manager.hh"
#include "util/macros/debug.hh"

void banana::thread() {
    log.start();

    if (core::init())
        core::spin();

    core::shutdown();

    log.close();
}

bool banana::core::init() {
    for (;;) { // wait for yay
        auto current = state::current();

        if (current == e_lifecycle::ready)
            break;

        if (current == e_lifecycle::failed || current == e_lifecycle::stopping)
            return false;

        state::poll(current);
    }

    hook_manager.install(HK_DEFAULT_CATEGORY);

    /*
        devel mode hooks go here too, just add clauses around purely debug mode code;
        otherwise doing some sort of debug / devel separation sounds like a structural headache...
    */
#ifndef NDEBUG
    hook_manager.install("debug");
#else
#if SPINLOCK_MUTEX
    /*
        it's not really what i'd "like" to "ship", but i can't figure out an ezpz variant that truly fixes stutters caused by the mutex...
        so until that happens, or until we no longer need to hook this (crack dream), it'll do to alleviate the problem somewhat.
    */
    hook_manager.enable_hook("debug", "sub_A6C860");
#endif
#endif

    return true;
}

void banana::core::spin() {
    for (;;) { // wait for die
        auto current = state::current();

        if (current == e_lifecycle::failed)
            FATAL_BREAKPOINT();

        if (current == e_lifecycle::stopping)
            return;

        state::poll(current);
    }
}

void banana::core::shutdown() {
    hook_manager.shutdown();
}