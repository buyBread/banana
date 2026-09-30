#pragma once

#include <windows.h>

#include "treyarch/ngl/frame_lock.hh"
#include "treyarch/shared/mash/string.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    int WINAPI WinMain(HINSTANCE instance,
                       HINSTANCE previous_instance,
                       LPSTR     command_line,
                       int       show_command);

    namespace references {
        inline util::memory_reference<u8>                processor_count    { 0x00F4E294 };
        // restored by the movie player after playback (sub_6ABB30)
        inline util::memory_reference<ngl::e_frame_lock> default_frame_lock { 0x00F4E2D0 };
        inline util::memory_reference<u8>                window_active      { 0x011136F8 };
        inline util::memory_reference<mash::string>      data_root          { 0x01113794 };
        inline util::memory_reference<mash::string>      image_root         { 0x011137BC };
    } // references
} // treyarch
