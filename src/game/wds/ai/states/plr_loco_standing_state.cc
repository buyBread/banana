#include "retail.hh"
#include "game/wds/ai/states/plr_loco_standing_state.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/entity/interface/physical_interface.hh"
#include "game/wds/entity/interface/player_interface.hh"
#include "game/wds/entity/interface/standing_interface.hh"
#include "game/wds/entity/actor.hh"
#include "game/als/param.hh"
#include "game/line_info.hh"
#include "game/math/references.hh"

using namespace treyarch;

// slot 1
void plr_loco_standing_state::construct_mashed_class() {
    ((void (__thiscall*)(plr_loco_standing_state*))retail::sub_548F10)(this);
}

// slot 2
void plr_loco_standing_state::destruct_mashed_class() {
    ((void (__thiscall*)(plr_loco_standing_state*))retail::sub_548F50)(this);
}

// slot 3
void plr_loco_standing_state::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(plr_loco_standing_state*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_57B590)(this,
                                                                                                                          mash_info,
                                                                                                                          containing_class_ptr,
                                                                                                                          buffer);
}

// slot 4
mash::virtual_types_key plr_loco_standing_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const plr_loco_standing_state*))retail::sub_4D69E0)(this);
}

// slot 6
bool plr_loco_standing_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const plr_loco_standing_state*, mash::virtual_types_key))retail::sub_4D69F0)(this, parent_class);
}

// sub_4D3A70
i32 plr_loco_standing_state::get_mash_sizeof() const {
    return sizeof(*this);
}

// slot 9
void plr_loco_standing_state::activate(      ai_state_machine* the_state_machine,
                                       const mashed_state*     the_mashed_state,
                                       const mashed_state*     prev_mashed_state,
                                       const param_block*      extra_params) {

    ((void (__thiscall*)(plr_loco_standing_state*,
                         ai_state_machine*,
                         const mashed_state*,
                         const mashed_state*,
                         const param_block*))retail::sub_54E960)(this,
                                                                 the_state_machine,
                                                                 the_mashed_state,
                                                                 prev_mashed_state,
                                                                 extra_params);
}

// slot 10
void plr_loco_standing_state::deactivate(const mashed_state* next_mashed_state) {
    ((void (__thiscall*)(plr_loco_standing_state*, const mashed_state*))retail::sub_54EDF0)(this, next_mashed_state);
}

