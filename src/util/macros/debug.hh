#pragma once

#include <windows.h>
#include <exception>

#define FATAL_BREAKPOINT()                   \
    do {                                     \
        if (::IsDebuggerPresent()) {         \
            __debugbreak();                  \
        } std::terminate();                  \
    } while (false)