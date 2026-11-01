#include "retail.hh"
#include "game/wds/ai/states/plr_loco_crawl_state.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/entity/interface/player_interface.hh"
#include "game/wds/entity/interface/standing_interface.hh"
#include "game/wds/entity/actor.hh"
#include "game/als/mocomp.hh"
#include "game/line_info.hh"
#include "game/local_collision.hh"

using namespace treyarch;

// slot 1
void plr_loco_crawl_state::construct_mashed_class() {
    ((void (__thiscall*)(plr_loco_crawl_state*))retail::sub_54B710)(this);
}

// slot 2
void plr_loco_crawl_state::destruct_mashed_class() {
    ((void (__thiscall*)(plr_loco_crawl_state*))retail::sub_450E50)(this);
}

// slot 3
void plr_loco_crawl_state::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(plr_loco_crawl_state*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_54B5E0)(this,
                                                                                                                       mash_info,
                                                                                                                       containing_class_ptr,
                                                                                                                       buffer);
}

// slot 4
mash::virtual_types_key plr_loco_crawl_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const plr_loco_crawl_state*))retail::sub_4D5050)(this);
}

// slot 6
bool plr_loco_crawl_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const plr_loco_crawl_state*, mash::virtual_types_key))retail::sub_5808A0)(this, parent_class);
}

// sub_4E1E90
i32 plr_loco_crawl_state::get_mash_sizeof() const {
    return sizeof(*this);
}

// slot 9
void plr_loco_crawl_state::activate(      ai_state_machine* the_state_machine,
                                    const mashed_state*     the_mashed_state,
                                    const mashed_state*     prev_mashed_state,
                                    const param_block*      extra_params) {

    ((void (__thiscall*)(plr_loco_crawl_state*,
                         ai_state_machine*,
                         const mashed_state*,
                         const mashed_state*,
                         const param_block*))retail::sub_552880)(this,
                                                                 the_state_machine,
                                                                 the_mashed_state,
                                                                 prev_mashed_state,
                                                                 extra_params);
}

// slot 10
void plr_loco_crawl_state::deactivate(const mashed_state* next_mashed_state) {
    ((void (__thiscall*)(plr_loco_crawl_state*, const mashed_state*))retail::sub_552C60)(this, next_mashed_state);
}