// sub_5745C0
e_state_exit_code plr_loco_standing_state::frame_advance(f32 delta_t) {
    e_state_exit_code the_exit_code = state_exit_code_no_exit;
    player_interface* player        = get_actor()->player_ifc();

    // the first frame takes the hero into the mode it was asked for
    if (initial_startup) {
        i32 desired_mode = player->desired_mode;

        if (!desired_mode)
            desired_mode = 1;

        switch (desired_mode) {
            case 1:
                if (!player->override_mode)
                    player->clear_controller(true);

                if (get_actor()->has_standing_ifc())
                    get_actor()->standing_ifc()->set_swimming(false);

                if (get_actor()->get_ai_core()) {
                    retail::sub_4C65F0((u32*)get_actor()->get_ai_core());

                    if (get_actor()->get_ai_core()->find_machine(references::unk_00fc6bc0.get()))
                        get_actor()->get_ai_core()->pop_base_machine(references::unk_00fc6bc0.get(), true);
                }

                break;

            // crawling and wall sprinting start on the surface below
            case 2:
            case 7: {
                if (desired_mode == 2 && !can_crawl)
                    break;

                if (desired_mode == 7 && !can_handle_exit_code(state_exit_code_wall_sprint))
                    break;

                line_info surface;
                vector3d  up(0.0f, 1.0f, 0.0f);

                if (retail::sub_42EFF0((f32*)&get_actor()->my_abs_po->get_position(), 5.0f, (f32*)&surface, (i32*)&up, true, false) && // get_best_surface_pos
                    (!(surface.hit_norm.y > 0.7f) || player->mode_locked)) {

                    if (desired_mode == 2) {
                        auto* als_node = (als_inode*)get_actor()->get_ai_core()->get_info_node(info_node_type_als);

                        retail::sub_4029A0((u32**)als_node, als::param_crawl_type,            0.0f, 0); // als_inode::set_desired_param
                        retail::sub_4029A0((u32**)als_node, als::param_crawl_transition_type, 4.0f, 0);

                        retail::sub_4029D0((u32**)als_node, als::param_attach_normal, (i32)&surface.hit_norm, 0); // als_inode::set_desired_param
                        retail::sub_4029D0((u32**)als_node, als::param_attach_point,  (i32)&surface.hit_pos,  0);

                        retail::sub_4029A0((u32**)als_node, als::param_desired_distance, (f32)(get_actor()->get_render_scale().z * 0.5), 0);

                        player->surface_info = surface;

                        player->unk_114(0);
                        player->unk_116(0);
                        player->unk_119(0);

                        the_exit_code = state_exit_code_crawl;
                    } else {
                        player->unk_335(-1.0f, false);
                        player->unk_114(0);
                        player->unk_116(0);
                        player->unk_321(&surface);
                        player->unk_119(0);

                        the_exit_code = state_exit_code_wall_sprint;
                    }
                }

                retail::sub_798F10((u8*)&surface); // line_info::~line_info

                break;
            }

            case 3:
                if (can_handle_exit_code(state_exit_code_swing) && player->unk_390())
                    the_exit_code = state_exit_code_swing;

                break;

            case 4:
                if (can_handle_exit_code(state_exit_code_fall))
                    the_exit_code = state_exit_code_fall;

                break;

            case 5:
                if (can_handle_exit_code(state_exit_code_jump))
                    the_exit_code = state_exit_code_jump;

                break;

            case 11:
                if (can_handle_exit_code(state_exit_code_yoyo) && player->unk_384())
                    the_exit_code = state_exit_code_yoyo;

                break;
        }

        // any other mode settles the hero on the ground first
        if (the_exit_code == state_exit_code_no_exit && desired_mode != 1) {
            if (get_actor()->has_ifc(entity_ifc_standing) && get_actor()->standing_ifc()->is_enabled()) {
                if (!get_actor()->has_physical_ifc() || !get_actor()->physical_ifc()->is_flagged(physical_interface_flag_biped_running)) {
                    u32 was_set = get_actor()->standing_ifc()->flags & standing_interface_flag_unk_00004000;

                    get_actor()->standing_ifc()->flags |= standing_interface_flag_unk_00004000;
                    get_actor()->standing_ifc()->manage_standing(true, 0.25f);

                    retail::sub_5488F0((u32*)get_actor()->standing_ifc(), was_set != 0);
                }
            }

            if (can_handle_exit_code(state_exit_code_fall)) {
                if ((get_actor()->has_ifc(entity_ifc_standing) && !get_actor()->standing_ifc()->is_effectively_standing()) ||
                    (get_actor()->has_physical_ifc()                                                        &&
                     get_actor()->physical_ifc()->is_flagged(physical_interface_flag_biped_running)       &&
                     !retail::sub_600630((i32)get_actor()->physical_ifc(), false)))

                    the_exit_code = state_exit_code_fall;
            }
        }

        initial_startup = false;

        if (the_exit_code == state_exit_code_no_exit) {
            player->mode = 0;
            player->mode = 1;
        }
    }

    player->set_desired_mode(0);

    if (the_exit_code == state_exit_code_no_exit) {
        the_exit_code = attack_enabled_state::frame_advance(delta_t);

        if (the_exit_code == state_exit_code_no_exit) {
            unk_031(delta_t);
            unk_021(delta_t);

            retail::sub_550C60((u32*)this, delta_t);

            if (!did_anim_request) {
                auto* als_node = (als_inode*)get_core()->get_info_node(info_node_type_als);

                did_anim_request = true;

                retail::sub_42EA60((u32*)get_actor(), (i32)&references::unk_00fc6a60.get(), (i32)&references::unk_00e66e1c.get(), false);

                if (unk_1b0)
                    retail::sub_4029A0((u32**)als_node, als::param_fall_speed, -0.0f - unk_164.y, 0); // als_inode::set_desired_param

                if (get_actor()->has_ifc(entity_ifc_standing)) {
                    u32 was_set     = get_actor()->standing_ifc()->flags & standing_interface_flag_unk_00004000;
                    u32 was_enabled = get_actor()->standing_ifc()->flags & standing_interface_flag_enabled;

                    get_actor()->standing_ifc()->flags |= standing_interface_flag_unk_00004000;
                    get_actor()->standing_ifc()->flags |= standing_interface_flag_enabled;
                    get_actor()->standing_ifc()->manage_standing(0.01f, true, 1.0f);

                    if (was_set)
                        get_actor()->standing_ifc()->flags |= standing_interface_flag_unk_00004000;
                    else
                        get_actor()->standing_ifc()->flags &= ~standing_interface_flag_unk_00004000;

                    if (was_enabled)
                        get_actor()->standing_ifc()->flags |= standing_interface_flag_enabled;
                    else
                        get_actor()->standing_ifc()->flags &= ~standing_interface_flag_enabled;
                }
            }

            the_exit_code = unk_030(delta_t);
        } else if (the_exit_code == state_exit_code_fall) {
            // falling off onto a wall or into a crawl instead
            if (player->unk_319(delta_t, wall_sprint_button, 0)) {
                auto* sprint = (wall_sprint_inode*)get_core()->get_info_node(info_node_type_wall_sprint);

                the_exit_code = sprint && sprint->unk_05c == 11 ? state_exit_code_transition : state_exit_code_wall_sprint;
            } else if (can_crawl && player->unk_264(delta_t, nullptr, 0, 0, 0))
                the_exit_code = state_exit_code_crawl;
        }

        if (the_exit_code == state_exit_code_no_exit) {
            unk_028(delta_t);

            auto* als_node = (als_inode*)get_core()->get_info_node(info_node_type_als);

            if (als_node->my_als->get_als_layer(als::base_layer)->did_transition_succeed()) {
                i32 none         = 0;
                i32 delay_frames = als_node->get_optional_als_pb_int(delay_standing_frames_key, none, als::base_layer, nullptr);

                if (delay_frames && get_actor()->has_ifc(entity_ifc_standing)) {
                    get_actor()->standing_ifc()->flags |= standing_interface_flag_enabled;

                    retail::sub_5FF3B0((i32)get_actor()->standing_ifc(), (f32)((f32)delay_frames / 30.0));
                }
            }

            bool grinds = false;

            if (can_handle_exit_code(state_exit_code_transition)          &&
                unk_1b4 == 2                                              &&
                !player->is_carrying_or_carried()                         &&
                player->get_command_button(plr_cmd_crawl_attach)->triggered_or_missed_press()) {

                grinds = true;

                // only with nothing in the way overhead
                if (unk_1bc == 1) {
                    const vector3d &up       = references::unk_00e66e1c.get();
                    const vector3d &position = get_actor()->my_abs_po->get_position();

                    vector3d above((f32)((f64)(f32)(up.x * 10.0) + position.x),
                                   (f32)((f64)(f32)(up.y * 10.0) + position.y),
                                   (f32)((f64)(f32)(up.z * 10.0) + position.z));

                    line_info probe(get_actor()->my_abs_po->get_position(), above);

                    if (retail::sub_798F50((char*)&probe, (i32)references::unk_00e7b2a0.read(), (i32)references::unk_00e7b2d0.read(), nullptr, 0)) // line_info::check_collision
                        grinds = false;

                    retail::sub_798F10((u8*)&probe); // line_info::~line_info
                }
            }

            if (grinds)
                the_exit_code = state_exit_code_wall_grind;
            else {
                if (!player->is_carrying_or_carried() && unk_1bc == 1) {
                    if (unk_1b4 == 5 && player->unk_530(delta_t)) {
                        if (can_handle_exit_code(state_exit_code_sync_obstacle_avoid) && player->unk_532(nullptr))
                            the_exit_code = state_exit_code_sync_obstacle_avoid;
                    } else if (can_handle_exit_code(state_exit_code_obstacle_hop) &&
                               unk_1b4 == 4                                       &&
                               !player->unk_528()->hit_entity.resolve())

                        the_exit_code = state_exit_code_obstacle_hop;
                }

                // the crawl entry waits 0.35 first; these two ways out keep the wait
                if (the_exit_code == state_exit_code_no_exit &&
                    unk_1bc == 1                             &&
                    unk_1b4 == 2                             &&
                    can_handle_exit_code(state_exit_code_crawl)) {

                    unk_1cc = (f32)((f64)unk_1cc + delta_t);

                    if (unk_1cc > 0.35f && player->unk_264(delta_t, nullptr, 1, 0, 0)) {
                        player->unk_193(0);

                        return state_exit_code_crawl;
                    }

                    return the_exit_code;
                }
            }
        }
    }

    unk_1cc = 0.0f;

    if (the_exit_code == state_exit_code_crawl)
        player->unk_193(0);

    return the_exit_code;
}

