#pragma once

#include "banana/util/types.hh"
#include "banana/util/macros/debug.hh"
#include "banana/util/gimmie/method.hh"

namespace util { namespace gimmie {
    template <typename T>
    inline T fn(u32 address) noexcept {
        if (!address)
            FATAL_BREAKPOINT();

        return (T)address;
    }
    template <typename T>
    inline T fn(HMODULE module, const std::string &name) noexcept {
        auto address = gimmie::method(module, name);

        if (!address)
            FATAL_BREAKPOINT();

        return (T)address;
    }

    template <typename T>
    inline T fn_unsafe(u32 address) noexcept {
        return (T)address;
    }
    template <typename T>
    inline T fn_unsafe(HMODULE module, const std::string &name) noexcept {
        return (T)gimmie::method_unsafe(module, name);
    }
}} // util::gimmie