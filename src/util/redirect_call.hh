#pragma once

#include <windows.h>

#include "util/types.hh"
#include "banana/logging.hh"

namespace util {
    template <typename fn, size_t n> // template for the static_assert
    bool redirect_call(u32 address, const u8 (&expected_instruction)[n], fn target) {
        static_assert(std::is_pointer_v<fn> && std::is_function_v<std::remove_pointer_t<fn>>, "target must be a function pointer");

        auto call = (u8*)address;

        if (std::memcmp(call, expected_instruction, n) != 0) {
            banana::log.err("unexpected callsite");

            return false;
        }

        i32 displacement = (u8*)target - (call + n);
            
        DWORD previous_protection {};

        if (!VirtualProtect(call, n, PAGE_EXECUTE_READWRITE, &previous_protection)) {
            banana::log.err("failed to redirect call");

            return false;
        }

        // say bye bye
        std::memcpy(call + 1, &displacement, sizeof(displacement));
            
        FlushInstructionCache(GetCurrentProcess(), call, n);

        DWORD unused_protection {};
            
        VirtualProtect(call, n, previous_protection, &unused_protection);

        return true;
    }
}