// slot 12
void plr_loco_standing_state::get_info_node_list(info_node_desc_list &the_list) const {
    ((void (__thiscall*)(const plr_loco_standing_state*, info_node_desc_list*))retail::sub_5491D0)(this, &the_list);
}

// slot 21
i32 plr_loco_standing_state::unk_021(f32 unk) {
    return ((i32 (__thiscall*)(plr_loco_standing_state*, f32))retail::sub_550A60)(this, unk);
}

// slot 24
als::category_id plr_loco_standing_state::get_als_category() const {
    als::category_id category;

    ((als::category_id* (__thiscall*)(const plr_loco_standing_state*, als::category_id*))retail::sub_549190)(this, &category);

    return category;
}

// slot 25
bool plr_loco_standing_state::unk_025() {
    return ((bool (__thiscall*)(plr_loco_standing_state*))retail::sub_54F030)(this);
}

// slot 26
void plr_loco_standing_state::map_controls(i32 config) {
    ((void (__thiscall*)(plr_loco_standing_state*, i32))retail::sub_54EEE0)(this, config);
}

// slot 27
void plr_loco_standing_state::set_player_mode(actor* act) {
    ((void (__thiscall*)(plr_loco_standing_state*, actor*))retail::sub_4D6A30)(this, act);
}