// sub_552EC0
e_state_exit_code plr_loco_crawl_state::frame_advance(f32 delta_t) {
    player_interface* player        = get_actor()->player_ifc();
    e_state_exit_code the_exit_code = enhanced_state::frame_advance(delta_t);

    auto* crawl = (crawl_layer_inode*)get_core()->get_info_node(info_node_type_crawl_layer);

    unk_023(delta_t, false);

    if (the_exit_code != state_exit_code_no_exit)
        return the_exit_code;

    // the hero lies along the surface it crawls on
    {
        const vector3d &position = get_actor()->my_abs_po->get_position();
        const vector3d &normal   = player->surface_info.hit_norm;
        const vector3d &forward  = get_actor()->my_abs_po->get_z_facing();

        po pose;
        pose.set_po(math::vector4(forward.x, forward.y, forward.z, 0.0f),
                    math::vector4(normal.x, normal.y, normal.z, 0.0f),
                    math::vector4(position.x, position.y, position.z, 0.0f));

        player->unk_329(&pose);
    }

    if (player->unk_278(delta_t))
        player->unk_281(get_core());

    if (can_handle_exit_code(state_exit_code_jump) &&
        player->mode.previous != 1                 &&
        player->mode.previous != 32                &&
        player->unk_340()                          &&
        player->unk_393(0.0f)) {

        player->unk_181(1);

        const vector3d &position = get_actor()->my_abs_po->get_position();
        const vector3d &normal   = player->surface_info.hit_norm;
        const vector3d &forward  = get_actor()->my_abs_po->get_z_facing();

        po pose(math::vector4(forward.x, forward.y, forward.z, 0.0f),
                math::vector4(normal.x, normal.y, normal.z, 0.0f),
                math::vector4(position.x, position.y, position.z, 0.0f));

        player->unk_329(&pose);

        return state_exit_code_jump;
    }

    if (can_handle_exit_code(state_exit_code_jump) && get_actor()->player_ifc()->handle_jump_charge(delta_t)) {
        player->unk_181(1);

        const vector3d &position = get_actor()->my_abs_po->get_position();
        const vector3d &normal   = player->surface_info.hit_norm;
        const vector3d &forward  = get_actor()->my_abs_po->get_z_facing();

        po pose;
        pose.set_po(math::vector4(forward.x, forward.y, forward.z, 0.0f),
                    math::vector4(normal.x, normal.y, normal.z, 0.0f),
                    math::vector4(position.x, position.y, position.z, 0.0f));

        player->unk_329(&pose);

        return state_exit_code_jump;
    }

    if (can_handle_exit_code(state_exit_code_fall)                 &&
        (player->is_carrying_or_carried()                          ||
         player->unk_143()                                         ||
         (get_actor()->has_standing_ifc()                          &&
          !get_actor()->standing_ifc()->is_effectively_standing())))

        return state_exit_code_fall;

    if (can_handle_exit_code(state_exit_code_attack) && player->should_execute_combat(delta_t, 0.0f))
        return state_exit_code_attack;

    if (can_handle_exit_code(state_exit_code_stand) && unk_022())
        return state_exit_code_stand;

    if (can_handle_exit_code(state_exit_code_hit_switch) && player->unk_259(delta_t))
        return state_exit_code_hit_switch;

    if (can_handle_exit_code(state_exit_code_ranged_hit_switch) && player->unk_260(delta_t))
        return state_exit_code_ranged_hit_switch;

    if (player->should_do_strength_test() && !player->is_a_scripted_strength_test() && can_handle_exit_code(state_exit_code_strength_test))
        return state_exit_code_strength_test;

    if (can_handle_exit_code(state_exit_code_dodge) && player->should_execute_dodge(delta_t))
        return state_exit_code_dodge;

    if (can_handle_exit_code(state_exit_code_wall_evade) && player->unk_270())
        return state_exit_code_wall_evade;

    if (can_handle_exit_code(state_exit_code_block) && player->unk_265())
        return state_exit_code_block;

    // moving over an edge onto the next surface
    if (player->get_movement_speed() > 0.0001                        &&
        (retail::sub_45B390((i32)crawl, true, false, als::ikcrawl_enabled) || // crawl_layer_inode, a transition check
         retail::sub_45CB60((i32)crawl, true, false, false))         &&
        (!als::ikcrawl_enabled || crawl->floor_transition)) {

        player->surface_info = crawl->surface_info;

        player->unk_114(crawl->exterior_transition);
        player->unk_116(crawl->exterior_transition_180);
        player->unk_119(crawl->floor_transition);

        return state_exit_code_transition;
    }

    if (can_handle_exit_code(state_exit_code_swing) && player->unk_390()) {
        const vector3d &position = get_actor()->my_abs_po->get_position();
        const vector3d &normal   = player->surface_info.hit_norm;
        const vector3d &forward  = get_actor()->my_abs_po->get_z_facing();

        po pose(math::vector4(forward.x, forward.y, forward.z, 0.0f),
                math::vector4(normal.x, normal.y, normal.z, 0.0f),
                math::vector4(position.x, position.y, position.z, 0.0f));

        player->unk_329(&pose);
        player->unk_330(-1.0f, false);
        player->unk_327(1);
        player->unk_158(40);
        player->unk_136();
        player->unk_332(0.5f, false);

        return state_exit_code_jump;
    }

    if (can_handle_exit_code(state_exit_code_zip) && player->unk_391(0, 0)) {
        const vector3d &position = get_actor()->my_abs_po->get_position();
        const vector3d &normal   = player->surface_info.hit_norm;
        const vector3d &forward  = get_actor()->my_abs_po->get_z_facing();

        po pose(math::vector4(forward.x, forward.y, forward.z, 0.0f),
                math::vector4(normal.x, normal.y, normal.z, 0.0f),
                math::vector4(position.x, position.y, position.z, 0.0f));

        player->unk_329(&pose);
        player->unk_330(-1.0f, false);
        player->unk_158(37);

        return state_exit_code_zip;
    }

    if (can_handle_exit_code(state_exit_code_wall_sprint) &&
        player->unk_319(delta_t, get_actor()->player_ifc()->get_command_button(9), 0))

        return state_exit_code_wall_sprint;

    // lost the surface: falls unless something is still within reach
    const vector3d &position = get_actor()->my_abs_po->get_position();
    const vector3d &up       = get_actor()->my_abs_po->get_y_facing();

    vector3d start((f32)((f64)(f32)(up.x * 0.5) + position.x),
                   (f32)((f64)(f32)(up.y * 0.5) + position.y),
                   (f32)((f64)(f32)(up.z * 0.5) + position.z));

    vector3d end((f32)((f64)position.x - up.x),
                 (f32)((f64)position.y - up.y),
                 (f32)((f64)position.z - up.z));

    line_info probe(start, end);

    if (!retail::sub_798F50((char*)&probe, (i32)references::unk_00e7b2a0.read(), (i32)references::unk_00e7b2d0.read(), nullptr, 0)) { // line_info::check_collision
        local_collision::exclude_entfilter_t<local_collision::entfilter_and<local_collision::entfilter_entity, local_collision::entfilter_no_capsules>> filter(get_actor());

        vector3d hit_pos;
        vector3d hit_norm;

        if (!retail::sub_7F7060((f32*)&get_actor()->my_abs_po->get_position(),
                                1.5f,
                                (i32 (__stdcall***)(char))&filter,
                                (i32 (__stdcall***)(char))references::unk_00e7b2dc.read(),
                                (f32*)&hit_pos,
                                (f32*)&hit_norm,
                                0,
                                nullptr,
                                nullptr))

            the_exit_code = state_exit_code_fall;
    }

    retail::sub_798F10((u8*)&probe); // line_info::~line_info

    return the_exit_code;
}

// slot 12
void plr_loco_crawl_state::get_info_node_list(info_node_desc_list &the_list) const {
    ((void (__thiscall*)(const plr_loco_crawl_state*, info_node_desc_list*))retail::sub_549260)(this, &the_list);
}

// slot 20
void plr_loco_crawl_state::map_controls(i32 config) {
    ((void (__thiscall*)(plr_loco_crawl_state*, i32))retail::sub_552E20)(this, config);
}

// slot 21
void plr_loco_crawl_state::set_player_mode(actor* act) {
    ((void (__thiscall*)(plr_loco_crawl_state*, actor*))retail::sub_4D5060)(this, act);
}

// slot 22
bool plr_loco_crawl_state::unk_022() {
    return ((bool (__thiscall*)(plr_loco_crawl_state*))retail::sub_552E40)(this);
}

// slot 23
void plr_loco_crawl_state::unk_023(f32 delta_t, bool unk) {
    ((void (__thiscall*)(plr_loco_crawl_state*, f32, bool))retail::sub_553AC0)(this, delta_t, unk);
}
