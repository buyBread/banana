#pragma once

#include "vc8/list.hh"
#include "vc8/map.hh"
#include "game/arch_base_vhandle.hh"
#include "game/frontend/igo/igo_3d_widget_parameters.hh"
#include "game/math/algebra.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_drawable;
    class igo_3d_text;
    class igo_3d_widget;

    enum igo_widget_suppression_condition : u32 {
        suppress_when_paused             = 0x01,
        suppress_when_interface_disabled = 0x02,
        suppress_when_zoom_map_open      = 0x04,
        suppress_when_loading_screen     = 0x08,
        suppress_when_letterbox_visible  = 0x10,
        suppress_when_pauseless_dialog   = 0x20,
    };

    using igo_3d_drawable_test   = u8   (__thiscall*)(igo_3d_drawable* self);
    using igo_3d_drawable_method = void (__thiscall*)(igo_3d_drawable* self);

    // the slots source calls on igo objects without a typed class; widgets have igo_3d_widget_vtable
    struct igo_3d_drawable_vtable {
        void*                  reserved_000[3];
        igo_3d_drawable_method render;
        void*                  reserved_010;
        igo_3d_drawable_method method_014;
        void*                  reserved_018[20];
        igo_3d_drawable_method method_068;
        igo_3d_drawable_method method_06c;
        void*                  reserved_070;
        igo_3d_drawable_test   test_074;
    };

    struct igo_3d_widget_vtable {
        igo_3d_widget* (__thiscall* destroy)(igo_3d_widget* self, u32 flags);
        void           (__thiscall* init)(igo_3d_widget* self);
        void           (__thiscall* update)(igo_3d_widget* self, f32 time_inc);
        void           (__thiscall* render)(igo_3d_widget* self);
        u8             (__thiscall* is_shown)(igo_3d_widget* self);
        void           (__thiscall* set_shown)(igo_3d_widget* self, bool shown);
        void           (__thiscall* set_position)(igo_3d_widget* self, f32 x, f32 y, f32 z);
        void           (__thiscall* set_center_pos)(igo_3d_widget* self, f32 x, f32 y);
        void           (__thiscall* get_position)(igo_3d_widget* self, f32* x, f32* y);
        void           (__thiscall* set_position_z)(igo_3d_widget* self, f32 z);
        void           (__thiscall* set_rotation_angle)(igo_3d_widget* self, f32 angle);
        void           (__thiscall* set_tilt_angle)(igo_3d_widget* self, f32 angle);
        void           (__thiscall* set_spin_angle)(igo_3d_widget* self, f32 angle);
        void           (__thiscall* set_scale)(igo_3d_widget* self, f32 scale);
        void           (__thiscall* set_scale_offset)(igo_3d_widget* self, f32 x, f32 y, f32 z);
        void           (__thiscall* set_alpha)(igo_3d_widget* self, f32 alpha);
        void           (__thiscall* set_widescreen_alignment)(igo_3d_widget* self, igo_3d_widget_parameters::e_wide_align_flags alignment);
        void           (__thiscall* set_suppression_mask)(igo_3d_widget* self, u32 mask);
        void           (__thiscall* set_color)(igo_3d_widget* self, f32 r, f32 g, f32 b); // each clamped into [0, 1]
        void           (__thiscall* set_uv_offset)(igo_3d_widget* self, f32 u, f32 v);
        void*          reserved_050[12];
        u32            (__thiscall* suppression_mask)(igo_3d_widget* self);
        void*          reserved_084[7];
        void           (__thiscall* start_timed_move)(igo_3d_widget* self, f32 duration, f32 from_x, f32 from_y, f32 from_z, f32 to_x, f32 to_y, f32 to_z, i32 curve);
        void           (__thiscall* render_entity)(igo_3d_widget* self); // spawns the entity first
        void           (__thiscall* spawn_entity)(igo_3d_widget* self);
        void*          reserved_0ac[11];
    };

    class igo_3d_drawable {
    public:
        igo_3d_drawable_vtable* vtable;

        void render() {
            vtable->render(this);
        }
    };

    class igo_3d_widget : public igo_3d_drawable {
    public:
        u8                            reserved_004[0x0C];
        igo_3d_widget_parameters*     parameters;
        u32                           flags;               // bit 0 shown, bit 1 the parameters live in a mashed resource
        f32                           alpha;
        f32                           scale_factor;
        f32                           alpha_factor;
        vector3d                      scale_offset;
        vector3d                      color;
        vector2d                      uv_offset;
        u8                            reserved_044[0x08];
        arch_base_vhandle             entity_vh;
        u8                            unk_050;             // latched by spawning the entity
        u8                            reserved_051[0x0F];
        matrix4x4                     trans_matrix;
        vc8::map<i32, igo_3d_text*>   unk_0a0;
        vc8::map<i32, igo_3d_widget*> unk_0ac;
        vc8::list<igo_3d_text*>       unk_0b8;
        vc8::list<igo_3d_widget*>     unk_0c4;
        u8                            reserved_0d0[0x08];
        bool                          faded_in;            // either fade sets it, the end of a fade out clears it
        bool                          fading_in;
        bool                          fading_out;
        u8                            pad_0db;
        f32                           fade_in_remaining;
        f32                           fade_in_duration;
        f32                           fade_out_remaining;
        f32                           fade_out_duration;
        f32                           fade_in_alpha_span;
        f32                           fade_in_alpha_base;
        f32                           fade_out_alpha_span;
        f32                           fade_out_alpha_base;
        u8                            reserved_0fc[0x20];
        u32                           unk_11c;             // the parameters' unk_94
        vector3d                      move_from;
        vector3d                      move_to;
        f32                           move_elapsed;
        f32                           move_duration;
        bool                          moving;
        u8                            pad_141[0x03];
        i32                           move_curve;
        u8                            reserved_148[0x08];

        igo_3d_widget_vtable* get_vtable() const {
            return (igo_3d_widget_vtable*)vtable;
        }

        u8 is_shown() {
            return get_vtable()->is_shown(this);
        }

        void set_shown(bool shown) {
            get_vtable()->set_shown(this, shown);
        }

        void set_position(f32 x, f32 y, f32 z) {
            get_vtable()->set_position(this, x, y, z);
        }

        void set_center_pos(f32 x, f32 y) {
            get_vtable()->set_center_pos(this, x, y);
        }

        void get_position(f32* x, f32* y) {
            get_vtable()->get_position(this, x, y);
        }

        void set_rotation_angle(f32 angle) {
            get_vtable()->set_rotation_angle(this, angle);
        }

        void set_tilt_angle(f32 angle) {
            get_vtable()->set_tilt_angle(this, angle);
        }

        void set_spin_angle(f32 angle) {
            get_vtable()->set_spin_angle(this, angle);
        }

        void set_scale(f32 scale) {
            get_vtable()->set_scale(this, scale);
        }

        void set_scale_offset(f32 x, f32 y, f32 z) {
            get_vtable()->set_scale_offset(this, x, y, z);
        }

        void set_alpha(f32 value) {
            get_vtable()->set_alpha(this, value);
        }

        void set_widescreen_alignment(igo_3d_widget_parameters::e_wide_align_flags alignment) {
            get_vtable()->set_widescreen_alignment(this, alignment);
        }

        void set_color(f32 r, f32 g, f32 b) {
            get_vtable()->set_color(this, r, g, b);
        }

        void set_uv_offset(f32 u, f32 v) {
            get_vtable()->set_uv_offset(this, u, v);
        }

        u32 suppression_mask() {
            return get_vtable()->suppression_mask(this);
        }

        void fade_in (f32 duration, f32 from_alpha, f32 to_alpha, f32 delay);
        void fade_out(f32 duration, f32 from_alpha, f32 to_alpha, f32 delay); // cancels a fade in

        void kill_layer_0_anims();
        bool is_anim_playing(); // layer 0's newest anim hasn't reached its end

        i32 get_mesh_index(const string_hash &nodename); // invalid_conglom_member without a conglomerate
        i32 find_child    (mash::string*      name);     // always -2; destroys the name
    };

    ASSERT_SIZEOF(igo_3d_drawable, 0x004);

    ASSERT_SIZEOF  (igo_3d_drawable_vtable,                     0x078);
    ASSERT_OFFSETOF(igo_3d_drawable_vtable, render,             0x00C);
    ASSERT_OFFSETOF(igo_3d_drawable_vtable, method_014,         0x014);
    ASSERT_OFFSETOF(igo_3d_drawable_vtable, method_068,         0x068);
    ASSERT_OFFSETOF(igo_3d_drawable_vtable, method_06c,         0x06C);
    ASSERT_OFFSETOF(igo_3d_drawable_vtable, test_074,           0x074);

    ASSERT_SIZEOF  (igo_3d_widget_vtable,                       54 * 4);
    ASSERT_OFFSETOF(igo_3d_widget_vtable, render,               0x00C);
    ASSERT_OFFSETOF(igo_3d_widget_vtable, set_shown,            0x014);
    ASSERT_OFFSETOF(igo_3d_widget_vtable, set_uv_offset,        0x04C);
    ASSERT_OFFSETOF(igo_3d_widget_vtable, suppression_mask,     0x080);
    ASSERT_OFFSETOF(igo_3d_widget_vtable, start_timed_move,     0x0A0);
    ASSERT_OFFSETOF(igo_3d_widget_vtable, spawn_entity,         0x0A8);

    ASSERT_SIZEOF  (igo_3d_widget,                              0x150);
    ASSERT_OFFSETOF(igo_3d_widget, parameters,                  0x010);
    ASSERT_OFFSETOF(igo_3d_widget, flags,                       0x014);
    ASSERT_OFFSETOF(igo_3d_widget, scale_offset,                0x024);
    ASSERT_OFFSETOF(igo_3d_widget, color,                       0x030);
    ASSERT_OFFSETOF(igo_3d_widget, uv_offset,                   0x03C);
    ASSERT_OFFSETOF(igo_3d_widget, entity_vh,                   0x04C);
    ASSERT_OFFSETOF(igo_3d_widget, unk_050,                     0x050);
    ASSERT_OFFSETOF(igo_3d_widget, trans_matrix,                0x060);
    ASSERT_OFFSETOF(igo_3d_widget, unk_0a0,                     0x0A0);
    ASSERT_OFFSETOF(igo_3d_widget, unk_0c4,                     0x0C4);
    ASSERT_OFFSETOF(igo_3d_widget, faded_in,                    0x0D8);
    ASSERT_OFFSETOF(igo_3d_widget, fade_in_remaining,           0x0DC);
    ASSERT_OFFSETOF(igo_3d_widget, fade_out_alpha_base,         0x0F8);
    ASSERT_OFFSETOF(igo_3d_widget, unk_11c,                     0x11C);
    ASSERT_OFFSETOF(igo_3d_widget, move_from,                   0x120);
    ASSERT_OFFSETOF(igo_3d_widget, move_elapsed,                0x138);
    ASSERT_OFFSETOF(igo_3d_widget, moving,                      0x140);
    ASSERT_OFFSETOF(igo_3d_widget, move_curve,                  0x144);
} // treyarch
