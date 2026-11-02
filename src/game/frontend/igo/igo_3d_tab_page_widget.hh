#pragma once

#include "vc8/vector.hh"
#include "game/frontend/igo/igo_3d_slideable_widget.hh"
#include "game/math/algebra.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/virtual_base.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // two conglomerate members of which only one is drawn
    class tab_page_member_pair {

    public:
        i32  member_a;
        i32  member_b;
        bool shows_a;

        virtual ~tab_page_member_pair();

        virtual void unk_001(i32 unk) = 0;
        virtual void update(f32 time_inc) = 0;
    };

    class igo_3d_tab_page_widget : public igo_3d_slideable_widget {

    public:
        u32                                unk_150;
        u8                                 reserved_154[0x08];
        i32                                slide_state;       // 1 sliding in, 5 sliding out
        vc8::vector<i32>                   tab_members;       // a conglomerate member per visible tab, only visible_tab's is drawn
        vc8::vector<igo_3d_text*>          tab_texts;
        vc8::vector<mash::string>          tab_names;         // set_current_tab wraps on their count
        i32                                current_tab;
        i32                                visible_tab;       // current_tab - first_visible_tab
        i32                                first_visible_tab; // set_current_tab scrolls it to keep the current tab in view
        vector3d                           offscreen;
        vector3d                           home;              // the parameters' position at construction
        vc8::vector<tab_page_member_pair*> member_pairs;
        vc8::vector<i32>                   hidden_members;    // conglomerate members never drawn
        u8                                 pad_1d4[0x0C];

        explicit igo_3d_tab_page_widget(mash::generating_vtable_version so_do_nothing) : igo_3d_slideable_widget(so_do_nothing) {}

        ~igo_3d_tab_page_widget() override;

        void init() override;
        void update(f32 time_inc) override;

        void render_congomerate_mesh(ngl::mesh*              mesh,
                                     i32                     index,
                                     const matrix4x4        &trans_matrix,
                                     const matrix4x4        &rel_pos_matrix,
                                     ngl::mesh_params*       mesh_params,
                                     ngl::shader_param_set*  shader_params) override;

        virtual void next_tab();
        virtual void previous_tab();
        virtual void slide_in();                 // a timed move from offscreen to home, then shown
        virtual void slide_out();                // a timed move from home to offscreen, still shown
        virtual void set_current_tab(i32 index); // wraps around either end
        virtual void place_offscreen();          // jumps to offscreen, x -100 beside home's y and z
    };

    // the scrapbook's "ui_shell_tab_bar"
    class igo_3d_scrapbook_tab_pages : public igo_3d_tab_page_widget {

    public:
        igo_3d_widget* lt_button; // "ui_shell_tab_bar_lt_btn"
        igo_3d_widget* rt_button; // "ui_shell_tab_bar_rt_btn"
        u8             pad_1e8[0x08];

        explicit igo_3d_scrapbook_tab_pages(mash::generating_vtable_version so_do_nothing) : igo_3d_tab_page_widget(so_do_nothing) {}

        ~igo_3d_scrapbook_tab_pages() override;

        // writes the source vptr over pages the retail constructor built
        void stamp_source_vtable() {
            *(mash::vtable_value_t*)this = mash::generate_vtable<igo_3d_scrapbook_tab_pages>();
        }

        void init() override;
        void spawn_entity() override;
        void slide_in() override;
    };

    // the skills station's "ui_shell_skills_tab_bar"
    class igo_3d_scrapbook_tab_pages_skills : public igo_3d_tab_page_widget {

    public:
        igo_3d_widget* lb_button; // "ui_shell_skills_tab_bar_lb_btn"
        igo_3d_widget* rb_button; // "ui_shell_skills_tab_bar_rb_btn"
        u8             pad_1e8[0x08];

        explicit igo_3d_scrapbook_tab_pages_skills(mash::generating_vtable_version so_do_nothing) : igo_3d_tab_page_widget(so_do_nothing) {}

        ~igo_3d_scrapbook_tab_pages_skills() override;

        // writes the source vptr over pages the retail constructor built
        void stamp_source_vtable() {
            *(mash::vtable_value_t*)this = mash::generate_vtable<igo_3d_scrapbook_tab_pages_skills>();
        }

        void init() override;
        void update(f32 time_inc) override;
        void spawn_entity() override;
        void place_offscreen() override; // x -200
    };

    ASSERT_OFFSETOF(tab_page_member_pair, member_a, 0x04);
    ASSERT_OFFSETOF(tab_page_member_pair, member_b, 0x08);
    ASSERT_OFFSETOF(tab_page_member_pair, shows_a,  0x0C);

    ASSERT_SIZEOF  (igo_3d_tab_page_widget,                    0x1E0);
    ASSERT_OFFSETOF(igo_3d_tab_page_widget, slide_state,       0x15C);
    ASSERT_OFFSETOF(igo_3d_tab_page_widget, tab_members,       0x160);
    ASSERT_OFFSETOF(igo_3d_tab_page_widget, tab_texts,         0x170);
    ASSERT_OFFSETOF(igo_3d_tab_page_widget, tab_names,         0x180);
    ASSERT_OFFSETOF(igo_3d_tab_page_widget, current_tab,       0x190);
    ASSERT_OFFSETOF(igo_3d_tab_page_widget, visible_tab,       0x194);
    ASSERT_OFFSETOF(igo_3d_tab_page_widget, first_visible_tab, 0x198);
    ASSERT_OFFSETOF(igo_3d_tab_page_widget, offscreen,         0x19C);
    ASSERT_OFFSETOF(igo_3d_tab_page_widget, home,              0x1A8);
    ASSERT_OFFSETOF(igo_3d_tab_page_widget, member_pairs,      0x1B4);
    ASSERT_OFFSETOF(igo_3d_tab_page_widget, hidden_members,    0x1C4);

    ASSERT_SIZEOF  (igo_3d_scrapbook_tab_pages,            0x1F0);
    ASSERT_OFFSETOF(igo_3d_scrapbook_tab_pages, lt_button, 0x1E0);
    ASSERT_OFFSETOF(igo_3d_scrapbook_tab_pages, rt_button, 0x1E4);

    ASSERT_SIZEOF  (igo_3d_scrapbook_tab_pages_skills,            0x1F0);
    ASSERT_OFFSETOF(igo_3d_scrapbook_tab_pages_skills, lb_button, 0x1E0);
    ASSERT_OFFSETOF(igo_3d_scrapbook_tab_pages_skills, rb_button, 0x1E4);
} // treyarch
