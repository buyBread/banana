#pragma once

#include "vc8/list.hh"
#include "game/frontend/igo/igo_3d_text.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_script_widget : public igo_3d_widget {

    public:
        inline static u32 instance_counter = 1;

        f32                     continuous_rotation;
        f32                     continuous_tilt;
        f32                     continuous_spin;
        vc8::list<igo_3d_text*> texts;
        bool                    is_child;            // not drawn on its own while a parent has it
        u8                      pad_169[0x03];
        f32                     max_timer;
        f32                     timer;               // runs up to max_timer
        string_hash             sound;
        string_hash             text_parameters_key; // every text is built with it
        bool                    sound_played;
        bool                    persistent;          // drawn while the interface is disabled
        u8                      pad_17e[0x02];
        u32                     instance_id;
        u8                      reserved_184[0x0C];

        igo_3d_script_widget(string_hash widget_key, string_hash text_key);

        ~igo_3d_script_widget() override;

        void init() override;
        void update(f32 time_inc) override;

        void render_child_widget(igo_3d_widget*          child,
                                 const matrix4x4        &trans_matrix,
                                 const matrix4x4        &rel_pos_matrix,
                                 ngl::mesh_params*       mesh_params,
                                 ngl::shader_param_set*  shader_params) override;

        virtual void unk_054(f32 time_inc);
        virtual void set_continuous_rotation(f32 rate);
        virtual void set_continuous_tilt(f32 rate);
        virtual void set_continuous_spin(f32 rate);
        virtual void attach_child_widget(igo_3d_script_widget* child, i32 member);
        virtual void detach_child_widget(igo_3d_script_widget* child);
        virtual bool get_is_child();
        virtual u32  get_instance_id();

        igo_3d_text* get_text(i32 id); // made on first use; -2 answers null
    };

    ASSERT_SIZEOF  (igo_3d_script_widget,                      0x190);
    ASSERT_OFFSETOF(igo_3d_script_widget, continuous_rotation, 0x150);
    ASSERT_OFFSETOF(igo_3d_script_widget, texts,               0x15C);
    ASSERT_OFFSETOF(igo_3d_script_widget, is_child,            0x168);
    ASSERT_OFFSETOF(igo_3d_script_widget, max_timer,           0x16C);
    ASSERT_OFFSETOF(igo_3d_script_widget, timer,               0x170);
    ASSERT_OFFSETOF(igo_3d_script_widget, sound,               0x174);
    ASSERT_OFFSETOF(igo_3d_script_widget, text_parameters_key, 0x178);
    ASSERT_OFFSETOF(igo_3d_script_widget, sound_played,        0x17C);
    ASSERT_OFFSETOF(igo_3d_script_widget, persistent,          0x17D);
    ASSERT_OFFSETOF(igo_3d_script_widget, instance_id,         0x180);
} // treyarch
