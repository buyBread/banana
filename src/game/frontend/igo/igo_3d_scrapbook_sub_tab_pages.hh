#pragma once

#include "vc8/vector.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/math/algebra.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/virtual_base.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace references {
        // the update applies them while they're set; zero in retail data and never written
        inline util::memory_reference<bool> sub_tab_pages_move_button           { 0x0102F828 };
        inline util::memory_reference<f32>  sub_tab_pages_button_x              { 0x0102F820 };
        inline util::memory_reference<f32>  sub_tab_pages_button_y              { 0x0102F824 };
        inline util::memory_reference<bool> sub_tab_pages_move                  { 0x0102F81E };
        inline util::memory_reference<f32>  sub_tab_pages_x                     { 0x00E73A70 };
        inline util::memory_reference<f32>  sub_tab_pages_y                     { 0x00E73A74 };
        inline util::memory_reference<bool> sub_tab_pages_move_initial_position { 0x0102F81D };
        inline util::memory_reference<f32>  sub_tab_pages_initial_x             { 0x00E73A6C };
        inline util::memory_reference<f32>  sub_tab_pages_initial_y             { 0x00E73A68 };
    } // references

    // the scrapbook's "ui_shell_tab_bar_bottom"; no slots past the widget's
    class igo_3d_scrapbook_sub_tab_pages : public igo_3d_widget {

    public:
        vc8::vector<igo_3d_widget*> items;            // slot 47 lays them out side by side
        igo_3d_widget*              button;           // "ui_shell_sub_tab_bar_lb_btn"
        vector3d                    initial_position; // the parameters' position at construction
        vector3d                    button_position;  // the button's position at construction
        u8                          pad_17c[0x04];

        explicit igo_3d_scrapbook_sub_tab_pages(mash::generating_vtable_version so_do_nothing) : igo_3d_widget(so_do_nothing) {}

        ~igo_3d_scrapbook_sub_tab_pages() override;

        // writes the source vptr over pages the retail constructor built
        void stamp_source_vtable() {
            *(mash::vtable_value_t*)this = mash::generate_vtable<igo_3d_scrapbook_sub_tab_pages>();
        }

        void update(f32 time_inc) override;
        void spawn_entity() override;

        void render_child_widget(igo_3d_widget*          child,
                                 const matrix4x4        &trans_matrix,
                                 const matrix4x4        &rel_pos_matrix,
                                 ngl::mesh_params*       mesh_params,
                                 ngl::shader_param_set*  shader_params) override;

        void clear_items(); // deletes each item
    };

    ASSERT_SIZEOF  (igo_3d_scrapbook_sub_tab_pages,                   0x180);
    ASSERT_OFFSETOF(igo_3d_scrapbook_sub_tab_pages, items,            0x150);
    ASSERT_OFFSETOF(igo_3d_scrapbook_sub_tab_pages, button,           0x160);
    ASSERT_OFFSETOF(igo_3d_scrapbook_sub_tab_pages, initial_position, 0x164);
    ASSERT_OFFSETOF(igo_3d_scrapbook_sub_tab_pages, button_position,  0x170);
} // treyarch
