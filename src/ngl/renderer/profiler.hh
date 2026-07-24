#pragma once

#include <cstdint>

#include "../../util.hh"

namespace banana { namespace NGL { namespace renderer {
namespace profiler {
    /* 
        padding:
        - 0x04-0x0b
        - 0x10-0x13
        - 0x18
        - 0x1b
    */

    // 0x0112384C+28 is the temporary struct
    // 0x01123820+28 is the "defaults" that get copied back at the end of each frame

    MEMORY_REFERENCE<uint8_t>  framerate_panel { 0x01123820 }; // framerate enum
    MEMORY_REFERENCE<uint8_t>  frametime_graph { 0x01123821 }; // bool
    MEMORY_REFERENCE<uint8_t>  flag_02         { 0x01123822 };
    MEMORY_REFERENCE<uint8_t>  flag_03         { 0x01123823 };
    MEMORY_REFERENCE<uint32_t> flag_0c         { 0x0112382C };
    MEMORY_REFERENCE<uint8_t>  flag_14         { 0x01123834 };
    MEMORY_REFERENCE<uint8_t>  flag_15         { 0x01123835 };
    MEMORY_REFERENCE<uint8_t>  flag_16         { 0x01123836 };
    MEMORY_REFERENCE<uint8_t>  flag_17         { 0x01123837 };
    MEMORY_REFERENCE<uint8_t>  flag_19         { 0x01123839 }; // frame log (dead in retail)
    MEMORY_REFERENCE<uint8_t>  flag_1a         { 0x0112383A };

    namespace frame_data {
        MEMORY_REFERENCE<float>    framerate    { 0x01123900 };
        MEMORY_REFERENCE<float>    cpu_time     { 0x01123910 }; // ms
        MEMORY_REFERENCE<float>    gpu_time     { 0x0112390C }; // ms
        MEMORY_REFERENCE<float>    scene_build  { 0x01123918 }; // ms
        MEMORY_REFERENCE<float>    scene_submit { 0x0112391C }; // ms
        MEMORY_REFERENCE<uint32_t> polygons     { 0x01123920 };
        MEMORY_REFERENCE<uint32_t> nodes        { 0x01123924 };
        MEMORY_REFERENCE<uint32_t> list_cur     { 0x011238C8 };
        MEMORY_REFERENCE<uint32_t> list_max     { 0x011238CC };
        MEMORY_REFERENCE<uint32_t> geom_cur     { 0x011238D0 }; // dead?
        MEMORY_REFERENCE<uint32_t> geom_max     { 0x011238D4 }; // dead?
    } // frame_data

    enum e_framerate {
        off = 0,
        full,
        compact,
    };
} // profiler
}}} // banana::NGL::renderer