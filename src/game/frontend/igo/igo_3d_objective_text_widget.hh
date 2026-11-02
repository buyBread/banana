#pragma once

#include "vc8/list.hh"
#include "game/frontend/igo/igo_3d_text.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/frontend/message_text.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/virtual_base.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_objective_text_widget : public igo_3d_widget {

    public:
        struct queue_entry {
            mash::string text;
            f32          unk_0c;
        };

        vc8::list<queue_entry> queue;
        message_text*          shown_message;
        igo_3d_text*           text;
        bool                   suspended;
        bool                   unk_165;          // set on the hint text
        u8                     pad_166[0x02];
        i32                    overlay_2_member; // "ui_hud_obj_body_overlay_2"
        i32                    overlay_3_member; // "ui_hud_obj_body_overlay_3"
        i32                    overlay_4_member; // "ui_hud_obj_body_overlay_4"
        u8                     reserved_174[0x0C];

        explicit igo_3d_objective_text_widget(mash::generating_vtable_version so_do_nothing) : igo_3d_widget(so_do_nothing),
                                                                                               queue(so_do_nothing) {}

        ~igo_3d_objective_text_widget() override;

        // writes the source vptr over a widget the retail constructor built
        void stamp_source_vtable() {
            *(mash::vtable_value_t*)this = mash::generate_vtable<igo_3d_objective_text_widget>();
        }

        void init() override;
        void update(f32 time_inc) override;
        void render() override;
        void spawn_entity() override;

        void render_congomerate_mesh(ngl::mesh*              mesh,
                                     i32                     index,
                                     const matrix4x4        &trans_matrix,
                                     const matrix4x4        &rel_pos_matrix,
                                     ngl::mesh_params*       mesh_params,
                                     ngl::shader_param_set*  shader_params) override; // the overlay members only draw at full alpha

        virtual void suspend(bool on);
        virtual void show_message(mash::string text, f32 duration); // owns the string
        virtual void show_message_by_id(u32 text_id, f32 duration);
        virtual void unk_057(message_text* message);
        virtual bool unk_058();

        void show_message(const char* message, f32 duration) {
            show_message(mash::string(message), duration);
        }

        void clear(bool fade); // fading lets the shown message run out, either way the queue empties
    };

    class igo_3d_hint_text_widget : public igo_3d_objective_text_widget {

    public:
        explicit igo_3d_hint_text_widget(mash::generating_vtable_version so_do_nothing) : igo_3d_objective_text_widget(so_do_nothing) {}

        // writes the source vptr over a widget the retail constructor built
        void stamp_source_vtable() {
            *(mash::vtable_value_t*)this = mash::generate_vtable<igo_3d_hint_text_widget>();
        }

        void init() override;
        void update(f32 time_inc) override;

        virtual void unk_059(mash::string text, i32 unk, f32 duration); // owns the string
        virtual void unk_060(u32 text_id, i32 unk, f32 duration);
    };

    ASSERT_SIZEOF  (igo_3d_objective_text_widget::queue_entry, 0x10);

    ASSERT_SIZEOF  (igo_3d_objective_text_widget,                   0x180);
    ASSERT_OFFSETOF(igo_3d_objective_text_widget, queue,            0x150);
    ASSERT_OFFSETOF(igo_3d_objective_text_widget, shown_message,    0x15C);
    ASSERT_OFFSETOF(igo_3d_objective_text_widget, text,             0x160);
    ASSERT_OFFSETOF(igo_3d_objective_text_widget, suspended,        0x164);
    ASSERT_OFFSETOF(igo_3d_objective_text_widget, unk_165,          0x165);
    ASSERT_OFFSETOF(igo_3d_objective_text_widget, overlay_2_member, 0x168);
    ASSERT_OFFSETOF(igo_3d_objective_text_widget, overlay_3_member, 0x16C);
    ASSERT_OFFSETOF(igo_3d_objective_text_widget, overlay_4_member, 0x170);

    ASSERT_SIZEOF  (igo_3d_hint_text_widget, 0x180);
} // treyarch
