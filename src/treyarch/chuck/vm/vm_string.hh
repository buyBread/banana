#pragma once

#include <cstddef>

#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    // CASTISTR and CASTFSTR write here before copying into a dynamic-array string
    namespace references {
        inline util::memory_reference<char> cast_string_buffer { 0x01124CF0 }; // 1024 bytes
    } // references

    // signed decimal; returns the length
    i32 int_to_string(i32 value, char* buffer);

    // "%.<decimals>f"; returns the length
    u32 float_to_string(f32 value, i32 decimals, char* buffer, size_t buffer_count);
}}} // treyarch::chuck::vm
