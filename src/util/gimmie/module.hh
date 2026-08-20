#pragma once

#include <windows.h>
#include <format>

#include "util/macros/debug.hh"

namespace util { namespace gimmie {
    inline HMODULE module(const std::string &name) {
        auto module = GetModuleHandleA(name.c_str());

        if (!module)
            FATAL_BREAKPOINT();

        return module;
    }

    inline HMODULE module_unsafe(const std::string &name) {
        return GetModuleHandleA(name.c_str());
    }
}}