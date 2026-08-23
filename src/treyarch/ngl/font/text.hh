#pragma once

#include "treyarch/ngl/font/font.hh"

namespace treyarch { namespace ngl {
    void get_string_dimensions(      font* font_data,
                               const char* text,
                                     u32*  width,
                                     u32*  height,
                                     f32   scale_x,
                                     f32   scale_y);

    void list_add_string(      font* font_data,
                         const char* text,
                               f32   x,
                               f32   y,
                               f32   z,
                               u32   color,
                               f32   scale_x,
                               f32   scale_y);
}} // treyarch::ngl
