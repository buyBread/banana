#pragma once

#include <windows.h>
#include <format>

#include "util/macros/debug.hh"
#include "util/gimmie/module.hh"

namespace util { namespace gimmie {
    inline void* method(HMODULE module, const std::string &name) {
        auto address = GetProcAddress(module, name.c_str());

        if (!address)
            FATAL_BREAKPOINT();

        return (void*)address;
    }
    inline void* method(const std::string &module, const std::string &name) {
        auto address = GetProcAddress(gimmie::module(module), name.c_str());

        if (!address)
            FATAL_BREAKPOINT();

        return (void*)address;
    }

    inline void* method_unsafe(HMODULE module, const std::string &name) {
        return (void*)GetProcAddress(module, name.c_str());
    }
    inline void* method_unsafe(const std::string &module, const std::string &name) {
        return (void*)GetProcAddress(gimmie::module_unsafe(module), name.c_str());
    }
}}