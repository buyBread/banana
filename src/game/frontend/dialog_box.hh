#pragma once

#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/frontend/igo/igo_generic_input_listener.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/virtual_base.hh"
#include "treyarch/mm/mm.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace references {
        // the widget update moves the buttons there while it's set; zero in retail data and never written
        inline util::memory_reference<bool> dialog_box_move_buttons     { 0x0102F8D4 };
        inline util::memory_reference<f32>  dialog_box_updown_button_x  { 0x0102F8CC };
        inline util::memory_reference<f32>  dialog_box_updown_button_y  { 0x0102F8D0 };
        inline util::memory_reference<f32>  dialog_box_confirm_button_x { 0x0102F8C4 };
        inline util::memory_reference<f32>  dialog_box_confirm_button_y { 0x0102F8C8 };
    } // references

    // the ui_hud_rvb_choice two-choice dialog
    class dialog_box : public igo_generic_input_listener, public igo_3d_widget {

    public:
        bool           shown;
        bool           confirmed;
        u8             choice;             // 0 or 1
        bool           freeze;             // show stores its argument here
        i32            input_delay;        // frames before input is taken again
        i32            unfreeze_delay;     // frames until the update pays back hero_freezes; a hidden box is deleted only at 0
        u8             reserved_16c[0x08];
        i32            hero_freezes;       // show's freeze_hero calls
        f32            choice_change_time;
        bool           choice_changing;    // set by a choice change, cleared a quarter second later
        u8             pad_17d[0x03];
        u8             reserved_180[0x30];
        f32            shown_time;
        igo_3d_widget* updown_button;      // "~updown"
        igo_3d_widget* confirm_button;     // "~confirm"
        u8             reserved_1bc[0x04];

        explicit dialog_box(mash::generating_vtable_version so_do_nothing) : igo_generic_input_listener(so_do_nothing),
                                                                             igo_3d_widget(so_do_nothing) {}

        ~dialog_box() override;

        // writes both source vptrs over a box the retail constructor built
        void stamp_source_vtables() {
            *(mash::vtable_value_t*)this                 = mash::generate_vtable<dialog_box>();
            *(mash::vtable_value_t*)(igo_3d_widget*)this = mash::generate_vtable<dialog_box, igo_3d_widget>();
        }

        // inlined into the deleting destructors
        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
        }

        void unk_002(i32 unk) override;
        void unk_003(i32 unk) override;
        void unk_004(i32 unk) override;
        void unk_005(i32 unk) override;
        void unk_006(i32 unk) override;
        void unk_007(i32 unk) override;

        void update(f32 time_inc) override;
        void render() override;
        void spawn_entity() override;

        void render_congomerate_mesh(ngl::mesh*              mesh,
                                     i32                     index,
                                     const matrix4x4        &trans_matrix,
                                     const matrix4x4        &rel_pos_matrix,
                                     ngl::mesh_params*       mesh_params,
                                     ngl::shader_param_set*  shader_params) override;

        virtual void show(bool freeze_hero);
        virtual void hide();
    };

    ASSERT_SIZEOF  (dialog_box,                     0x1C0);
    ASSERT_OFFSETOF(dialog_box, shown,              0x160);
    ASSERT_OFFSETOF(dialog_box, confirmed,          0x161);
    ASSERT_OFFSETOF(dialog_box, choice,             0x162);
    ASSERT_OFFSETOF(dialog_box, input_delay,        0x164);
    ASSERT_OFFSETOF(dialog_box, unfreeze_delay,     0x168);
    ASSERT_OFFSETOF(dialog_box, hero_freezes,       0x174);
    ASSERT_OFFSETOF(dialog_box, choice_change_time, 0x178);
    ASSERT_OFFSETOF(dialog_box, choice_changing,    0x17C);
    ASSERT_OFFSETOF(dialog_box, shown_time,         0x1B0);
    ASSERT_OFFSETOF(dialog_box, updown_button,      0x1B4);
    ASSERT_OFFSETOF(dialog_box, confirm_button,     0x1B8);
} // treyarch