// slot 28
void plr_loco_standing_state::unk_028(f32 delta_t) {
    ((void (__thiscall*)(plr_loco_standing_state*, f32))retail::sub_54F0E0)(this, delta_t);
}

// slot 29
void plr_loco_standing_state::unk_029(f32 delta_t) {
    ((void (__thiscall*)(plr_loco_standing_state*, f32))retail::sub_54F220)(this, delta_t);
}

// slot 30
e_state_exit_code plr_loco_standing_state::unk_030(f32 delta_t) {
    return ((e_state_exit_code (__thiscall*)(plr_loco_standing_state*, f32))retail::sub_575240)(this, delta_t);
}

// slot 31
void plr_loco_standing_state::unk_031(f32 delta_t) {
    ((void (__thiscall*)(plr_loco_standing_state*, f32))retail::sub_54F730)(this, delta_t);
}

// slot 32
void plr_loco_standing_state::unk_032(i32 unk, void* unk_2) {
    ((void (__thiscall*)(plr_loco_standing_state*, i32, void*))retail::sub_550330)(this, unk, unk_2);
}

// slot 33
void plr_loco_standing_state::unk_033(f32 unk, f32 unk_2, f32 unk_3) {
    ((void (__thiscall*)(plr_loco_standing_state*, f32, f32, f32))retail::sub_5507D0)(this, unk, unk_2, unk_3);
}
