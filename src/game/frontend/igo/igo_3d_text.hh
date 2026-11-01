#pragma once

#include "treyarch/mash/string.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_text;

    struct igo_3d_text_vtable {
        igo_3d_text*  (__thiscall* destroy)(igo_3d_text* self, u32 flags);
        void*         reserved_004;
        void          (__thiscall* set_shown)(igo_3d_text* self, bool shown);
        void*         reserved_00c[4];
        void          (__thiscall* set_dimensions)(igo_3d_text* self, f32 w, f32 h);
        void          (__thiscall* set_h_justification)(igo_3d_text* self, i32 just_flags);
        void          (__thiscall* set_v_justification)(igo_3d_text* self, i32 just_flags);
        void          (__thiscall* set_word_wrap)(igo_3d_text* self, bool wrap);
        void*         reserved_02c[4];
        void          (__thiscall* set_text_no_localize)(igo_3d_text* self, mash::string text); // owns the string
        void          (__thiscall* set_font)(igo_3d_text* self, i32 font, i32 mat_type);
        void*         reserved_044;
        void          (__thiscall* set_font_scale)(igo_3d_text* self, f32 scale);
        void*         reserved_04c;
        void          (__thiscall* set_font_color)(igo_3d_text* self, f32 r, f32 g, f32 b);
        void*         reserved_054[3];
        f32           (__thiscall* get_width)(igo_3d_text* self);  // set_dimensions' w
        f32           (__thiscall* get_height)(igo_3d_text* self); // set_dimensions' h
        void*         reserved_068[13];
        void          (__thiscall* set_button_scale)(igo_3d_text* self, f32 scale);
        void*         reserved_0a0[6];
        mash::string* (__thiscall* get_text)(igo_3d_text* self, mash::string* result); // by value
        void*         reserved_0bc[6];
        void          (__thiscall* set_text_position_x)(igo_3d_text* self, f32 x);
        void          (__thiscall* set_text_position_y)(igo_3d_text* self, f32 y);
        void*         reserved_0dc[7];
        void          (__thiscall* set_character_gap)(igo_3d_text* self, f32 gap);
        void          (__thiscall* set_x_scale)(igo_3d_text* self, f32 scale);
        void          (__thiscall* set_word_wrap_line_gap)(igo_3d_text* self, f32 gap);
        void*         reserved_104[5];
        void          (__thiscall* set_shadow)(igo_3d_text* self, bool shadow);
        void          (__thiscall* set_shadow_color)(igo_3d_text* self, f32 r, f32 g, f32 b);
        void          (__thiscall* set_shadow_alpha)(igo_3d_text* self, f32 alpha);
        void*         reserved_124[5];
        void          (__thiscall* render)(igo_3d_text* self); // sub_69D520
        void*         reserved_13c[5];
        u8            (__thiscall* all_lines_revealed)(igo_3d_text* self);
        void*         reserved_154[11];
    };

    class igo_3d_text {
    public:
        igo_3d_text_vtable* vtable;
        u8                  reserved_004[0x50];
        bool                typewriter;         // reveals a character every 1/45 s
        u8                  reserved_055[0x23];

        void destroy() {
            vtable->destroy(this, 1);
        }

        void set_shown(bool shown) {
            vtable->set_shown(this, shown);
        }

        void set_dimensions(f32 w, f32 h) {
            vtable->set_dimensions(this, w, h);
        }

        f32 get_width() {
            return vtable->get_width(this);
        }

        void set_h_justification(i32 just_flags) {
            vtable->set_h_justification(this, just_flags);
        }

        void set_v_justification(i32 just_flags) {
            vtable->set_v_justification(this, just_flags);
        }

        void set_word_wrap(bool wrap) {
            vtable->set_word_wrap(this, wrap);
        }

        void set_text_no_localize(const char* text) { // the slot owns the string built here
            vtable->set_text_no_localize(this, mash::string(text));
        }

        void set_font(i32 font, i32 mat_type) {
            vtable->set_font(this, font, mat_type);
        }

        void set_font_scale(f32 scale) {
            vtable->set_font_scale(this, scale);
        }

        void set_font_color(f32 r, f32 g, f32 b) {
            vtable->set_font_color(this, r, g, b);
        }

        void set_button_scale(f32 scale) {
            vtable->set_button_scale(this, scale);
        }

        mash::string* get_text(mash::string* result) {
            return vtable->get_text(this, result);
        }

        void set_text_position_x(f32 x) {
            vtable->set_text_position_x(this, x);
        }

        void set_text_position_y(f32 y) {
            vtable->set_text_position_y(this, y);
        }

        void set_character_gap(f32 gap) {
            vtable->set_character_gap(this, gap);
        }

        void set_x_scale(f32 scale) {
            vtable->set_x_scale(this, scale);
        }

        void set_word_wrap_line_gap(f32 gap) {
            vtable->set_word_wrap_line_gap(this, gap);
        }

        void set_shadow(bool shadow) {
            vtable->set_shadow(this, shadow);
        }

        void set_shadow_color(f32 r, f32 g, f32 b) {
            vtable->set_shadow_color(this, r, g, b);
        }

        void set_shadow_alpha(f32 alpha) {
            vtable->set_shadow_alpha(this, alpha);
        }

        void render() {
            vtable->render(this);
        }

        u8 all_lines_revealed() {
            return vtable->all_lines_revealed(this);
        }
    };

    ASSERT_SIZEOF  (igo_3d_text_vtable,                         96 * 4);
    ASSERT_OFFSETOF(igo_3d_text_vtable, set_shown,              0x008);
    ASSERT_OFFSETOF(igo_3d_text_vtable, set_dimensions,         0x01C);
    ASSERT_OFFSETOF(igo_3d_text_vtable, set_text_no_localize,   0x03C);
    ASSERT_OFFSETOF(igo_3d_text_vtable, set_font_scale,         0x048);
    ASSERT_OFFSETOF(igo_3d_text_vtable, set_font_color,         0x050);
    ASSERT_OFFSETOF(igo_3d_text_vtable, get_width,              0x060);
    ASSERT_OFFSETOF(igo_3d_text_vtable, get_height,             0x064);
    ASSERT_OFFSETOF(igo_3d_text_vtable, set_button_scale,       0x09C);
    ASSERT_OFFSETOF(igo_3d_text_vtable, get_text,               0x0B8);
    ASSERT_OFFSETOF(igo_3d_text_vtable, set_text_position_x,    0x0D4);
    ASSERT_OFFSETOF(igo_3d_text_vtable, set_character_gap,      0x0F8);
    ASSERT_OFFSETOF(igo_3d_text_vtable, set_word_wrap_line_gap, 0x100);
    ASSERT_OFFSETOF(igo_3d_text_vtable, set_shadow,             0x118);
    ASSERT_OFFSETOF(igo_3d_text_vtable, set_shadow_alpha,       0x120);
    ASSERT_OFFSETOF(igo_3d_text_vtable, render,                 0x138);
    ASSERT_OFFSETOF(igo_3d_text_vtable, all_lines_revealed,     0x150);

    ASSERT_SIZEOF  (igo_3d_text,             0x78);
    ASSERT_OFFSETOF(igo_3d_text, typewriter, 0x54);
} // treyarch
