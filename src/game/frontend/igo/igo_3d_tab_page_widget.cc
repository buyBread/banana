#include "retail.hh"
#include "game/frontend/igo/igo_3d_tab_page_widget.hh"
#include "game/frontend/igo/igo_3d_text.hh"

using namespace treyarch;

// sub_6FAFF0
igo_3d_tab_page_widget::~igo_3d_tab_page_widget() {
    for (u32 index = 0; index < member_pairs.size(); ++index) {
        if (member_pairs[index]) {
            delete member_pairs[index];
            member_pairs[index] = nullptr;
        }
    }

    for (u32 index = 0; index < tab_texts.size(); ++index) {
        if (tab_texts[index]) {
            tab_texts[index]->destroy();
            tab_texts[index] = nullptr;
        }
    }

    member_pairs.clear();
    tab_texts.clear();
    tab_members.clear();
    tab_names.clear();
    hidden_members.clear();

    release_entity();
}

// slot 1
void igo_3d_tab_page_widget::init() {
    ((void (__thiscall*)(igo_3d_tab_page_widget*))retail::sub_68FD30)(this);
}

// slot 2
void igo_3d_tab_page_widget::update(f32 time_inc) {
    ((void (__thiscall*)(igo_3d_tab_page_widget*, f32))retail::sub_6A5070)(this, time_inc);
}

// sub_6A4F80
void igo_3d_tab_page_widget::render_congomerate_mesh(ngl::mesh*              mesh,
                                                     i32                     index,
                                                     const matrix4x4        &trans_matrix,
                                                     const matrix4x4        &rel_pos_matrix,
                                                     ngl::mesh_params*       mesh_params,
                                                     ngl::shader_param_set*  shader_params) {

    for (u32 hidden = 0; hidden < hidden_members.size(); ++hidden) {
        if (hidden_members[hidden] == index)
            return;
    }

    bool is_tab_member = false;
    i32  tab_count     = (i32)tab_members.size();

    for (i32 tab = 0; tab < tab_count; ++tab) {
        if (tab_members[tab] == index) {
            if (tab != visible_tab)
                return;

            is_tab_member = true;

            break;
        }
    }

    if (!is_tab_member) {
        // a member several pairs hold must be shown by every one of them
        for (u32 pair_index = 0; pair_index < member_pairs.size(); ++pair_index) {
            tab_page_member_pair* pair = member_pairs[pair_index];

            if (!pair)
                continue;

            if (index == pair->member_a || index == pair->member_b) {
                i32 shown = pair->shows_a ? pair->member_a : pair->member_b;

                if (shown != index)
                    return;
            }
        }
    }

    igo_3d_widget::render_congomerate_mesh(mesh,
                                           index,
                                           trans_matrix,
                                           rel_pos_matrix,
                                           mesh_params,
                                           shader_params);
}

// slot 54
void igo_3d_tab_page_widget::next_tab() {
    ((void (__thiscall*)(igo_3d_tab_page_widget*))retail::sub_68FD50)(this);
}

// slot 55
void igo_3d_tab_page_widget::previous_tab() {
    ((void (__thiscall*)(igo_3d_tab_page_widget*))retail::sub_68FD70)(this);
}

// sub_68FD90
void igo_3d_tab_page_widget::slide_in() {
    start_timed_move(0.2f, offscreen.x, offscreen.y, offscreen.z, home.x, home.y, home.z, 2);

    slide_state = 1;

    set_shown(true);
}

// sub_68FE00
void igo_3d_tab_page_widget::slide_out() {
    start_timed_move(0.2f, home.x, home.y, home.z, offscreen.x, offscreen.y, offscreen.z, 2);

    slide_state = 5;

    set_shown(true);
}

// slot 58
void igo_3d_tab_page_widget::set_current_tab(i32 index) {
    ((void (__thiscall*)(igo_3d_tab_page_widget*, i32))retail::sub_6BC4B0)(this, index);
}

// sub_68FCD0
void igo_3d_tab_page_widget::place_offscreen() {
    offscreen.x = -100.0f;
    offscreen.y = home.y;
    offscreen.z = home.z;

    set_position(offscreen.x, offscreen.y, offscreen.z);
}

// sub_6FB2E0
igo_3d_scrapbook_tab_pages::~igo_3d_scrapbook_tab_pages() {
    if (lt_button) {
        delete lt_button;
        lt_button = nullptr;
    }

    if (rt_button) {
        delete rt_button;
        rt_button = nullptr;
    }
}

// slot 1
void igo_3d_scrapbook_tab_pages::init() {
    ((void (__thiscall*)(igo_3d_scrapbook_tab_pages*))retail::sub_70D890)(this);
}

// slot 42
void igo_3d_scrapbook_tab_pages::spawn_entity() {
    ((void (__thiscall*)(igo_3d_scrapbook_tab_pages*))retail::sub_7063E0)(this);
}

// slot 56
void igo_3d_scrapbook_tab_pages::slide_in() {
    ((void (__thiscall*)(igo_3d_scrapbook_tab_pages*))retail::sub_6FB370)(this);
}

// sub_6FCA90
igo_3d_scrapbook_tab_pages_skills::~igo_3d_scrapbook_tab_pages_skills() {
    if (lb_button) {
        delete lb_button;
        lb_button = nullptr;
    }

    if (rb_button) {
        delete rb_button;
        rb_button = nullptr;
    }
}

// slot 1
void igo_3d_scrapbook_tab_pages_skills::init() {
    ((void (__thiscall*)(igo_3d_scrapbook_tab_pages_skills*))retail::sub_708660)(this);
}

// slot 2
void igo_3d_scrapbook_tab_pages_skills::update(f32 time_inc) {
    ((void (__thiscall*)(igo_3d_scrapbook_tab_pages_skills*, f32))retail::sub_6ACA10)(this, time_inc);
}

// slot 42
void igo_3d_scrapbook_tab_pages_skills::spawn_entity() {
    ((void (__thiscall*)(igo_3d_scrapbook_tab_pages_skills*))retail::sub_6FCB20)(this);
}

// sub_6934B0
void igo_3d_scrapbook_tab_pages_skills::place_offscreen() {
    offscreen.x = -200.0f;
    offscreen.y = home.y;
    offscreen.z = home.z;

    set_position(offscreen.x, offscreen.y, offscreen.z);
}
