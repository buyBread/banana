#pragma once

#include <cstring>
#include <type_traits>

#include <windows.h>

#include "util/types.hh"
#include "banana/logging.hh"

namespace util {
    template <typename fn, size_t n> // template for the static_assert
    bool redirect_call(u32 address, const u8 (&expected_instruction)[n], fn target) {
        constexpr bool function_pointer = std::is_pointer_v<fn> &&
                                          std::is_function_v<std::remove_pointer_t<fn>>;

        constexpr bool member_function_pointer = std::is_member_function_pointer_v<fn>;

        static_assert(function_pointer || member_function_pointer, "target must be a function or a member-function pointer");

        u8* target_address {};

        if constexpr (function_pointer)
            target_address = (u8*)target;
        else {
            static_assert(sizeof(fn) == sizeof(target_address));

            std::memcpy(&target_address, &target, sizeof(target_address));
        }

        auto call = (u8*)address;

        if (std::memcmp(call, expected_instruction, n) != 0) {
            banana::log.err("unexpected callsite");

            return false;
        }

        i32 displacement = target_address - (call + n);
            
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
