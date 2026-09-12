#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    struct arch_base_vhandle_entry {
        void* object;
        u32   handle;
    };

    struct arch_base_vhandle {
        u32 value;

                 constexpr arch_base_vhandle()          noexcept : value(0)     {}
        explicit constexpr arch_base_vhandle(u32 value) noexcept : value(value) {}

        void* resolve() const noexcept {
            if (!value)
                return nullptr;

            static util::memory_reference<arch_base_vhandle_entry*> entries { 0x00FFEA24 };

            arch_base_vhandle_entry &entry = entries.read()[value & 0x7FFF];

            return entry.handle == value ? entry.object : nullptr;
        }

        constexpr u32 hash() const noexcept { return value; }
        constexpr bool null() const noexcept { return value == 0; }

        friend constexpr bool operator==(arch_base_vhandle left, arch_base_vhandle right) noexcept {
            return left.value == right.value;
        }

        friend constexpr bool operator<(arch_base_vhandle left, arch_base_vhandle right) noexcept {
            return left.value < right.value;
        }
    };

    ASSERT_SIZEOF(arch_base_vhandle, 0x04);
} // treyarch
