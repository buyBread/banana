#pragma once

#include <windows.h>
#include <cstdio>
#include <mutex>

#include "flags.hh"

namespace util {
    inline void allocate_console() {
#if !ALLOCATE_CONSOLE
        return;
#endif
        static std::once_flag allocate_once;

        std::call_once(allocate_once, []{
            AllocConsole();

            freopen("CONIN$",  "r", stdin);
            freopen("CONOUT$", "w", stdout);
            freopen("CONOUT$", "w", stderr);

            HANDLE handle_output = GetStdHandle(STD_OUTPUT_HANDLE);

            if (handle_output == INVALID_HANDLE_VALUE)
                return;

            DWORD mode = 0;

            if (!GetConsoleMode(handle_output, &mode))
                return;

            mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

            SetConsoleMode(handle_output, mode);
        });  
    }
} // util