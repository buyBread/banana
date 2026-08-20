#pragma once

#include <windows.h>
#include <format>

#include "util/macros/debug.hh"

namespace util { namespace gimmie {
    inline HMODULE dll(const std::string &name) {
        char path[MAX_PATH]; GetSystemDirectoryA(path, MAX_PATH);

        // try system32 first
        auto module = LoadLibraryA(std::format("{}\\{}", path, name).c_str());

        if (!module) {
            module = LoadLibraryA(name.c_str());

            if (!module)
                FATAL_BREAKPOINT();
        }

        return module;
    }

    inline HMODULE dll_unsafe(const std::string &name) {
        return LoadLibraryA(name.c_str());
    }
}}