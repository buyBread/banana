#pragma once

#include <windows.h>

#include "util/memory_reference.hh"
#include "util/types.hh"
#include "treyarch/ngl/scene/scene.hh"

namespace treyarch { namespace ngl {
    ngl::scene* __cdecl init(HWND window);
    ngl::scene* __cdecl present();

    using resource_callback = void* (__cdecl*)(fixed_string* name, u32 type);

    namespace references {
        inline util::memory_reference<u8>   initialized   { 0x011184F0 };
        inline util::memory_reference<u32>  frame_epoch   { 0x011184F4 };
        inline util::memory_reference<HWND> render_window { 0x0111AA90 };

        inline util::memory_reference<ngl::resource_callback> resource_callback { 0x01118510 };
    } // references
}} // treyarch::ngl
