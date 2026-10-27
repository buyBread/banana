#pragma once

#include <windows.h>
#include <string>

namespace util {
    inline std::wstring get_env(std::wstring_view name) {
        DWORD size = GetEnvironmentVariableW(name.data(), nullptr, 0);

        if (size == 0)
            return {};

        std::wstring value(size, L'\0');

        GetEnvironmentVariableW(name.data(), value.data(), size);

        value.resize(size - 1); // remove terminating NUL
        
        return value;
    }
} // util