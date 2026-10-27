#pragma once

#include <windows.h>
#include <cstring>
#include <type_traits>
#include <cassert>

#include "banana/util/types.hh"
#include "banana/logging.hh"

template <std::size_t N>
struct std::formatter<u8[N]> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const u8 (&arr)[N], std::format_context& ctx) const {
        auto out = ctx.out();
        
        for (std::size_t i = 0; i < N; ++i) {
            out = std::format_to(out, "{:02X}", arr[i]);

            if (i + 1 != N)
                *out++ = ' ';
        }

        return out;
    }
};

namespace util {
    namespace detail {
        template <typename fn>
        u8* code_address(fn target) {
            constexpr bool function_pointer = std::is_pointer_v<fn> &&
                                              std::is_function_v<std::remove_pointer_t<fn>>;

            constexpr bool member_function_pointer = std::is_member_function_pointer_v<fn>;

            static_assert(function_pointer || member_function_pointer, "target must be a function or a member-function pointer");

            u8* address {};

            if constexpr (function_pointer)
                address = (u8*)target;
            else {
                static_assert(sizeof(fn) == sizeof(address));

                std::memcpy(&address, &target, sizeof(address));
            }

            return address;
        }

        inline bool patch_code(const char* patcher, u8* code, const void* bytes, size_t size) {
            DWORD previous_protection {};

            if (!VirtualProtect(code, size, PAGE_EXECUTE_READWRITE, &previous_protection)) {
                banana::log.err("{}: failed to change protection", patcher);

                return false;
            }

            // say bye bye
            std::memcpy(code, bytes, size);

            FlushInstructionCache(GetCurrentProcess(), code, size);

            DWORD unused_protection {};

            VirtualProtect(code, size, previous_protection, &unused_protection);

            return true;
        }
    } // detail

    template <typename fn, size_t n> // template for the static_assert
    bool redirect_rel32(u32 address, const u8 (&expected_instruction)[n], fn target) {
        banana::log.dbg("redirect_rel32: patching {} @ {:08X}", expected_instruction, address);

        assert(expected_instruction[0] == 0xE8 || expected_instruction[0] == 0xE9);

        static_assert(n == 5, "callsite is invalid");

        u8* target_address = detail::code_address(target);

        auto call = (u8*)address;

        if (std::memcmp(call, expected_instruction, n) != 0) {
            banana::log.err("redirect_rel32: unexpected {} site", expected_instruction[0] == 0xE8 ? "call" : "branch");

            return false;
        }

        i32 displacement = target_address - (call + n);

        return detail::patch_code("redirect_rel32", call + 1, &displacement, sizeof(displacement));
    }

    // overwrites a function's entry with a jmp to target; for functions called from too many sites to patch each
    template <typename fn, size_t n>
    bool redirect_entry(u32 address, const u8 (&expected_prologue)[n], fn target) {
        banana::log.dbg("redirect_entry: patching {} @ {:08X}", expected_prologue, address);

        static_assert(n >= 5, "prologue is too short for a jmp");

        u8* target_address = detail::code_address(target);

        auto entry = (u8*)address;

        if (std::memcmp(entry, expected_prologue, n) != 0) {
            banana::log.err("redirect_entry: unexpected prologue");

            return false;
        }

        i32 displacement = target_address - (entry + 5);

        u8 jump[5] { 0xE9 };

        std::memcpy(jump + 1, &displacement, sizeof(displacement));

        return detail::patch_code("redirect_entry", entry, jump, sizeof(jump));
    }
}
