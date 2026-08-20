#pragma once

#include <d3d9.h>
#include <windows.h>

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace d3d9 {
    struct display_mode {
        i32 width;
        i32 height;
        u8  progressive;
        u8  pal;
        u8  widescreen;
        u8  pad_00b[5];
    };

    struct display_dimensions {
        i32 width;
        i32 height;
    };

    void select_default_display_mode();
    
    HWND initialize_render_window();
    void initialize_presentation_parameters();
    
    LRESULT CALLBACK window_procedure(HWND   window,
                                      UINT   message,
                                      WPARAM word,
                                      LPARAM long_word);

    namespace references {
        inline ::util::memory_reference<display_mode>       display_modes          { 0x00F53C10 };
        inline ::util::memory_reference<D3DFORMAT>          back_buffer_format     { 0x00F528D4 };
        inline ::util::memory_reference<u8>                 windowed               { 0x00F53514 };
        inline ::util::memory_reference<u8>                 owns_window            { 0x00F53515 };
        inline ::util::memory_reference<u32>                selected_display_index { 0x0111850C };
        inline ::util::memory_reference<display_mode>       selected_display_mode  { 0x01118550 };
        inline ::util::memory_reference<u8>                 window_inactive        { 0x0111AA94 };
        inline ::util::memory_reference<u8>                 force_reference_device { 0x0111AA96 };
        inline ::util::memory_reference<display_dimensions> window_dimensions      { 0x0111AA98 };
        inline ::util::memory_reference<WINDOWPLACEMENT>    window_placement       { 0x0111AAA0 };
    } // references

    ASSERT_SIZEOF  (display_mode,              0x10);
    ASSERT_OFFSETOF(display_mode, width,       0x00);
    ASSERT_OFFSETOF(display_mode, height,      0x04);
    ASSERT_OFFSETOF(display_mode, progressive, 0x08);
    ASSERT_OFFSETOF(display_mode, pal,         0x09);
    ASSERT_OFFSETOF(display_mode, widescreen,  0x0A);

    ASSERT_SIZEOF(display_dimensions, 0x08);
}}} // treyarch::ngl::d3d9
