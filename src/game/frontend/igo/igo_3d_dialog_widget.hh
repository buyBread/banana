#pragma once

#include <utility>

#include "vc8/vector.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/frontend/igo/igo_generic_decay_timer.hh"
#include "game/frontend/igo/igo_generic_input_listener.hh"
#include "game/frontend/igo/simple_text_nav_button.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mm/mm.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_text;
    class mission_objective_data;

    namespace references {
        inline util::memory_reference<string_hash> dialog_3d_entered   { 0x0102CAA8 };
        inline util::memory_reference<string_hash> dialog_3d_exited    { 0x0102C45C };
        inline util::memory_reference<string_hash> dialog_3d_selection { 0x0102C52C };
    } // references

    // the string slots own their by-value strings
    class igo_3d_dialog_widget : public igo_3d_widget, public igo_generic_input_listener {

    public:
        bool                             game_was_paused;     // bring_up pauses the game unless it was
        bool                             unk_15d;             // slot 71 stores it, slot 70 reads it; input needs it set
        bool                             unk_15e;             // the button shows set it
        bool                             button_1_shown;
        bool                             button_2_shown;
        u8                               pad_161[0x03];
        i32                              bring_up_mode;
        i32                              selected_button;     // 0 is button 1, 1 is button 2
        u32                              unk_16c;
        igo_3d_text*                     objectives_title;    // "objectives_title"
        igo_3d_text*                     objectives_subtitle; // "objectives_subtitle"
        vc8::vector<igo_3d_text*>        lines;
        vc8::vector<i32>                 line_points;
        vc8::vector<std::pair<f32, f32>> line_scores;         // actual and target
        vc8::vector<bool>                line_not_required;
        igo_3d_text*                     objectives_text;     // "objectives_text"
        i32                              unk_1c0;
        i32                              first_visible_line;  // shows five lines from here
        simple_text_nav_button*          button_1;
        simple_text_nav_button*          button_2;
        igo_3d_widget*                   nav_button_1;        // "~updown"
        igo_3d_widget*                   nav_button_2;        // "~confirm"
        vc8::vector<i32>                 hidden_members;      // slot 44 skips these
        igo_generic_decay_timer          unk_1e8;
        igo_generic_decay_timer          unk_204;
        i32                              result;              // 1 accepted, 0 cancelled; slot 72 answers it while no button is shown
        f32                              unk_224;
        u32                              unk_228;
        u8                               unk_22c;             // hides the actual score of the objectives added
        u8                               reserved_22d[0x03];

        explicit igo_3d_dialog_widget(mash::generating_vtable_version so_do_nothing) : igo_3d_widget(so_do_nothing),
                                                                                       igo_generic_input_listener(so_do_nothing) {}

        ~igo_3d_dialog_widget() override;

        // inlined into the deleting destructors
        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
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
                                     ngl::shader_param_set*  shader_params) override;

        void render_text(igo_3d_text*            text,
                         const matrix4x4        &trans_matrix,
                         const matrix4x4        &rel_pos_matrix,
                         ngl::mesh_params*       mesh_params,
                         ngl::shader_param_set*  shader_params) override;

        void unk_003(i32 unk) override;
        void unk_004(i32 unk) override;
        void unk_005(i32 unk) override; // down
        void unk_006(i32 unk) override; // up
        void unk_007(i32 unk) override; // accept
        void unk_010(i32 unk) override; // cancel

        virtual void set_title_by_id(u32 text_id);
        virtual void set_title(mash::string title);
        virtual void set_text_by_id(u32 text_id);
        virtual void set_text(mash::string text);
        virtual void clear();
        virtual void add_objective(mission_objective_data* objective, bool indented);
        virtual void add_mission_objective(mash::string line, i32 points, f32 actual_score, f32 target_score, bool not_required);
        virtual void add_text_line(mash::string line);
        virtual void set_button_texts(mash::string first, mash::string second);
        virtual void set_button_text(mash::string text, bool first);
        virtual void bring_up(i32 mode);
        virtual void close();
        virtual bool unk_066();
        virtual bool unk_067();
        virtual bool unk_068();
        virtual u8   is_dialog_shown();
        virtual bool unk_070();
        virtual bool unk_071(bool unk);
        virtual i32  get_selected_button(); // 1-based while a button is shown
        virtual i32  unk_073();
        virtual i32  unk_074();
        virtual void set_selected_button(i32 button); // 1-based, clamped to the lines
        virtual bool select_button(i32 button);       // 1-based, true when the selection moved to a shown button
        virtual void unk_077(i32 unk);
        virtual void unk_078();

        void set_button_1_text(u32 text_id);
        void set_button_2_text(u32 text_id);

        void show_button_1(bool shown);
        void show_button_2(bool shown);

        void set_lines_shown(bool shown);
        void show_visible_lines(); // the five lines from first_visible_line, hides the rest

        bool set_first_visible_line(i32 index); // clamped to the lines, true when it changed

        bool takes_input();

        void spawn_reward_orbs(i32 experience); // a ring around the hero, 20 experience per orb and the remainder on the first

        void fill_mission_results(); // adds every objective line, then rescales the lines' points to the quest's totals

        void set_title(const char* title) {
            set_title(mash::string(title));
        }

        void set_text(const char* text) {
            set_text(mash::string(text));
        }

        void add_text_line(const char* line) { // add_mission_objective with no points
            add_text_line(mash::string(line));
        }
    };

    ASSERT_SIZEOF  (igo_3d_dialog_widget,                      0x230);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, game_was_paused,     0x15C);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, unk_15d,             0x15D);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, unk_15e,             0x15E);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, button_1_shown,      0x15F);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, button_2_shown,      0x160);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, bring_up_mode,       0x164);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, selected_button,     0x168);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, objectives_title,    0x170);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, objectives_subtitle, 0x174);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, lines,               0x178);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, line_points,         0x188);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, line_scores,         0x198);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, line_not_required,   0x1A8);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, objectives_text,     0x1BC);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, first_visible_line,  0x1C4);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, button_1,            0x1C8);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, button_2,            0x1CC);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, nav_button_1,        0x1D0);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, nav_button_2,        0x1D4);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, hidden_members,      0x1D8);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, unk_1e8,             0x1E8);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, unk_204,             0x204);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, result,              0x220);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, unk_22c,             0x22C);
} // treyarch
