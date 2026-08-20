#pragma once

#include "util/types.hh"

namespace treyarch {
    // lol
    // i didn't know this was a https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3dukmdt/nf-d3dukmdt-makefourcc
    constexpr u32 four_cc(char a,
                          char b,
                          char c,
                          char d = '\0') {

        return  (u32)(u8)a
             | ((u32)(u8)b <<  8)
             | ((u32)(u8)c << 16)
             | ((u32)(u8)d << 24);
    }
} // treyarch