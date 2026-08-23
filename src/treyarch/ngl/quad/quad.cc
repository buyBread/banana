#include <cstring>

#include "treyarch/ngl/quad/quad.hh"

using namespace treyarch;

void ngl::init_quad(quad* value) {
    std::memset(value, 0, sizeof(quad));

    value->vertices[0].color = 0xFFFFFFFF;
    value->vertices[1].color = 0xFFFFFFFF;
    value->vertices[2].color = 0xFFFFFFFF;
    value->vertices[3].color = 0xFFFFFFFF;

    value->vertices[0].u = 0.0f;
    value->vertices[0].v = 0.0f;
    value->vertices[1].u = 1.0f;
    value->vertices[1].v = 0.0f;
    value->vertices[2].u = 0.0f;
    value->vertices[2].v = 1.0f;
    value->vertices[3].u = 1.0f;
    value->vertices[3].v = 1.0f;

    value->map_flags  = 49;
    value->blend_mode = 0x0000000506C10000ULL;
}

void ngl::set_quad_rect(quad* value,
                        f32   left,
                        f32   top,
                        f32   right,
                        f32   bottom) {
                            
    value->vertices[0].x = left;
    value->vertices[0].y = top;
    value->vertices[1].x = right;
    value->vertices[1].y = top;
    value->vertices[2].x = left;
    value->vertices[2].y = bottom;
    value->vertices[3].x = right;
    value->vertices[3].y = bottom;
}

void ngl::set_quad_color(quad* value, u32 color) {
    value->vertices[0].color = color;
    value->vertices[1].color = color;
    value->vertices[2].color = color;
    value->vertices[3].color = color;
}

void ngl::set_quad_z(quad* value, f32 z) {
    value->z = z;
}
