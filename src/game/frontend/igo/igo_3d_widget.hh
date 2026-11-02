#pragma once

#include "vc8/list.hh"
#include "vc8/map.hh"
#include "game/arch_base_vhandle.hh"
#include "game/frontend/igo/igo_3d_widget_parameters.hh"
#include "game/math/algebra.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mm/mm.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_text;

    enum igo_widget_suppression_condition : u32 {
        suppress_when_paused             = 0x01,
        suppress_when_interface_disabled = 0x02,
        suppress_when_zoom_map_open      = 0x04,
        suppress_when_loading_screen     = 0x08,
        suppress_when_letterbox_visible  = 0x10,
        suppress_when_pauseless_dialog   = 0x20,
    };

    class alignas(16) igo_3d_widget {

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
        i32                           unk_044;
        i32                           unk_048;
        arch_base_vhandle             entity_vh;
        bool                          entity_spawned;      // spawn_entity latches it, release_entity clears it
        u8                            reserved_051[0x0F];
        matrix4x4                     trans_matrix;
        vc8::map<i32, igo_3d_text*>   member_texts;        // by conglomerate member index
        vc8::map<i32, igo_3d_widget*> member_widgets;      // by conglomerate member index
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

        explicit igo_3d_widget(mash::generating_vtable_version so_do_nothing) : member_texts(so_do_nothing),
                                                                                member_widgets(so_do_nothing),
                                                                                unk_0b8(so_do_nothing),
                                                                                unk_0c4(so_do_nothing) {}

        virtual ~igo_3d_widget();

        // inlined into the deleting destructors
        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
        }

        virtual void init();
        virtual void update(f32 time_inc);
        virtual void render();
        virtual u8   is_shown();
        virtual void set_shown(bool shown);

        virtual void set_position(f32 x, f32 y, f32 z);
        virtual void set_center_pos(f32 x, f32 y);
        virtual void get_position(f32* x, f32* y);
        virtual void set_position_z(f32 z);
        virtual void set_rotation_angle(f32 angle);
        virtual void set_tilt_angle(f32 angle);
        virtual void set_spin_angle(f32 angle);
        virtual void set_scale(f32 scale);
        virtual void set_scale_offset(f32 x, f32 y, f32 z);
        virtual void set_alpha(f32 value);
        virtual void set_widescreen_alignment(igo_3d_widget_parameters::e_wide_align_flags alignment);
        virtual void set_suppression_mask(u32 mask);
        virtual void set_color(f32 r, f32 g, f32 b);
        virtual void set_uv_offset(f32 u, f32 v);
        virtual void unk_020(i32 value);
        virtual void unk_021(i32 value);

        virtual f32  get_position_x();
        virtual f32  get_position_y();
        virtual f32  get_position_z();
        virtual f32  get_rotation_angle();
        virtual f32  get_tilt_angle();
        virtual f32  get_spin_angle();
        virtual f32  get_scale();
        virtual f32  get_alpha();
        virtual bool unk_030();

        virtual igo_3d_widget_parameters::e_wide_align_flags get_widescreen_alignment();

        virtual u32  suppression_mask();
        virtual i32  unk_033();
        virtual i32  unk_034();
        virtual void set_scale_factor(f32 factor);
        virtual f32  get_scale_factor();
        virtual void set_alpha_factor(f32 factor);
        virtual f32  get_alpha_factor();

        virtual void play_sound(string_hash sound);
        virtual void start_timed_move(f32 duration, f32 from_x, f32 from_y, f32 from_z, f32 to_x, f32 to_y, f32 to_z, i32 curve);
        virtual void render_entity();
        virtual void spawn_entity();

        virtual i32   render_mesh                (i32 unk_0, void* unk_1, i32 unk_2, i32 unk_3);
        virtual i32   render_congomerate_mesh    (i32 unk_0, i32 unk_1, void* unk_2, i32 unk_3, i32 unk_4, i32 unk_5);
        virtual i32   render_mesh_from_parameters(i32 unk_0, i32 unk_1, i32 unk_2, i32 unk_3);
        virtual i32   render_text                (i32 unk_0, i32 unk_1, i32 unk_2, i32 unk_3, i32 unk_4);
        virtual i32   render_child_widget        (i32 unk_0, i32 unk_1, i32 unk_2, i32 unk_3, i32 unk_4);
        virtual i32   render_value_mesh          (i32 unk_0, i32 unk_1, i32 unk_2, i32 unk_3, i32 unk_4, i32 unk_5);
        virtual void  render_value_text          (i32 unk_0, i32 unk_1, i32 unk_2, i32 unk_3, i32 unk_4);
        virtual i32   create_child_texts();
        virtual void* create_child_widgets();
        virtual void  unk_052();
        virtual void  unk_053();

        void fade_in (f32 duration, f32 from_alpha, f32 to_alpha, f32 delay);
        void fade_out(f32 duration, f32 from_alpha, f32 to_alpha, f32 delay); // cancels a fade in

        void kill_layer_0_anims();
        void reset_anim_speeds();
        bool is_anim_playing(); // layer 0's newest anim hasn't reached its end
        void reset_entity_po(); // puts the entity back at trans_matrix
        void release_entity();  // destroys the spawned entity

        i32 get_mesh_index(const string_hash &nodename); // invalid_conglom_member without a conglomerate
        i32 find_child    (mash::string*      name);     // always -2; destroys the name
    };

    ASSERT_SIZEOF  (igo_3d_widget,                              0x150);
    ASSERT_OFFSETOF(igo_3d_widget, parameters,                  0x010);
    ASSERT_OFFSETOF(igo_3d_widget, flags,                       0x014);
    ASSERT_OFFSETOF(igo_3d_widget, scale_offset,                0x024);
    ASSERT_OFFSETOF(igo_3d_widget, color,                       0x030);
    ASSERT_OFFSETOF(igo_3d_widget, uv_offset,                   0x03C);
    ASSERT_OFFSETOF(igo_3d_widget, unk_044,                     0x044);
    ASSERT_OFFSETOF(igo_3d_widget, unk_048,                     0x048);
    ASSERT_OFFSETOF(igo_3d_widget, entity_vh,                   0x04C);
    ASSERT_OFFSETOF(igo_3d_widget, entity_spawned,              0x050);
    ASSERT_OFFSETOF(igo_3d_widget, trans_matrix,                0x060);
    ASSERT_OFFSETOF(igo_3d_widget, member_texts,                0x0A0);
    ASSERT_OFFSETOF(igo_3d_widget, member_widgets,              0x0AC);
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
