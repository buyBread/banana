#include <cstdio>
#include <cstring>

#include "treyarch/chuck/vm/vm_string.hh"

using namespace treyarch;
using namespace treyarch::chuck::vm;

// sub_A24B30
i32 chuck::vm::int_to_string(i32 value, char* buffer) {
    i32 length    = 0;
    i32 remaining = value < 0 ? -value : value;

    do {
        buffer[length++] = (char)(remaining % 10 + '0');

        remaining /= 10;
    } while (remaining);

    if (value < 0)
        buffer[length++] = '-';

    for (char* left = buffer, *right = buffer + length - 1; left < right; ++left, --right) {
        char swapped = *left;

        *left  = *right;
        *right = swapped;
    }

    buffer[length] = 0;

    return length;
}

// sub_A24BA0
u32 chuck::vm::float_to_string(f32 value, i32 decimals, char* buffer, size_t buffer_count) {
    char format[32];

    _snprintf(format, 30, "%%.%df", decimals);
    format[29] = 0;

    _snprintf(buffer, buffer_count, format, value);
    buffer[buffer_count - 1] = 0;

    return (u32)std::strlen(buffer);
}
