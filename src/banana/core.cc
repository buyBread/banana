#include <thread>
#include <chrono>

#include "banana/util/macros/debug.hh"
#include "banana/core.hh"
#include "banana/lifecycle.hh"
#include "banana/hooks/manager.hh"
#include "banana/imgui/imgui.hh"

void banana::thread() {
    if (core::init())
        core::spin();

    imgui::shutdown();

    core::shutdown();
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

    // devel mode hooks go here too
#ifndef NDEBUG
    hook_manager.install("debug");
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