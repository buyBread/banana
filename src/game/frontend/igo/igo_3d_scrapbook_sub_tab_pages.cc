#include "retail.hh"
#include "game/frontend/igo/igo_3d_scrapbook_sub_tab_pages.hh"

using namespace treyarch;

// sub_6FB3B0
igo_3d_scrapbook_sub_tab_pages::~igo_3d_scrapbook_sub_tab_pages() {
    clear_items();
}

// sub_68FE70
void igo_3d_scrapbook_sub_tab_pages::update(f32 time_inc) {
    if (references::sub_tab_pages_move_button.read() && button)
        button->set_center_pos(references::sub_tab_pages_button_x.read(), references::sub_tab_pages_button_y.read());

    if (references::sub_tab_pages_move.read())
        set_center_pos(references::sub_tab_pages_x.read(), references::sub_tab_pages_y.read());

    if (references::sub_tab_pages_move_initial_position.read()) {
        initial_position.x = references::sub_tab_pages_initial_x.read();
        initial_position.y = references::sub_tab_pages_initial_y.read();
    }
}

// slot 42
void igo_3d_scrapbook_sub_tab_pages::spawn_entity() {
    ((void (__thiscall*)(igo_3d_scrapbook_sub_tab_pages*))retail::sub_6ED5A0)(this);
}

// slot 47
void igo_3d_scrapbook_sub_tab_pages::render_child_widget(igo_3d_widget*          child,
                                                         const matrix4x4        &trans_matrix,
                                                         const matrix4x4        &rel_pos_matrix,
                                                         ngl::mesh_params*       mesh_params,
                                                         ngl::shader_param_set*  shader_params) {

    ((void (__thiscall*)(igo_3d_scrapbook_sub_tab_pages*,
                         igo_3d_widget*,
                         const matrix4x4&,
                         const matrix4x4&,
                         ngl::mesh_params*,
                         ngl::shader_param_set*))retail::sub_6DEEE0)(this,
                                                                     child,
                                                                     trans_matrix,
                                                                     rel_pos_matrix,
                                                                     mesh_params,
                                                                     shader_params);
}

// sub_6ED5F0
void igo_3d_scrapbook_sub_tab_pages::clear_items() {
    for (u32 index = 0; index < items.size(); ++index) {
        if (items[index]) {
            delete items[index];
            items[index] = nullptr;
        }
    }

    items.clear();
}
