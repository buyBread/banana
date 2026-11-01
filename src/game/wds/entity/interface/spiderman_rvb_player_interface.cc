#include <cfloat>
#include <cmath>

#include "retail.hh"
#include "game/wds/entity/interface/spiderman_rvb_player_interface.hh"
#include "game/als/als.hh"
#include "game/als/param.hh"
#include "game/colgeom.hh"
#include "game/game.hh"
#include "game/game_meter_manager.hh"
#include "game/script/script_access.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/ai/states/plr_loco_swing_params.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/conglomerate.hh"
#include "game/wds/entity/interface/damage_interface.hh"
#include "game/wds/entity/interface/gun_weapon_interface.hh"
#include "game/wds/entity/interface/melee_weapon_interface.hh"
#include "game/wds/entity/interface/physical_interface.hh"
#include "game/wds/entity/interface/spiderman_rvb_attacks.hh"
#include "game/wds/entity/interface/standing_interface.hh"
#include "game/wds/entity/interface/thrown_weapon_interface.hh"
#include "game/wds/references.hh"
#include "game/math/references.hh"
#include "game/wds/world_dynamics_system.hh"
#include "treyarch/math/trig.hh"
#include "treyarch/ngl/mesh/mesh.hh"

using namespace treyarch;

string_hash spiderman_rvb_player_interface::my_type_key { 0xFA62ACC8 }; // "spiderman_rvb_player_interface"
bool        spiderman_rvb_player_interface::mario_jump_pressed = true;

// slot 0
spiderman_rvb_player_interface::~spiderman_rvb_player_interface() {
    retail::sub_7219C0((u32*)this);

    // retail's destructor tore down player_interface's members too; empty the button arrays so their source destructors don't free them again
    buttons.array         = nullptr;
    command_buttons.array = nullptr;

    // and rebuild the preset strings empty, so their source destructors don't count them out a second time
    ::new (&preset_dodge_anim) mash::string();
    ::new (&preset_block_anim) mash::string();
}

// slot 1
void spiderman_rvb_player_interface::construct_mashed_class() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9C2470)(this);
}

// slot 2
void spiderman_rvb_player_interface::destruct_mashed_class() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9C2AA0)(this);
}

// slot 3
void spiderman_rvb_player_interface::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_9C25C0)(this,
                                                                                                                                 mash_info,
                                                                                                                                 containing_class_ptr,
                                                                                                                                 buffer);
}

// slot 4
mash::virtual_types_key spiderman_rvb_player_interface::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const spiderman_rvb_player_interface*))retail::sub_721920)(this);
}

// slot 6
bool spiderman_rvb_player_interface::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const spiderman_rvb_player_interface*, mash::virtual_types_key))retail::sub_721930)(this, parent_class);
}

// slot 8
i32 spiderman_rvb_player_interface::get_mash_sizeof() const {
    return ((i32 (__thiscall*)(const spiderman_rvb_player_interface*))retail::sub_721970)(this);
}

// slot 9
bool spiderman_rvb_player_interface::get_ifc_num(const string_hash &name, f32* value, bool document) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, const string_hash&, f32*, bool))retail::sub_9B5730)(this, name, value, document);
}

// slot 10
bool spiderman_rvb_player_interface::set_ifc_num(const string_hash &name, f32 value, bool document) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, const string_hash&, f32, bool))retail::sub_9B6060)(this, name, value, document);
}

// slot 11
bool spiderman_rvb_player_interface::get_ifc_vec(const string_hash &name, vector3d* value, bool document) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, const string_hash&, vector3d*, bool))retail::sub_9A67A0)(this, name, value, document);
}

// slot 18
void spiderman_rvb_player_interface::set_owner(actor* owner) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, actor*))retail::sub_9A4280)(this, owner);
}

// sub_9A4140
f32 spiderman_rvb_player_interface::unk_021() {
    i32 id = retail::sub_9210B0((u32*)references::upgrade_manager.read(), (u32*)&references::web_upgrade_power_3.get()); // get_upgrade_id

    if (id >= 0 && retail::sub_9219C0((u32*)references::upgrade_manager.read(), id))
        return 2.0f;

    id = retail::sub_9210B0((u32*)references::upgrade_manager.read(), (u32*)&references::web_upgrade_power_2.get());

    if (id >= 0 && retail::sub_9219C0((u32*)references::upgrade_manager.read(), id))
        return 1.6f;

    id = retail::sub_9210B0((u32*)references::upgrade_manager.read(), (u32*)&references::web_upgrade_power_1.get());

    if (id >= 0 && retail::sub_9219C0((u32*)references::upgrade_manager.read(), id))
        return 1.3f;

    return 1.0f;
}

// sub_99CE50
void spiderman_rvb_player_interface::set_rpg_progress(f32 progress) {
    if (0.0f > progress)
        progress = 0.0f;
    else if (progress > 1.0f)
        progress = 1.0f;

    unk_023((i32)((f64)progress * 5.0));
}

// slot 23
bool spiderman_rvb_player_interface::unk_023(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_99CEC0)(this, unk);
}

// sub_9A7610
void spiderman_rvb_player_interface::unk_024(actor* owner) {
    master_object = new script_object_instance();

    arch_base_vhandle owner_handle = owner->my_handle;

    script_arg owner_arg;
    owner_arg.type  = 3;
    owner_arg.value = &owner_handle;

    master_object->init(mash::string("spiderman_rvb"),
                        mash::string("spiderman_rvb_master_object(entity)"),
                        owner->pack_slot,
                        owner,
                        nullptr,
                        true,
                        owner_arg,
                        script_arg(),
                        script_arg(),
                        script_arg(),
                        script_arg(),
                        script_arg(),
                        script_arg(),
                        script_arg());

    master_object->register_function(mash::string("frame_advance(num)"), 0);
}

// slot 25
void* spiderman_rvb_player_interface::unk_025(f32 delta_t) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A9550)(this, delta_t);
}

// slot 26
bool spiderman_rvb_player_interface::unk_026() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7216A0)(this);
}

// slot 27
bool spiderman_rvb_player_interface::unk_027(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_7216B0)(this, unk);
}

// slot 28
bool spiderman_rvb_player_interface::unk_028(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_7216C0)(this, unk);
}

// slot 29
bool spiderman_rvb_player_interface::unk_029() const {
    return ((bool (__thiscall*)(const spiderman_rvb_player_interface*))retail::sub_7216D0)(this);
}

// slot 30
f32 spiderman_rvb_player_interface::unk_030() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7216E0)(this);
}

// slot 31
i32 spiderman_rvb_player_interface::unk_031(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_7216F0)(this, unk);
}

// slot 32
i32 spiderman_rvb_player_interface::unk_032(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721710)(this, unk);
}

// slot 34
i32 spiderman_rvb_player_interface::unk_034(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A73C0)(this, unk);
}

// slot 35
i32 spiderman_rvb_player_interface::unk_035(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6FD0)(this, unk);
}

// slot 36
void* spiderman_rvb_player_interface::unk_036(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_9A6FE0)(this, unk);
}

// slot 37
void spiderman_rvb_player_interface::set_zip_override_anchor_num(f32 num) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A6FF0)(this, num);
}

// slot 38
f32 spiderman_rvb_player_interface::unk_038() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A7010)(this);
}

// slot 39
i32 spiderman_rvb_player_interface::unk_039(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A7020)(this, unk);
}

// sub_9A7030
arch_base_vhandle spiderman_rvb_player_interface::unk_040() const {
    return unk_0724;
}

// slot 41
arch_base_vhandle spiderman_rvb_player_interface::unk_041(i32 index) {
    arch_base_vhandle result;
    ((arch_base_vhandle* (__thiscall*)(spiderman_rvb_player_interface*, arch_base_vhandle*, i32))retail::sub_9B7A20)(this, &result, index);

    return result;
}

// slot 42
i32 spiderman_rvb_player_interface::unk_042() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721220)(this);
}

// slot 43
void spiderman_rvb_player_interface::unk_043(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A70D0)(this, unk);
}

// slot 44
bool spiderman_rvb_player_interface::unk_044(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721200)(this, unk);
}

// slot 45
bool spiderman_rvb_player_interface::unk_045() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7211F0)(this);
}

// sub_9B77A0
void spiderman_rvb_player_interface::set_zlock_entity(arch_base_vhandle entity) {
    if (unk_071c == entity)
        return;

    unk_553(unk_071c, entity, unk_055());

    unk_0704 = 0.0f;
    unk_071c = entity;
}

// slot 47
void spiderman_rvb_player_interface::unk_047(u32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, u32))retail::sub_9B77F0)(this, unk);
}

// slot 48
arch_base_vhandle spiderman_rvb_player_interface::unk_048() const {
    arch_base_vhandle result;
    ((arch_base_vhandle* (__thiscall*)(const spiderman_rvb_player_interface*, arch_base_vhandle*))retail::sub_9A7050)(this, &result);

    return result;
}

// slot 49
bool spiderman_rvb_player_interface::unk_049(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9B7B50)(this, unk);
}

// slot 50
i32 spiderman_rvb_player_interface::unk_050(arch_base_vhandle target) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, arch_base_vhandle))retail::sub_9A70A0)(this, target);
}

// slot 51
i32 spiderman_rvb_player_interface::unk_051(i32 unk, i32 unk_2, i32 unk_3) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32, i32, i32))retail::sub_9B7B10)(this, unk, unk_2, unk_3);
}

// slot 52
void* spiderman_rvb_player_interface::unk_052(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_9A7060)(this, unk);
}

// slot 53
void* spiderman_rvb_player_interface::unk_053(void* unk, void* unk_2) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*, void*))retail::sub_9A7070)(this, unk, unk_2);
}

// sub_9B7950
void spiderman_rvb_player_interface::unk_054(arch_base_vhandle target, u32 kind) {
    if (unk_06f0 == target)
        return;

    unk_553(unk_06f0, target, unk_048());

    unk_06f0 = target;
    unk_06e8 = kind;

    if (target.value && kind < 4)
        unk_06ec = timed_target_times[kind];
    else
        unk_06ec = 0.0f;
}

// slot 55
arch_base_vhandle spiderman_rvb_player_interface::unk_055() {
    arch_base_vhandle result;
    ((arch_base_vhandle* (__thiscall*)(spiderman_rvb_player_interface*, arch_base_vhandle*))retail::sub_721240)(this, &result);

    return result;
}

// slot 56
f32 spiderman_rvb_player_interface::unk_056() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721250)(this);
}

// slot 57
i32 spiderman_rvb_player_interface::unk_057() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721260)(this);
}

// slot 60
i32 spiderman_rvb_player_interface::unk_060(arch_base_vhandle target) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, arch_base_vhandle))retail::sub_721280)(this, target);
}

// slot 61
arch_base_vhandle spiderman_rvb_player_interface::unk_061() {
    arch_base_vhandle result;
    ((arch_base_vhandle* (__thiscall*)(spiderman_rvb_player_interface*, arch_base_vhandle*))retail::sub_721270)(this, &result);

    return result;
}

// slot 62
void* spiderman_rvb_player_interface::unk_062(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_721290)(this, unk);
}

// slot 63
i32 spiderman_rvb_player_interface::unk_063(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_7212C0)(this, unk);
}

// slot 64
i32 spiderman_rvb_player_interface::unk_064(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_7212E0)(this, unk);
}

// slot 65
void* spiderman_rvb_player_interface::unk_065(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_721330)(this, unk);
}

// slot 66
void spiderman_rvb_player_interface::unk_066(i32 unk, i32 unk_2, i32 unk_3) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32, i32, i32))retail::sub_7212F0)(this, unk, unk_2, unk_3);
}

// slot 67
i32 spiderman_rvb_player_interface::unk_067(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721350)(this, unk);
}

// slot 68
bool spiderman_rvb_player_interface::unk_068(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721370)(this, unk);
}

// slot 69
bool spiderman_rvb_player_interface::unk_069() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721380)(this);
}

// slot 70
i32 spiderman_rvb_player_interface::unk_070(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721320)(this, unk);
}

// slot 71
void* spiderman_rvb_player_interface::unk_071(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_721340)(this, unk);
}

// slot 72
bool spiderman_rvb_player_interface::unk_072() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_667F80)(this);
}

// slot 74
bool spiderman_rvb_player_interface::unk_074() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_667F80)(this);
}

// slot 76
i32 spiderman_rvb_player_interface::unk_076() const {
    return ((i32 (__thiscall*)(const spiderman_rvb_player_interface*))retail::sub_A944D0)(this);
}

// slot 77
i32 spiderman_rvb_player_interface::unk_077() const {
    return ((i32 (__thiscall*)(const spiderman_rvb_player_interface*))retail::sub_ABD4A0)(this);
}

// slot 78
void spiderman_rvb_player_interface::set_swing_override_anchor(arch_base_vhandle anchor) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, arch_base_vhandle))retail::sub_9A59D0)(this, anchor);
}

// slot 79
void spiderman_rvb_player_interface::set_zip_override_anchor(arch_base_vhandle anchor) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, arch_base_vhandle))retail::sub_9A59E0)(this, anchor);
}

// slot 80
arch_base_vhandle spiderman_rvb_player_interface::unk_080() {
    arch_base_vhandle result;
    ((arch_base_vhandle* (__thiscall*)(spiderman_rvb_player_interface*, arch_base_vhandle*))retail::sub_9A59F0)(this, &result);

    return result;
}

// slot 81
void spiderman_rvb_player_interface::unk_081(f32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A9100)(this, unk);
}

// slot 82
i32 spiderman_rvb_player_interface::unk_082(f32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A88B0)(this, unk);
}

// slot 84
f32 spiderman_rvb_player_interface::get_movement_speed() const {
    return ((f32 (__thiscall*)(const spiderman_rvb_player_interface*))retail::sub_9A4EB0)(this);
}

// slot 86
void spiderman_rvb_player_interface::unk_086(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A5210)(this, unk);
}

// slot 87
bool spiderman_rvb_player_interface::unk_087() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720E10)(this);
}

// slot 88
bool spiderman_rvb_player_interface::unk_088(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_720E20)(this, unk);
}

// slot 89
bool spiderman_rvb_player_interface::map_controls(i32 config, bool force) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32, bool))retail::sub_9A47B0)(this, config, force);
}

// slot 90
void spiderman_rvb_player_interface::render_phase() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A47A0)(this);
}

// slot 91
void spiderman_rvb_player_interface::update_interface() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9BB0E0)(this);
}

// sub_9BB110
void spiderman_rvb_player_interface::frame_advance(f32 delta_t) {
    actor* owner = my_actor;

    // stuck below the world for two seconds without standing: dead, then put back at y 1.5
    if (!(owner->flags & entity_flag_suspended) &&
        owner->my_abs_po->get_position().y < -8.0f &&
        owner->has_standing_ifc()) {

        if (!owner->standing_ifc()->is_effectively_standing()) {
            fall_timer = (f32)((f64)fall_timer + (f64)delta_t);

            if (fall_timer > 2.0f) {
                actor* hero = my_actor;

                fall_timer = 0.0f;

                if (retail::sub_4024A0((u32*)hero)) // entity_base::has_damage_ifc
                    hero->damage_ifc()->set_hit_points(0);

                if (retail::sub_48E700((u8*)my_actor))
                    retail::sub_640D20((u8*)my_actor);

                vector3d position = my_actor->my_abs_po->get_position();
                position.y        = 1.5f;

                references::g_world_ptr.read()->malor_point(position, false, true);
            }
        }
    } else
        fall_timer = 0.0f;

    if (references::unk_00f4db94.read() != 1 || references::reload_current_upgrade_settings.read()) {
        references::unk_00f4db94                   .write(1);
        references::reload_current_upgrade_settings.write(0);

        unk_023(1);
    }

    unk_527(delta_t);
    unk_540(delta_t);

    if (get_command_button(plr_cmd_redsuit)->triggered())
        unk_573();

    if (auto* als_node = (als_inode*)my_actor->get_ai_core()->get_info_node(info_node_type_als)) {
        auto* als = als_node->my_als;

        if (rvb_black_mode) {
            als->get_als_layer(als::base_layer)->set_desired_param(als::param_player_in_blacksuit_mode, 1.0f);

            if (!suit_locked && unk_0538 && unk_0540 > 0.0f)
                als->get_als_layer(als::base_layer)->set_desired_param(als::param_player_desires_blacksuit_mode, 0.0f);
            else
                als->get_als_layer(als::base_layer)->set_desired_param(als::param_player_desires_blacksuit_mode, 1.0f);
        } else {
            als->get_als_layer(als::base_layer)->set_desired_param(als::param_player_in_blacksuit_mode, 0.0f);

            if (!suit_locked && unk_0538 && unk_0540 > 0.0f)
                als->get_als_layer(als::base_layer)->set_desired_param(als::param_player_desires_blacksuit_mode, 1.0f);
            else
                als->get_als_layer(als::base_layer)->set_desired_param(als::param_player_desires_blacksuit_mode, 0.0f);
        }

        u8 in_combat = (u8)unk_042();

        als->get_als_layer(als::base_layer)->set_desired_param(als::param_player_in_combat_mode, (f32)in_combat);

        bool in_focus = false;

        if (unk_048().value && unk_048().resolve())
            in_focus = true;

        als->get_als_layer(als::base_layer)->set_desired_param(als::param_player_in_focus_mode, (f32)in_focus);
    }

    references::unk_00f4d5a0.write(3.25f);
    references::unk_00f4d5a4.write(0.002f);
    references::unk_00f4d590.write(3.25f);
    references::unk_00f4d594.write(0.002f);

    retail::sub_574560((i32)swing, (i32)my_actor, delta_t); // swing_params::frame_advance

    vector3d velocity = references::unk_00f4de2c.read() ? *unk_095() : my_actor->physical_ifc()->get_velocity();

    cur_lateral_velocity = (f32)std::sqrt((f64)(f32)((f64)velocity.x * (f64)velocity.x + (f64)velocity.z * (f64)velocity.z));

    retail::sub_9A7D10((u8*)this, delta_t);

    if (mode.current != 9 && mode.current != 3)
        unk_027(1);

    references::unk_01112c58.write(0);

    switch (mode.current) {
        case 2:
        case 4:
        case 5:
        case 14:
            unk_544(delta_t);

            break;

        default:
            break;
    }

    if (is_crawl_jumping() || is_carrying_or_carried()) {
        unk_198();
        unk_203();
    } else if (mode.current != mode_last_frame) {
        if (!unk_0251 && mode.current != 3 && (mode_last_frame == 3 || mode_last_frame == 9))
            unk_203();

        unk_198();
    } else if (unk_490() || unk_494()) {
        unk_198();
        unk_203();
    } else if (mode.current >= 4 && (mode.current <= 5 || mode.current == 14))
        unk_201(delta_t);
    else
        unk_198();

    retail::sub_9A7B00((i32)this, delta_t);

    for (i32 index = 0; index < 2; ++index) {
        if (swingers[index]->frame_advance(this, delta_t, index)) {
            break_web((e_swinger_hand)index, false, true);
            unk_332(0.0f, true);
            unk_189(true);
            unk_136();
        }

        if (swingers[index]->attach_pendulum.enabled) {
            desired_swing_anchor_data.array[index].goal_constraint = swingers[index]->goal_constraint;
            desired_swing_anchor_data.array[index].unk_10          = swingers[index]->unk_54;
            desired_swing_anchor_data.array[index].pos             = swingers[index]->attach_pendulum.get_pivot_abs_pos();
        }
    }

    for (i32 index = 0; index < 8; ++index)
        swing_leftovers[index]->frame_advance(delta_t);

    unk_0830 = (f32)((f64)unk_0830 - (f64)delta_t);
    unk_0834 = (f32)((f64)unk_0834 - (f64)delta_t);
    unk_0838 = (f32)((f64)unk_0838 - (f64)delta_t);
    unk_0840 = (f32)((f64)unk_0840 - (f64)delta_t);
    unk_083c = (f32)((f64)unk_083c - (f64)delta_t);
    unk_0848 = (f32)((f64)unk_0848 - (f64)delta_t);
    unk_084c = (f32)((f64)unk_084c - (f64)delta_t);

    if (unk_0674) {
        unk_0674 = false;
        unk_0678 = (f32)((f64)delta_t + (f64)unk_0678);
    } else
        unk_0678 = 0.0f;

    if (mode.current == 8)
        unk_057c = 0.0f;
    else
        unk_057c = (f32)((f64)unk_057c - (f64)delta_t);

    if (unk_0b54 > 0.0f) {
        unk_0b54 = (f32)((f64)unk_0b54 - (f64)delta_t);

        if (0.0f > unk_0b54)
            unk_486(true, 0.0f);
    }

    if (unk_0b58 > 0.0f) {
        unk_0b58 = (f32)((f64)unk_0b58 - (f64)delta_t);

        if (0.0f > unk_0b58)
            unk_488(true, 0.0f);
    }

    forced_jump_type = 42;
    unk_0a1c = 106;

    unk_025(delta_t);

    retail::sub_987AC0((i32*)web_shooters, delta_t); // player_web_shooters::frame_advance

    unk_153(1);

    if (master_object) {
        script_arg time_arg;
        time_arg.type  = 1;
        time_arg.value = &delta_t;

        script_arg none[7];

        retail::sub_87CFC0(master_object, // script_object_instance: call a registered function
                           references::frame_advance_num.read().source_hash_code,
                           (u32*)&time_arg,
                           (u32*)&none[0],
                           (u32*)&none[1],
                           (u32*)&none[2],
                           (u32*)&none[3],
                           (u32*)&none[4],
                           (u32*)&none[5],
                           (u32*)&none[6]);
    }

    bool                focus_mode = get_command_button(plr_cmd_focus_mode)->triggered();
    game_meter_manager* meters     = references::game_meter_manager.read();

    if (unk_306()) {
        if (focus_mode ||
            retail::sub_9339C0((i32*)meters, 0) ||
            my_actor->damage_ifc()->hit_points.val <= 0)

            unk_0ba4 ^= 3;
    } else if (unk_307())
        unk_0ba4 &= ~2u;
    else if (focus_mode && retail::sub_933930((i32*)meters, 0))
        unk_0ba4 |= 1;

    player_interface::frame_advance(delta_t);

    unk_582(delta_t);
    unk_081(delta_t);
    unk_082(delta_t);

    if (unk_0c14 > 0.0f) {
        unk_0c14 = (f32)((f64)unk_0c14 - (f64)delta_t);

        if (unk_0c14 <= 0.0f) {
            unk_0c14 = 0.0f;

            if (unk_0c10.value && unk_0c10.resolve()) {
                if (collision_geometry* colgeom = ((entity_base*)unk_0c10.resolve())->get_colgeom()) {
                    colgeom->flags |= fi_collide_camera;

                    unk_0c10 = arch_base_vhandle();
                }
            }
        }
    }

    retail::sub_9A9360((u32**)this);

    unk_255(delta_t);
    unk_595(delta_t);

    if (noone_attack_me_time > 0.0f) {
        noone_attack_me_time = (f32)((f64)noone_attack_me_time - (f64)delta_t);

        if (noone_attack_me_time <= 0.0f) {
            if (auto* target = (combat_target_inode*)my_actor->get_ai_core()->get_info_node(info_node_type_combat_target)) {
                target->set_threat_capacity(10);
                target->set_engagement_capacity(10);
            }
        }
    }

    unk_06bc = my_actor->my_abs_po->get_position();
}

// sub_9BBC80
void spiderman_rvb_player_interface::post_process(f32 delta_t) {
    player_interface::post_process(delta_t);

    for (i32 index = 0; index < max_swingers; ++index)
        swingers[index]->post_process(this, delta_t, index);

    for (i32 index = 0; index < 8; ++index)
        swing_leftovers[index]->post_process(delta_t);

    if (get_post_process_flag(pp_swing_anchor_search))
        retail::sub_9B0800((u32*)this, unk_395()); // attempt_to_initiate_swing_check

    if (get_post_process_flag(pp_swing_avoid_ground))
        unk_460(delta_t);
}

// slot 96
bool spiderman_rvb_player_interface::unk_096(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_720DD0)(this, unk);
}

// slot 97
bool spiderman_rvb_player_interface::unk_097() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720DE0)(this);
}

// slot 98
bool spiderman_rvb_player_interface::unk_098(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_720DF0)(this, unk);
}

// sub_9A4400
bool spiderman_rvb_player_interface::is_walking() const {
    return get_movement_speed() >= 0.1f && get_movement_speed() <= 0.5f;
}

// slot 100
i32 spiderman_rvb_player_interface::unk_100() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A4440)(this);
}

// slot 101
i32 spiderman_rvb_player_interface::unk_101() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A4480)(this);
}

// slot 107
i32 spiderman_rvb_player_interface::unk_107() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720E30)(this);
}

// slot 108
i32 spiderman_rvb_player_interface::unk_108(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_720E40)(this, unk);
}

// sub_9A44A0
bool spiderman_rvb_player_interface::exterior_transition() {
    return _exterior_transition && (crawling() || is_wallsprinting()) && is_transition();
}

// slot 114
bool spiderman_rvb_player_interface::unk_114(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A44F0)(this, unk);
}

// sub_9A4510
bool spiderman_rvb_player_interface::exterior_transition_180() {
    return _exterior_transition_180 && (crawling() || is_wallsprinting()) && is_transition();
}

// slot 116
bool spiderman_rvb_player_interface::unk_116(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A4560)(this, unk);
}

// slot 118
i32 spiderman_rvb_player_interface::unk_118() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A4580)(this);
}

// slot 119
bool spiderman_rvb_player_interface::unk_119(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A45A0)(this, unk);
}

// slot 121
i32 spiderman_rvb_player_interface::unk_121() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A77B0)(this);
}

// slot 125
bool spiderman_rvb_player_interface::swing_r_hand() const {
    return ((bool (__thiscall*)(const spiderman_rvb_player_interface*))retail::sub_720E50)(this);
}

// slot 126
bool spiderman_rvb_player_interface::swing_l_hand() const {
    return ((bool (__thiscall*)(const spiderman_rvb_player_interface*))retail::sub_720E60)(this);
}

// slot 129
bool spiderman_rvb_player_interface::swap_swing_hands(bool refire) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, bool))retail::sub_9A77D0)(this, refire);
}

// slot 130
bool spiderman_rvb_player_interface::swing_r_hand_locked() const {
    return ((bool (__thiscall*)(const spiderman_rvb_player_interface*))retail::sub_9A67B0)(this);
}

// slot 131
bool spiderman_rvb_player_interface::swing_l_hand_locked() const {
    return ((bool (__thiscall*)(const spiderman_rvb_player_interface*))retail::sub_9A67C0)(this);
}

// sub_9A6550
void spiderman_rvb_player_interface::lock_swing_r_hand(bool l) {
    swing->r_hand_is_locked = l;

    if (swing->r_hand_is_locked) {
        if (swing_l_hand() || swing_l_hand_locked())
            break_web(swinger_right, false, true);
        else if (active_swinger == swinger_left)
            swap_swing_hands(false);
    }
}

// sub_9A65D0
void spiderman_rvb_player_interface::lock_swing_l_hand(bool l) {
    swing->l_hand_is_locked = l;

    if (swing->l_hand_is_locked) {
        if (swing_r_hand() || swing_r_hand_locked())
            break_web(swinger_left, false, true);
        else if (active_swinger == swinger_right)
            swap_swing_hands(false);
    }
}

// slot 136
i32 spiderman_rvb_player_interface::unk_136() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9B0A10)(this);
}

// sub_9A45C0
bool spiderman_rvb_player_interface::on_ground() const {
    return my_actor->my_abs_po->get_y_facing().y > 0.7f;
}

// sub_9A45F0
bool spiderman_rvb_player_interface::on_ceiling() const {
    return -0.7f > my_actor->my_abs_po->get_y_facing().y;
}

// slot 141
i32 spiderman_rvb_player_interface::unk_141() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A4620)(this);
}

// sub_9A5280
void spiderman_rvb_player_interface::inc_jump_charge(f32 delta_t) {
    jump_charge_time = (f32)((f64)delta_t + (f64)jump_charge_time);

    if (jump_charge_time > jump_max_charge_time)
        jump_charge_time = jump_max_charge_time;
}

// sub_9A52D0
void spiderman_rvb_player_interface::set_abs_jump_charge(f32 fraction) {
    jump_charge_time = (f32)(((f64)jump_max_charge_time - (f64)jump_min_charge_time) * (f64)fraction + (f64)jump_min_charge_time);
}

// sub_9A5310
void spiderman_rvb_player_interface::clear_jump_charge(bool store_for_als) {
    stored_als_jump_charge = store_for_als ? get_jump_charge() : 0.0f;
    jump_charge_time       = 0.0f;
}

// sub_9A5350
f32 spiderman_rvb_player_interface::get_jump_charge() const {
    if (jump_charge_time > jump_min_charge_time)
        return (jump_charge_time - jump_min_charge_time) / (jump_max_charge_time - jump_min_charge_time);

    return 0.0f;
}

// slot 149
f32 spiderman_rvb_player_interface::unk_149() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720EA0)(this);
}

// slot 152
bool spiderman_rvb_player_interface::unk_152() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720EB0)(this);
}

// slot 153
bool spiderman_rvb_player_interface::unk_153(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_720EC0)(this, unk);
}

// sub_99CDE0
bool spiderman_rvb_player_interface::unk_156() {
    bool immune = false;

    if (als::animation_logic_system_interface* the_als = my_actor->get_als()) {
        i32 not_immune = 0;

        als::state_machine_interface* layer = the_als->get_als_layer(als::base_layer);

        string_hash immune_to_gunfire;
        immune_to_gunfire.initialize(mash::ALLOCATED, "immune_to_gunfire");

        immune = layer->get_optional_pb_int(immune_to_gunfire, not_immune, nullptr) != 0;
    }

    if (unk_030() > 20.0f)
        return true;

    return immune;
}

// slot 157
i32 spiderman_rvb_player_interface::predict_jump_type() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A9A50)(this);
}

// slot 158
i32 spiderman_rvb_player_interface::unk_158(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721980)(this, unk);
}

// slot 163
void spiderman_rvb_player_interface::unk_163() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6F70)(this);
}

// slot 164
void spiderman_rvb_player_interface::unk_164() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6FA0)(this);
}

// slot 165
i32 spiderman_rvb_player_interface::unk_165() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7217E0)(this);
}

// slot 166
i32 spiderman_rvb_player_interface::unk_166() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7217F0)(this);
}

// slot 167
bool spiderman_rvb_player_interface::unk_167(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721880)(this, unk);
}

// slot 168
bool spiderman_rvb_player_interface::unk_168() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721890)(this);
}

// slot 169
bool spiderman_rvb_player_interface::unk_169(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721800)(this, unk);
}

// slot 170
bool spiderman_rvb_player_interface::unk_170() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721810)(this);
}

// slot 171
bool spiderman_rvb_player_interface::unk_171(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721820)(this, unk);
}

// slot 172
bool spiderman_rvb_player_interface::unk_172() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721830)(this);
}

// slot 173
bool spiderman_rvb_player_interface::unk_173(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721840)(this, unk);
}

// slot 174
bool spiderman_rvb_player_interface::unk_174() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721850)(this);
}

// slot 175
bool spiderman_rvb_player_interface::unk_175(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721860)(this, unk);
}

// slot 176
bool spiderman_rvb_player_interface::unk_176() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721870)(this);
}

// slot 177
bool spiderman_rvb_player_interface::unk_177(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_7218A0)(this, unk);
}

// slot 178
bool spiderman_rvb_player_interface::unk_178() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7218B0)(this);
}

// slot 179
bool spiderman_rvb_player_interface::unk_179(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_7218C0)(this, unk);
}

// slot 180
bool spiderman_rvb_player_interface::unk_180() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7218D0)(this);
}

// slot 188
bool spiderman_rvb_player_interface::unk_188() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6000)(this);
}

// slot 192
bool spiderman_rvb_player_interface::unk_192() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720EE0)(this);
}

// slot 193
bool spiderman_rvb_player_interface::unk_193(bool unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, bool))retail::sub_720EF0)(this, unk);
}

// sub_9A98A0
bool spiderman_rvb_player_interface::handle_jump_charge(f32 delta_t) {
    game_button* jump = get_command_button(plr_cmd_jump);

    if (!unk_186()) {
        if (!jump->pressed()) {
            unk_187(1);

            return false;
        }

        if (!jump->pressed()) {
            clear_jump_charge(false);

            return false;
        }

        if (unk_184())
            inc_jump_charge(delta_t);

        return false;
    }

    player_jump_info* info = get_jump_info(predict_jump_type());

    if (!((bool (__thiscall*)(player_jump_info*))(*(void***)info)[9])(info)) { // slot 9
        clear_jump_charge(false);

        return false;
    }

    if (((bool (__thiscall*)(player_jump_info*))(*(void***)info)[24])(info)) { // slot 24
        if (jump->triggered_or_missed_press())
            mario_jump_pressed = true;
        else if (!mario_jump_pressed)
            return false;

        if (!jump->released() && !(jump->get_pressed_time() > 0.125f))
            return false;

        if (jump->get_pressed_time() > 0.125f)
            set_abs_jump_charge(1.0f);

        mario_jump_pressed = false;

        return true;
    }

    if (jump->pressed() && unk_184()) {
        inc_jump_charge(delta_t);

        return false;
    }

    if (jump->released())
        return true;

    clear_jump_charge(false);

    return false;
}

// slot 195
i32 spiderman_rvb_player_interface::unk_195() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A53B0)(this);
}

// sub_9A53D0
vector3d spiderman_rvb_player_interface::unk_196() {
    switch (unk_068c.kind) {
        case 1:
            return unk_068c.position;

        case 2:
            if (auto* target = (entity_base*)unk_068c.handle.resolve())
                return target->my_abs_po->get_position();

            break;

        default:
            break;
    }

    return references::unk_01112ca0.get();
}

// slot 197
i32 spiderman_rvb_player_interface::unk_197() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A5490)(this);
}

// slot 198
void spiderman_rvb_player_interface::unk_198() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A54A0)(this);
}

// slot 201
i32 spiderman_rvb_player_interface::unk_201(f32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9AAA50)(this, unk);
}

// slot 202
i32 spiderman_rvb_player_interface::unk_202() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A57D0)(this);
}

// slot 203
void spiderman_rvb_player_interface::unk_203() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A57E0)(this);
}

// sub_9A57F0
vector3d spiderman_rvb_player_interface::unk_204() {
    switch (unk_06c8.kind) {
        case 1:
        case 2:
            return unk_06c8.position;

        case 3: {
            po*        abs_po = ((entity_base*)unk_06c8.handle.resolve())->my_abs_po;
            matrix4x4 &m      = abs_po->get_matrix();

            m.x.w = 0.0f;
            m.y.w = 0.0f;
            m.z.w = 0.0f;
            m.w.w = 1.0f;

            return abs_po->non_affine_slow_xform(unk_06c8.position);
        }

        default:
            return references::unk_01112ca0.get();
    }
}

// slot 205
void* spiderman_rvb_player_interface::unk_205(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_9AA1D0)(this, unk);
}

// slot 206
i32 spiderman_rvb_player_interface::unk_206(f32 unk, entity* unk_2, i32 unk_3) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, f32, entity*, i32))retail::sub_9AB550)(this, unk, unk_2, unk_3);
}

// slot 207
void spiderman_rvb_player_interface::unk_207() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A5E20)(this);
}

// slot 208
void spiderman_rvb_player_interface::unk_208(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_720FE0)(this, unk);
}

// slot 209
void spiderman_rvb_player_interface::unk_209(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721000)(this, unk);
}

// slot 210
f32 spiderman_rvb_player_interface::unk_210() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721020)(this);
}

// sub_7211E0
i32 spiderman_rvb_player_interface::unk_215() {
    return unk_0c0c;
}

// sub_9B3DC0
void spiderman_rvb_player_interface::init_attacks() {
    player_interface::init_attacks();

    f32 cone_a = math::cos(math::math_pi / 4.0f);
    f32 cone_b = math::cos(math::math_pi / 4.0f);
    f32 cone_c = math::cos(math::math_pi / 4.0f);

    attacks[combat_move_light_melee] = new spiderman_rvb_melee_or_car_combat_attack();
    attacks[combat_move_light_melee]->init(get_command_button(plr_cmd_combat_light_melee),
                                           true,
                                           0x051580C1,
                                           0.0f,
                                           5.0f,
                                           cone_a,
                                           -5.0f,
                                           5.0f,
                                           -1.1f,
                                           true,
                                           false);

    attacks[combat_move_heavy_melee] = new spiderman_rvb_melee_or_car_combat_attack();
    attacks[combat_move_heavy_melee]->init(get_command_button(plr_cmd_combat_heavy_melee),
                                           true,
                                           0x051580C1,
                                           0.0f,
                                           5.0f,
                                           cone_a,
                                           -4.0f,
                                           4.0f,
                                           -1.1f,
                                           true,
                                           false);

    attacks[combat_move_special_attack] = nullptr;

    attacks[combat_move_grapple] = new spiderman_rvb_grapple_attack();
    attacks[combat_move_grapple]->init(get_command_button(plr_cmd_combat_grapple),
                                       false,
                                       0x009500C1,
                                       0.0f,
                                       4.0f,
                                       cone_a,
                                       -3.0f,
                                       3.0f,
                                       -1.1f,
                                       false,
                                       false);

    attacks[combat_move_light_ranged] = new spiderman_rvb_web_yank_attack();
    attacks[combat_move_light_ranged]->init(get_command_button(plr_cmd_combat_light_ranged),
                                            true,
                                            0x009580C1,
                                            0.0f,
                                            30.0f,
                                            -1.1f,
                                            -30.0f,
                                            30.0f,
                                            -1.1f,
                                            false,
                                            false);

    attacks[combat_move_boss_finisher] = nullptr;

    attacks[combat_move_car_finisher] = new spiderman_rvb_car_finish_attack();
    attacks[combat_move_car_finisher]->init(get_command_button(plr_cmd_combat_car_finisher),
                                            false,
                                            0x001500E1,
                                            0.0f,
                                            15.0f,
                                            -1.1f,
                                            -15.0f,
                                            15.0f,
                                            -1.1f,
                                            true,
                                            false);

    attacks[combat_move_jump_attack] = new spiderman_rvb_air_jump_attack();
    attacks[combat_move_jump_attack]->init(get_command_button(plr_cmd_combat_jump_attack),
                                           true,
                                           0x001580C1,
                                           0.0f,
                                           6.0f,
                                           cone_a,
                                           -10.0f,
                                           10.0f,
                                           -1.1f,
                                           true,
                                           true);

    attacks[combat_move_bounce_attack] = nullptr;

    attacks[combat_move_throw_manip] = new spiderman_rvb_throw_manip_attack();
    attacks[combat_move_throw_manip]->init(get_command_button(plr_cmd_combat_throw_manip),
                                           true,
                                           0x001500C1,
                                           0.0f,
                                           30.0f,
                                           cone_a,
                                           -30.0f,
                                           30.0f,
                                           -1.1f,
                                           false,
                                           false);

    attacks[combat_move_stealth_kill] = nullptr;

    attacks[combat_move_dodge] = new plr_combat_attack();
    attacks[combat_move_dodge]->init(nullptr,
                                     false,
                                     0,
                                     0.0f,
                                     9999.0f,
                                     -1.1f,
                                     -9999.0f,
                                     9999.0f,
                                     -1.1f,
                                     true,
                                     false);

    attacks[combat_move_manual_dodge] = nullptr;

    attacks[combat_move_grab_civ] = nullptr;

    attacks[combat_move_save_civ] = new spiderman_rvb_civ_save_attack();
    attacks[combat_move_save_civ]->init(get_command_button(plr_cmd_combat_shaba_save_civ),
                                        false,
                                        0x005500C4,
                                        0.0f,
                                        2.5f,
                                        -1.0f,
                                        -2.5f,
                                        2.5f,
                                        -1.1f,
                                        false,
                                        false);

    attacks[combat_move_boost_power_attack] = new spiderman_rvb_special_y_attack();
    attacks[combat_move_boost_power_attack]->init(get_command_button(plr_cmd_combat_boost_power_attack),
                                                  true,
                                                  0x001500C1,
                                                  0.0f,
                                                  10.0f,
                                                  -1.1f,
                                                  -10.0f,
                                                  10.0f,
                                                  -1.1f,
                                                  true,
                                                  false);

    attacks[combat_move_boost_speed_attack] = new spiderman_rvb_special_x_attack();
    attacks[combat_move_boost_speed_attack]->init(get_command_button(plr_cmd_combat_boost_speed_attack),
                                                  true,
                                                  0x001500C1,
                                                  0.0f,
                                                  20.0f,
                                                  -1.1f,
                                                  -10.0f,
                                                  10.0f,
                                                  -1.1f,
                                                  true,
                                                  false);

    attacks[combat_move_boost_air_attack] = nullptr;

    attacks[combat_move_boost_web_attack] = new spiderman_rvb_special_b_attack();
    attacks[combat_move_boost_web_attack]->init(get_command_button(plr_cmd_combat_boost_web_attack),
                                                true,
                                                0x001500C1,
                                                0.0f,
                                                10.0f,
                                                -1.1f,
                                                -10.0f,
                                                10.0f,
                                                -1.1f,
                                                true,
                                                false);

    attacks[combat_move_momentum_light] = nullptr;

    attacks[combat_move_momentum_heavy] = new spiderman_rvb_momentum_attack();
    attacks[combat_move_momentum_heavy]->init(get_command_button(plr_cmd_combat_heavy_melee),
                                              true,
                                              0x011500D1,
                                              2.5f,
                                              30.0f,
                                              cone_b,
                                              -30.0f,
                                              30.0f,
                                              -1.1f,
                                              true,
                                              true);

    attacks[combat_move_momentum_grapple] = nullptr;

    attacks[combat_move_swing_light] = new spiderman_rvb_swing_attack();
    attacks[combat_move_swing_light]->init(get_command_button(plr_cmd_combat_light_melee),
                                           true,
                                           0x011500D1,
                                           3.0f,
                                           15.0f,
                                           cone_c,
                                           -5.0f,
                                           5.0f,
                                           -1.1f,
                                           true,
                                           true);

    attacks[combat_move_swing_heavy] = new spiderman_rvb_swing_attack();
    attacks[combat_move_swing_heavy]->init(get_command_button(plr_cmd_combat_heavy_melee),
                                           true,
                                           0x011500D1,
                                           3.0f,
                                           15.0f,
                                           cone_c,
                                           -5.0f,
                                           5.0f,
                                           -1.1f,
                                           true,
                                           true);

    attacks[combat_move_swing_grapple] = new spiderman_rvb_swing_attack();
    attacks[combat_move_swing_grapple]->init(get_command_button(plr_cmd_combat_grapple),
                                             true,
                                             0x011500D5,
                                             3.0f,
                                             15.0f,
                                             cone_c,
                                             -5.0f,
                                             5.0f,
                                             -1.1f,
                                             true,
                                             true);

    for (i32 move = combat_move_light_wall_attack; move <= combat_move_pole_swing_grapple; ++move)
        attacks[move] = nullptr;

    attacks[combat_move_counter_attack] = new spiderman_rvb_counter_attack();
    attacks[combat_move_counter_attack]->init(get_command_button(plr_cmd_combat_counter_attack),
                                              false,
                                              0,
                                              0.0f,
                                              0.0f,
                                              0.0f,
                                              0.0f,
                                              0.0f,
                                              -1.1f,
                                              true,
                                              true);

    clear_attacks(true);
}

// sub_9A6680
void spiderman_rvb_player_interface::update_attacks(f32 delta_t) {
    player_interface::update_attacks(delta_t);
}

// sub_9B4860
void spiderman_rvb_player_interface::init_tricks() {
    player_interface::init_tricks();

    for (i32 index = 0; index < 4; ++index) {
        tricks[index] = new plr_trick_move();
        tricks[index]->init(get_command_button((e_player_button_command)(plr_cmd_trick_a + index)));
    }

    clear_tricks();
}

// sub_9B4C00
bool spiderman_rvb_player_interface::check_for_block(      entity_base*               attacker,
                                                           i32,
                                                     const combo_move_data*           combo_move,
                                                           hit_react_record*          react,
                                                           weapon_interface_extender* weapon,
                                                           entity_base*               weapon_entity,
                                                           i32) {

    if (!unk_04a5) {
        unk_077c = false;

        return unk_04a5;
    }

    f32 block = 0.0f;

    unk_0768 = 0;
    unk_076c = 1.0f;
    unk_077e = false;

    if (combo_move) {
        if (combo_move->flags_2 & combo_flag_allow_target_block) {
            block = (f32)combo_move->params.get_optional_pb_int(references::damage.read(), 0, nullptr);

            if (react) {
                unk_0768 = react->hit_react_level;
                unk_076c = react->scales[0].scale.get_random_value();
            }
        } else
            block = (f32)((f64)10000.0f + 1.0);

        unk_077e = (react && react->hit_react_level != -1) || block != 0.0f;
    } else if (weapon) {
        entity* holder = weapon->my_entity;

        if (holder->has_ifc(entity_ifc_gun_weapon))
            block = (f32)((gun_weapon_interface*)holder->my_ifc_storage->get_ifc(entity_ifc_gun_weapon))->unk_020;
        else if (holder->has_ifc(entity_ifc_thrown_weapon)) {
            block    = (f32)((thrown_weapon_interface*)holder->my_ifc_storage->get_ifc(entity_ifc_thrown_weapon))->unk_010;
            unk_077e = true;
        } else if ((u8)retail::sub_4E1D20((u32*)holder)) { // has_ifc(entity_ifc_melee_weapon)
            block    = (f32)((f64)((melee_weapon_interface*)holder->my_ifc_storage->get_ifc(entity_ifc_melee_weapon))->unk_010 * 0.0);
            unk_0768 = 1;
            unk_077e = true;
        }
    }

    unk_0764 = weapon && weapon_entity ? weapon_entity->my_handle : arch_base_vhandle();

    if (attacker) {
        unk_0760 = attacker->my_handle;

        if (unk_077e)
            unk_0780 = attacker->my_handle;
    }

    if (unk_077c)
        block = (f32)((f64)block + (f64)unk_0770);

    unk_0770 = block;
    unk_077c = true;

    return unk_04a5;
}

// sub_721060
arch_base_vhandle spiderman_rvb_player_interface::unk_243() {
    return unk_0760;
}

// sub_721070
arch_base_vhandle spiderman_rvb_player_interface::unk_244() const {
    return unk_0764;
}

// sub_721080
i32 spiderman_rvb_player_interface::unk_245() {
    return unk_0768;
}

// sub_721090
f32 spiderman_rvb_player_interface::unk_246() {
    return unk_076c;
}

// sub_7210A0
f32 spiderman_rvb_player_interface::unk_247() {
    return unk_0770;
}

// sub_7210B0
bool spiderman_rvb_player_interface::unk_248() {
    return unk_077c;
}

// sub_7210C0
void spiderman_rvb_player_interface::unk_249(bool unk) {
    unk_077c = unk;
}

// sub_720F00
f32 spiderman_rvb_player_interface::unk_250() {
    return unk_0774;
}

// slot 251
f32 spiderman_rvb_player_interface::get_block_strength() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720040)(this);
}

// sub_720F10
void spiderman_rvb_player_interface::unk_252(f32 unk) {
    unk_0774 = unk;
}

// sub_720F30
f32 spiderman_rvb_player_interface::unk_253() {
    return unk_0778;
}

// sub_720F40
void spiderman_rvb_player_interface::unk_254(f32 unk) {
    unk_0778 = unk;
}

// sub_9B4B20
void spiderman_rvb_player_interface::unk_255(f32) {
    if (mode.current != 19) {
        unk_0774 = 10000.0f;
        unk_0778 = 10000.0f;
    }
}

// sub_9A6690
void spiderman_rvb_player_interface::unk_256() {
    unk_0774 = 10000.0f;
    unk_0778 = 10000.0f;
}

// sub_7210D0
bool spiderman_rvb_player_interface::unk_257() {
    return unk_077e;
}

// sub_7210E0
arch_base_vhandle spiderman_rvb_player_interface::unk_258() const {
    return unk_0780;
}

// slot 262
i32 spiderman_rvb_player_interface::unk_262(i32 unk, i32 unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32, i32))retail::sub_9B2610)(this, unk, unk_2);
}

// slot 263
bool spiderman_rvb_player_interface::unk_263(void* unk, i32 unk_2) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, void*, i32))retail::sub_9B4E00)(this, unk, unk_2);
}

// slot 264
bool spiderman_rvb_player_interface::unk_264(f32 delta_t, void* unk_2, i32 unk_3, i32 unk_4, i32 unk_5) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, f32, void*, i32, i32, i32))retail::sub_9BDEF0)(this, delta_t, unk_2, unk_3, unk_4, unk_5);
}

// sub_9B4A60
bool spiderman_rvb_player_interface::unk_265() {
    game_button* block_button = get_command_button(plr_cmd_block);

    if (block_button->released() || !block_button->pressed())
        unk_193(true);

    return unk_192()                         &&
           unk_077() == 1                    &&
           get_command_button(plr_cmd_block)->pressed() &&
           !is_carrying_or_carried()         &&
           !(player_axis[0]->get_force() > 0.3f && unk_341());
}

// sub_9A66C0
void spiderman_rvb_player_interface::force_evade(vector3d v) {
    unk_0850 = true;
    unk_0854 = v;
}

// sub_9A6700
void spiderman_rvb_player_interface::unk_267() {
    unk_0850 = false;
    unk_0854 = references::unk_01112ca0.get();
}

// sub_721030
bool spiderman_rvb_player_interface::unk_268() {
    return unk_0850;
}

// sub_721040
vector3d spiderman_rvb_player_interface::unk_269() {
    return unk_0854;
}

// sub_9B49B0
bool spiderman_rvb_player_interface::unk_270() {
    if (unk_0850)
        return true;

    game_button* evade_button = get_command_button(plr_cmd_evade);

    if (evade_button->released() || !evade_button->pressed())
        unk_191(true);

    return unk_190()                         &&
           !unk_341()                        &&
           unk_077() == 1                    &&
           get_command_button(plr_cmd_evade)->pressed() &&
           player_axis[0]->get_force() > 0.3f;
}

// sub_9B4B50
bool spiderman_rvb_player_interface::unk_271() {
    if (unk_0850)
        return true;

    game_button* evade_button = get_command_button(plr_cmd_air_evade);

    if (evade_button->released() || !evade_button->pressed())
        unk_191(true);

    return unk_190()                         &&
           !unk_341()                        &&
           unk_077() == 1                    &&
           get_command_button(plr_cmd_air_evade)->pressed() &&
           player_axis[0]->get_force() > 0.3f;
}

// slot 272
bool spiderman_rvb_player_interface::unk_272() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9BD1D0)(this);
}

// sub_9A6740
entity* spiderman_rvb_player_interface::unk_273(entity* target) {
    if (!target->is_a_vehicle()) {
        if (retail::sub_58EE40((i32)target)) {
            retail::sub_58EEA0((i32)target, 1);

            if (auto* promoted = (entity*)retail::sub_5A76D0((u32*)target))
                return promoted;
        } else if (target->is_a_placeholder_vehicle())
            return (entity*)retail::sub_5A70C0((i32)target);
    }

    return target;
}

// slot 284
bool spiderman_rvb_player_interface::unk_284() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_667F80)(this);
}

// sub_720DA0
bool spiderman_rvb_player_interface::is_in_rvb_black_mode() const {
    return rvb_black_mode;
}

// slot 306
i32 spiderman_rvb_player_interface::unk_306() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A43E0)(this);
}

// slot 307
i32 spiderman_rvb_player_interface::unk_307() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A43F0)(this);
}

// slot 308
f32 spiderman_rvb_player_interface::unk_308() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720DB0)(this);
}

// slot 310
f32 spiderman_rvb_player_interface::unk_310() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720DC0)(this);
}

// slot 315
f32 spiderman_rvb_player_interface::unk_315() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721690)(this);
}

// slot 319
bool spiderman_rvb_player_interface::unk_319(f32 delta_t, game_button* button, i32 unk_3) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, f32, game_button*, i32))retail::sub_9BC680)(this, delta_t, button, unk_3);
}

// slot 320
void* spiderman_rvb_player_interface::unk_320() {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7213E0)(this);
}

// slot 321
void* spiderman_rvb_player_interface::unk_321(const line_info* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, const line_info*))retail::sub_7213F0)(this, unk);
}

// slot 322
bool spiderman_rvb_player_interface::unk_322(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A5050)(this, unk);
}

// slot 323
void* spiderman_rvb_player_interface::unk_323(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_7214F0)(this, unk);
}

// slot 324
i32 spiderman_rvb_player_interface::unk_324(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721520)(this, unk);
}

// sub_9A4FE0
f32 spiderman_rvb_player_interface::unk_325() {
    f32 fraction = unk_0844 > 0.0f ?
        (f32)((f64)unk_0840 / (f64)unk_0844) :
        (f32)((f64)unk_0840 / (f64)(1.0f / 3.0f));

    if (0.0f > fraction)
        fraction = 0.0f;
    else if (fraction > 1.0f)
        fraction = 1.0f;

    return (f32)std::sqrt((f64)fraction);
}

// slot 326
bool spiderman_rvb_player_interface::unk_326() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721540)(this);
}

// slot 327
bool spiderman_rvb_player_interface::unk_327(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721550)(this, unk);
}

// sub_9A4EC0
void spiderman_rvb_player_interface::unk_330(f32 duration, bool force) {
    if (0.0f > duration)
        duration = 0.2f;

    if (force || duration > unk_0830)
        unk_0830 = duration;
}

// sub_9A4F00
void spiderman_rvb_player_interface::unk_331(f32 duration, bool force) {
    if (0.0f > duration)
        duration = 0.75f;

    if (force || duration > unk_0834)
        unk_0834 = duration;
}

// sub_9A4F40
void spiderman_rvb_player_interface::unk_332(f32 duration, bool force) {
    if (0.0f > duration)
        duration = 0.65f;

    if (force || duration > unk_0838)
        unk_0838 = duration;
}

// sub_9A4F80
void spiderman_rvb_player_interface::unk_333(f32 duration, bool force) {
    unk_322(1);

    if (0.0f > duration)
        duration = 1.0f / 3.0f;

    if (force || duration > unk_0840) {
        unk_0840 = duration;
        unk_0844 = duration;
    }
}

// sub_9A5070
void spiderman_rvb_player_interface::unk_334(f32 duration, bool force) {
    if (0.0f > duration)
        duration = 0.0f;

    if (force || duration > unk_083c)
        unk_083c = duration;
}

// sub_9A50B0
void spiderman_rvb_player_interface::unk_335(f32 duration, bool force) {
    if (0.0f > duration)
        duration = 0.25f;

    if (force || duration > unk_084c)
        unk_084c = duration;
}

// sub_9A50F0
void spiderman_rvb_player_interface::unk_336(f32 duration, bool force) {
    if (0.0f > duration)
        duration = 0.5f;

    if (force || duration > unk_0848)
        unk_0848 = duration;
}

// slot 337
i32 spiderman_rvb_player_interface::unk_337() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721400)(this);
}

// slot 338
i32 spiderman_rvb_player_interface::unk_338() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721420)(this);
}

// slot 339
i32 spiderman_rvb_player_interface::unk_339() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721440)(this);
}

// slot 340
bool spiderman_rvb_player_interface::unk_340() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721460)(this);
}

// slot 341
bool spiderman_rvb_player_interface::unk_341() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721480)(this);
}

// slot 342
i32 spiderman_rvb_player_interface::unk_342() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7214A0)(this);
}

// slot 343
i32 spiderman_rvb_player_interface::unk_343() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7214C0)(this);
}

// slot 344
void spiderman_rvb_player_interface::unk_344(f32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A6A40)(this, unk);
}

// slot 345
void spiderman_rvb_player_interface::unk_345(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6A60)(this, unk);
}

// slot 346
void spiderman_rvb_player_interface::unk_346(f32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A6A80)(this, unk);
}

// slot 347
void spiderman_rvb_player_interface::unk_347(f32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A6AA0)(this, unk);
}

// slot 348
void spiderman_rvb_player_interface::unk_348(f32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A6AC0)(this, unk);
}

// slot 349
void spiderman_rvb_player_interface::unk_349(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6AE0)(this, unk);
}

// slot 350
void spiderman_rvb_player_interface::unk_350(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6B00)(this, unk);
}

// slot 351
void spiderman_rvb_player_interface::unk_351(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6B60)(this, unk);
}

// slot 352
void spiderman_rvb_player_interface::unk_352(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6B20)(this, unk);
}

// slot 353
void spiderman_rvb_player_interface::unk_353(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6B80)(this, unk);
}

// slot 354
void spiderman_rvb_player_interface::unk_354(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6BA0)(this, unk);
}

// slot 355
void spiderman_rvb_player_interface::unk_355(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6BC0)(this, unk);
}

// slot 356
void spiderman_rvb_player_interface::unk_356(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6B40)(this, unk);
}

// slot 357
void spiderman_rvb_player_interface::unk_357(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6BE0)(this, unk);
}

// slot 358
void spiderman_rvb_player_interface::unk_358(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6C00)(this, unk);
}

// slot 359
void spiderman_rvb_player_interface::unk_359(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6C20)(this, unk);
}

// slot 360
void spiderman_rvb_player_interface::unk_360(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6C40)(this, unk);
}

// slot 361
void spiderman_rvb_player_interface::unk_361(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6C60)(this, unk);
}

// sub_9A5E40
void spiderman_rvb_player_interface::get_sweet_spot_params(f32* incline_sin,
                                                           f32* incline_cos,
                                                           f32* sweet_cone_cos,
                                                           f32* distance,
                                                           f32* min_len_squared,
                                                           f32* max_len_squared) {

    *incline_sin     = swing->sweet_spot_incline_angle_sin;
    *incline_cos     = swing->sweet_spot_incline_angle_cos;
    *distance        = swing->sweet_spot_distance;
    *min_len_squared = swing->min_web_length_squared;
    *max_len_squared = swing->max_web_length_squared;
    *sweet_cone_cos  = swing->sweet_cone_angle_cos;
}

// sub_9A5E80
void spiderman_rvb_player_interface::get_dynamic_sweet_spot_params(f32* min_length,
                                                                   f32* max_length,
                                                                   f32* overdrive_length,
                                                                   f32* overdrive_velocity,
                                                                   f32* max_no_anchor_length,
                                                                   f32* min_angle,
                                                                   f32* max_angle) {

    *min_length           = swing->min_dynamic_sweet_length;
    *max_length           = swing->max_dynamic_sweet_length;
    *overdrive_length     = swing->max_high_speed_sweet_length;
    *overdrive_velocity   = swing->sweet_spot_overdrive_velocity;
    *max_no_anchor_length = swing->max_no_anchor_sweet_length;
    *min_angle            = swing->min_dynamic_sweet_angle;
    *max_angle            = swing->max_dynamic_sweet_angle;
}

// slot 365
i32 spiderman_rvb_player_interface::unk_365(void* unk, void* unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, void*, void*))retail::sub_9A5ED0)(this, unk, unk_2);
}

// slot 366
void* spiderman_rvb_player_interface::unk_366(void* unk, void* unk_2, void* unk_3, void* unk_4) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*, void*, void*, void*))retail::sub_9A5F00)(this, unk, unk_2, unk_3, unk_4);
}

// slot 367
i32 spiderman_rvb_player_interface::unk_367(void* unk, void* unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, void*, void*))retail::sub_9A5F30)(this, unk, unk_2);
}

// slot 368
void spiderman_rvb_player_interface::unk_368(f32 unk, swing_data* swinger) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32, swing_data*))retail::sub_9AD300)(this, unk, swinger);
}

// slot 369
void spiderman_rvb_player_interface::unk_369(i32 unk, i32 unk_2) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32, i32))retail::sub_9A6130)(this, unk, unk_2);
}

// slot 370
bool spiderman_rvb_player_interface::break_web(e_swinger_hand web_to_break, bool add_momentum, bool add_dangler) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, e_swinger_hand, bool, bool))retail::sub_9A4650)(this,
                                                                                                                 web_to_break,
                                                                                                                 add_momentum,
                                                                                                                 add_dangler);
}

// slot 371
i32 spiderman_rvb_player_interface::unk_371(i32 unk, i32 unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32, i32))retail::sub_9A46D0)(this, unk, unk_2);
}

// slot 373
i32 spiderman_rvb_player_interface::unk_373() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A4740)(this);
}

// slot 374
f32 spiderman_rvb_player_interface::unk_374() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720E70)(this);
}

// slot 375
f32 spiderman_rvb_player_interface::unk_375() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720E90)(this);
}

// slot 376
i32 spiderman_rvb_player_interface::unk_376() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721570)(this);
}

// sub_9A5FA0
bool spiderman_rvb_player_interface::unk_377(vector3d* point) {
    if (!point)
        return false;

    for (i32 index = 0; index < max_swingers; ++index) {
        if (swingers[index]->attach_pendulum.enabled) {
            *point = swingers[index]->unk_54;

            return true;
        }
    }

    return false;
}

// slot 378
bool spiderman_rvb_player_interface::unk_378() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9AF050)(this);
}

// slot 379
i32 spiderman_rvb_player_interface::unk_379() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7219A0)(this);
}

// slot 380
i32 spiderman_rvb_player_interface::unk_380(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_7219B0)(this, unk);
}

// slot 381
f32 spiderman_rvb_player_interface::unk_381(void* unk, i32 unk_2, f32 unk_3) {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*, void*, i32, f32))retail::sub_9AD940)(this, unk, unk_2, unk_3);
}

// slot 382
bool spiderman_rvb_player_interface::unk_382(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9BEFC0)(this, unk);
}

// slot 383
i32 spiderman_rvb_player_interface::unk_383() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721580)(this);
}

// slot 384
bool spiderman_rvb_player_interface::unk_384() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9B0170)(this);
}

// slot 385
i32 spiderman_rvb_player_interface::unk_385() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721580)(this);
}

// slot 386
bool spiderman_rvb_player_interface::unk_386(f32 unk, bool unk_2) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, f32, bool))retail::sub_9B0280)(this, unk, unk_2);
}

// slot 387
void* spiderman_rvb_player_interface::unk_387() {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721590)(this);
}

// slot 388
bool spiderman_rvb_player_interface::unk_388() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7215A0)(this);
}

// slot 389
bool spiderman_rvb_player_interface::unk_389(void* unk, f32 unk_2) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, void*, f32))retail::sub_9BE960)(this, unk, unk_2);
}

// slot 390
bool spiderman_rvb_player_interface::unk_390() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9BC460)(this);
}

// slot 391
bool spiderman_rvb_player_interface::unk_391(i32 unk, i32 unk_2) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32, i32))retail::sub_9B03A0)(this, unk, unk_2);
}

// sub_9B0510
bool spiderman_rvb_player_interface::unk_392(bool skip) {
    if (skip)
        return true;

    if (get_command_button(plr_cmd_pole_swing_attach)->pressed() && !is_carrying_or_carried())
        unk_057c = 0.25f;

    if (unk_057c > 0.0f)
        return unk_386(1.0f, true);

    return false;
}

// slot 393
bool spiderman_rvb_player_interface::unk_393(f32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9B0590)(this, unk);
}

// slot 394
i32 spiderman_rvb_player_interface::unk_394() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A4780)(this);
}

// slot 395
i32 spiderman_rvb_player_interface::unk_395() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7215E0)(this);
}

// slot 396
i32 spiderman_rvb_player_interface::unk_396(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_7215F0)(this, unk);
}

// slot 398
i32 spiderman_rvb_player_interface::unk_398(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A60A0)(this, unk);
}

// slot 399
swing_data* spiderman_rvb_player_interface::get_swinger(e_swinger_hand i) {
    return ((swing_data* (__thiscall*)(spiderman_rvb_player_interface*, e_swinger_hand))retail::sub_721600)(this, i);
}

// slot 400
void spiderman_rvb_player_interface::unk_400(polytube* tube, f32 radius) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, polytube*, f32))retail::sub_9A6030)(this, tube, radius);
}

// sub_9A6050
i32 spiderman_rvb_player_interface::get_als_swing_hand_value() {
    bool r_hand = get_swinger(swinger_right)->status != swinger_inactive;
    bool l_hand = get_swinger(swinger_left)->status != swinger_inactive;

    if (r_hand)
        return l_hand ? 2 : 0;

    return l_hand ? 1 : -1;
}

// slot 402
i32 spiderman_rvb_player_interface::unk_402(i32 unk, i32 unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32, i32))retail::sub_9A4D30)(this, unk, unk_2);
}

// slot 403
f32 spiderman_rvb_player_interface::unk_403() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A4EA0)(this);
}

// sub_9A4C10
f32 spiderman_rvb_player_interface::unk_404(f32 progress) {
    if (0.0f > progress)
        progress = (f32)retail::sub_9216D0((u32*)references::upgrade_manager.read());

    return (f32)((1.0f - progress) * 10.0) + (f32)(progress * 20.0);
}

// sub_9A4C60
f32 spiderman_rvb_player_interface::unk_405(f32 progress) {
    if (0.0f > progress)
        progress = (f32)retail::sub_9216D0((u32*)references::upgrade_manager.read());

    return (1.0f - progress) + progress;
}

// sub_9A4CA0
f32 spiderman_rvb_player_interface::unk_406(f32 progress) {
    if (0.0f > progress)
        progress = (f32)retail::sub_9216D0((u32*)references::upgrade_manager.read());

    return (f32)((1.0f - progress) * 0.5) + (f32)(progress * 1.5);
}

// sub_9A4CF0
f32 spiderman_rvb_player_interface::unk_407(f32 progress) {
    if (0.0f > progress)
        progress = (f32)retail::sub_9216D0((u32*)references::upgrade_manager.read());

    f32 remaining = 1.0f - progress;

    return remaining + remaining + progress;
}

// slot 408
void spiderman_rvb_player_interface::unk_408(i32 unk, i32 unk_2) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32, i32))retail::sub_9B0A30)(this, unk, unk_2);
}

// slot 409
void spiderman_rvb_player_interface::unk_409(i32 unk, i32 unk_2, i32 unk_3) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32, i32, i32))retail::sub_9B0D00)(this, unk, unk_2, unk_3);
}

// slot 410
void spiderman_rvb_player_interface::unk_410(i32 unk, i32 unk_2, i32 unk_3, i32 unk_4) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32, i32, i32, i32))retail::sub_9B0E60)(this, unk, unk_2, unk_3, unk_4);
}

// slot 411
i32 spiderman_rvb_player_interface::unk_411(i32 unk, i32 unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32, i32))retail::sub_9A6110)(this, unk, unk_2);
}

// slot 412
void spiderman_rvb_player_interface::unk_412(i32 unk, i32 unk_2, i32 unk_3, i32 unk_4, i32 unk_5, i32 unk_6) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32, i32, i32, i32, i32, i32))retail::sub_9B1100)(this, unk, unk_2, unk_3, unk_4, unk_5, unk_6);
}

// slot 413
i32 spiderman_rvb_player_interface::unk_413(i32 unk, i32 unk_2, void* unk_3) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32, i32, void*))retail::sub_9A6180)(this, unk, unk_2, unk_3);
}

// slot 414
f32 spiderman_rvb_player_interface::unk_414() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A67D0)(this);
}

// slot 415
f32 spiderman_rvb_player_interface::unk_415() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A67E0)(this);
}

// slot 416
f32 spiderman_rvb_player_interface::unk_416() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A67F0)(this);
}

// slot 417
i32 spiderman_rvb_player_interface::unk_417() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6440)(this);
}

// slot 418
i32 spiderman_rvb_player_interface::unk_418() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721610)(this);
}

// slot 419
i32 spiderman_rvb_player_interface::unk_419() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721620)(this);
}

// slot 420
i32 spiderman_rvb_player_interface::unk_420() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721630)(this);
}

// slot 421
i32 spiderman_rvb_player_interface::unk_421() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721640)(this);
}

// slot 422
i32 spiderman_rvb_player_interface::unk_422() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721650)(this);
}

// slot 423
i32 spiderman_rvb_player_interface::unk_423() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721660)(this);
}

// slot 425
f32 spiderman_rvb_player_interface::get_als_swing_angle() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A69A0)(this);
}

// slot 427
f32 spiderman_rvb_player_interface::get_als_swing_side_angle() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A69B0)(this);
}

// slot 429
f32 spiderman_rvb_player_interface::get_als_swing_to_anchor_angle() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A69C0)(this);
}

// slot 432
f32 spiderman_rvb_player_interface::unk_432() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A69D0)(this);
}

// slot 433
f32 spiderman_rvb_player_interface::unk_433() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A69E0)(this);
}

// slot 434
f32 spiderman_rvb_player_interface::unk_434() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A4770)(this);
}

// slot 438
f32 spiderman_rvb_player_interface::unk_438() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7218F0)(this);
}

// slot 439
void spiderman_rvb_player_interface::unk_439(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721900)(this, unk);
}

// slot 440
f32 spiderman_rvb_player_interface::unk_440() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6800)(this);
}

// slot 441
f32 spiderman_rvb_player_interface::unk_441() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6810)(this);
}

// slot 442
f32 spiderman_rvb_player_interface::unk_442() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6820)(this);
}

// slot 443
f32 spiderman_rvb_player_interface::unk_443() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6830)(this);
}

// slot 444
f32 spiderman_rvb_player_interface::unk_444() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721670)(this);
}

// slot 445
f32 spiderman_rvb_player_interface::unk_445() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6860)(this);
}

// slot 446
f32 spiderman_rvb_player_interface::unk_446() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6890)(this);
}

// slot 447
f32 spiderman_rvb_player_interface::unk_447() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A68C0)(this);
}

// slot 448
f32 spiderman_rvb_player_interface::unk_448() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A68F0)(this);
}

// slot 449
f32 spiderman_rvb_player_interface::unk_449() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6900)(this);
}

// slot 450
f32 spiderman_rvb_player_interface::unk_450() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6910)(this);
}

// slot 451
f32 spiderman_rvb_player_interface::unk_451() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6920)(this);
}

// slot 452
f32 spiderman_rvb_player_interface::unk_452() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A69F0)(this);
}

// slot 453
i32 spiderman_rvb_player_interface::unk_453(i32 unk, i32 unk_2, i32 unk_3) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32, i32, i32))retail::sub_9A6A00)(this, unk, unk_2, unk_3);
}

// slot 454
f32 spiderman_rvb_player_interface::unk_454() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6930)(this);
}

// slot 455
i32 spiderman_rvb_player_interface::unk_455(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6940)(this, unk);
}

// slot 456
bool spiderman_rvb_player_interface::unk_456() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A6960)(this);
}

// slot 457
bool spiderman_rvb_player_interface::unk_457(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A6980)(this, unk);
}

// slot 458
void spiderman_rvb_player_interface::unk_458(void* unk, void* unk_2, f32 unk_3) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, void*, void*, f32))retail::sub_9A6C80)(this, unk, unk_2, unk_3);
}

// slot 459
i32 spiderman_rvb_player_interface::unk_459() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721680)(this);
}

// slot 460
i32 spiderman_rvb_player_interface::unk_460(f32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9B5140)(this, unk);
}

// sub_7210F0
bool spiderman_rvb_player_interface::should_do_strength_test() const {
    return my_strength_test_info.should_do_strength_test();
}

// sub_721110
bool spiderman_rvb_player_interface::is_a_scripted_strength_test() const {
    string_hash none;
    none.initialize(mash::ALLOCATED);

    return !(my_strength_test_info.get_category_name() == none);
}

// sub_721100
bool spiderman_rvb_player_interface::unk_463() {
    return my_strength_test_info.unk_0c;
}

// slot 464
i32 spiderman_rvb_player_interface::unk_464() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721150)(this);
}

// slot 465
void spiderman_rvb_player_interface::unk_465() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721160)(this);
}

// slot 466
bool spiderman_rvb_player_interface::unk_466() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721170)(this);
}

// slot 467
void* spiderman_rvb_player_interface::unk_467() {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721180)(this);
}

// slot 468
void spiderman_rvb_player_interface::unk_468() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721190)(this);
}

// slot 469
bool spiderman_rvb_player_interface::unk_469() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7211A0)(this);
}

// slot 470
bool spiderman_rvb_player_interface::unk_470() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7211B0)(this);
}

// slot 471
i32 spiderman_rvb_player_interface::unk_471() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7211C0)(this);
}

// slot 472
void spiderman_rvb_player_interface::unk_472() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7211D0)(this);
}

// sub_9B76B0
arch_base_vhandle spiderman_rvb_player_interface::unk_483(const plr_combat_attack* attack) const {
    if (unk_048().value)
        return unk_048();

    return player_interface::unk_483(attack);
}

// sub_9A75B0
bool spiderman_rvb_player_interface::unk_484() {
    auto* als_node = (als_inode*)my_actor->get_ai_core()->get_info_node(info_node_type_als);

    return als_node->get_optional_als_pb_int(references::wall_explode_recovery.get(), 0, als::base_layer, nullptr) > 0;
}

// sub_9A4200
bool spiderman_rvb_player_interface::unk_485() {
    if (unk_0b54 > 0.0f)
        return false;

    return unk_0b5c;
}

// sub_9A4220
bool spiderman_rvb_player_interface::unk_486(bool unk, f32 delay) {
    unk_0b54 = delay;
    unk_0b5c = unk;

    return unk_0b5c;
}

// sub_9A4240
bool spiderman_rvb_player_interface::unk_487() {
    if (unk_0b58 > 0.0f)
        return false;

    return unk_0b5d;
}

// sub_9A4260
bool spiderman_rvb_player_interface::unk_488(bool unk, f32 delay) {
    unk_0b58 = delay;
    unk_0b5d = unk;

    return unk_0b5d;
}

// sub_9A6DE0
i32 spiderman_rvb_player_interface::unk_489() {
    return unk_07e4 > 0.0f && (f64)unk_07e4 >= (f64)0.97f;
}

// sub_9A6E10
i32 spiderman_rvb_player_interface::unk_490() {
    return unk_07e4 > 0.0f;
}

// sub_9A6DB0
f32 spiderman_rvb_player_interface::unk_491() {
    if (0.0f > unk_07e4)
        return 0.0f;

    if (unk_07e4 > 1.0f)
        return 1.0f;

    return unk_07e4;
}

// sub_9A6D80
void spiderman_rvb_player_interface::unk_492(bool start) {
    unk_07e4 = start ? 1.0f : 0.0f;
}

// sub_9A6EA0
i32 spiderman_rvb_player_interface::unk_493() {
    return unk_07e8 > 0.0f && unk_07e8 >= 0.0030000005f;
}

// sub_9A6ED0
i32 spiderman_rvb_player_interface::unk_494() {
    return unk_07e8 > 0.0f;
}

// sub_9A6EF0
i32 spiderman_rvb_player_interface::is_in_webcast_approach_normal_hit_range() {
    return unk_07c8 == 1;
}

// sub_9A6F00
i32 spiderman_rvb_player_interface::is_in_webcast_approach_critical_hit_range() {
    return unk_07c8 == 2;
}

// sub_9A6F10
i32 spiderman_rvb_player_interface::is_in_approach_counter() {
    return unk_07cc == 2;
}

// sub_9A6F20
i32 spiderman_rvb_player_interface::is_in_webcast_approach_early_range() {
    return unk_07c8 == 0;
}

// sub_9A6F30
i32 spiderman_rvb_player_interface::is_in_webcast_approach_counter_range() {
    return unk_07c8 == 3;
}

// sub_9A6F40
i32 spiderman_rvb_player_interface::unk_500() {
    return unk_07b4;
}

// sub_9A6F60
i32 spiderman_rvb_player_interface::unk_501() {
    return unk_07b8;
}

// sub_721730
f32 spiderman_rvb_player_interface::approach_norm_to_crit_percent() {
    return unk_07e0;
}

// sub_721740
i32 spiderman_rvb_player_interface::unk_503() {
    return unk_07cc;
}

// sub_9B7670
bool spiderman_rvb_player_interface::is_webcasting_entity(arch_base_vhandle entity) {
    arch_base_vhandle target = unk_061();

    return (unk_07b0 || unk_07b4) && entity == target;
}

// sub_9A6E30
void spiderman_rvb_player_interface::unk_506(bool start) {
    unk_07e8 = start ? 0.033f : 0.0f;
}

// sub_721770
const vector3d* spiderman_rvb_player_interface::unk_507(const vector3d* v) {
    unk_0804 = *v;

    return v;
}

// sub_721750
vector3d spiderman_rvb_player_interface::unk_508() const {
    return unk_0804;
}

// sub_7217A0
i32 spiderman_rvb_player_interface::unk_509() {
    return unk_0800 > 0.0f;
}

// sub_9A6D50
void spiderman_rvb_player_interface::unk_510(bool start) {
    unk_0800 = start ? 0.75f : 0.0f;
}

// sub_7217C0
bool spiderman_rvb_player_interface::unk_511() {
    return unk_07fc;
}

// sub_7217D0
bool spiderman_rvb_player_interface::unk_512(bool unk) {
    unk_07fc = unk;

    return unk;
}

// slot 513
bool spiderman_rvb_player_interface::unk_513(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9BEF90)(this, unk);
}

// slot 514
void* spiderman_rvb_player_interface::unk_514(i32 unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9C2C00)(this, unk);
}

// sub_9ADA50
bool spiderman_rvb_player_interface::is_in_wallmode() {
    if (mode.current == 2 || mode.current == 7)
        return true;

    if (mode.current == 9 && unk_485())
        return true;

    return mode.current == 17;
}

// slot 517
i32 spiderman_rvb_player_interface::unk_517() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9AA4D0)(this);
}

// slot 518
bool spiderman_rvb_player_interface::unk_518(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_721390)(this, unk);
}

// slot 519
bool spiderman_rvb_player_interface::unk_519() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7213A0)(this);
}

// slot 520
bool spiderman_rvb_player_interface::unk_520() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9BDA20)(this);
}

// slot 522
i32 spiderman_rvb_player_interface::unk_522() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7213C0)(this);
}

// sub_9A5130
void spiderman_rvb_player_interface::lock_suit(bool lock, bool black) {
    if (rvb_black_mode != black && lock) {
        if (suit_locked)
            suit_locked = false;

        unk_573();
    }

    suit_locked = lock;
}

// slot 524
bool spiderman_rvb_player_interface::unk_524() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7213D0)(this);
}

// sub_9A7E90
void spiderman_rvb_player_interface::mode_lock_suit(bool lock, f32 time) {
    suit_mode_locked = true;
    unk_0538         = lock;
    unk_0534         = mode.current;
    unk_053c         = 0.0f;
    unk_0540         = 0.0f;

    if (lock)
        unk_053c = time;

    if (unk_0539 && lock)
        unk_0540 = 0.0f > unk_053c ? 1.0f : unk_053c;
}

// sub_9A7F00
void spiderman_rvb_player_interface::unk_526() {
    suit_mode_locked = false;
    unk_0534         = 0;

    if (!suit_locked && unk_0538 && unk_0540 > 0.0f && !unk_0539)
        unk_573();

    unk_0538 = false;
}

// sub_9A7F50
void spiderman_rvb_player_interface::unk_527(f32 delta_t) { // the suit mode lock's frame
    unk_0539 = false;

    if (suit_locked || !suit_mode_locked || !unk_0538)
        return;

    f32 time = unk_053c;

    if (time >= 0.0f && unk_0540 > 0.0f)
        unk_0540 = (f32)((f64)unk_0540 - (f64)delta_t);

    if (get_command_button(plr_cmd_redsuit)->triggered()) {
        unk_0539 = true;
        unk_0540 = 0.0f > time ? 1.0f : time;
    }

    if (mode.current != unk_0534)
        unk_526();
}

// slot 528
line_info* spiderman_rvb_player_interface::unk_528() {
    return ((line_info* (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7218E0)(this);
}

// sub_9B1250
void spiderman_rvb_player_interface::unk_529(const line_info* line) {
    if (line) {
        if (!(unk_0ba8.hit_entity == line->hit_entity))
            unk_0c04 = 0.0f;

        unk_0ba8 = *line;
    } else {
        unk_0ba8.clear();
        unk_0c04 = 0.0f;
    }
}

// sub_9B12A0
bool spiderman_rvb_player_interface::unk_530(f32 delta_t) {
    arch_base_vhandle &hit = unk_0ba8.hit_entity;

    if (hit.resolve() && hit.resolve())
        hit = unk_273((entity*)hit.resolve())->my_handle;

    if (!unk_531(nullptr) && !unk_532(nullptr))
        return false;

    unk_0c04 = (f32)((f64)delta_t + (f64)unk_0c04);

    if (unk_0c04 >= 0.75f)
        return true;

    game_button* hop = get_command_button(plr_cmd_parkour);

    return hop && hop->pressed();
}

// sub_9B1360
bool spiderman_rvb_player_interface::unk_531(const line_info* line) {
    line_info copy;
    copy = line ? *line : unk_0ba8;

    arch_base_vhandle &hit = copy.hit_entity;

    bool vehicle = false;

    if (hit.resolve() && hit.resolve()) {
        if (hit.resolve()->is_a_vehicle())
            vehicle = true;
        else if (hit.resolve()->is_an_actor()) {
            if (ngl::mesh* mesh = ((entity*)hit.resolve())->unk_111()) {
                mash::string name(mesh->name.get_text());

                vehicle = name.find("vehicle") != mash::string::npos;
            }
        }
    }

    retail::sub_798F10((u8*)&copy); // line_info::~line_info

    return vehicle;
}

// slot 532
bool spiderman_rvb_player_interface::unk_532(const line_info* unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, const line_info*))retail::sub_9B14B0)(this, unk);
}

// sub_9A94B0
void spiderman_rvb_player_interface::unk_534(arch_base_vhandle target, f32 time) {
    if (!target.value || !target.resolve() || !(time > 0.0f))
        return;

    if (unk_0c10.value && unk_0c10.resolve()) {
        if (collision_geometry* colgeom = ((entity_base*)unk_0c10.resolve())->get_colgeom())
            colgeom->flags |= fi_collide_camera;
    }

    unk_0c10 = target;

    if (collision_geometry* colgeom = ((entity_base*)target.resolve())->get_colgeom())
        colgeom->flags &= ~fi_collide_camera;

    unk_0c14 = time;
}

// slot 536
bool spiderman_rvb_player_interface::unk_536(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9B3120)(this, unk);
}

// slot 537
bool spiderman_rvb_player_interface::unk_537(void* unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_9B1F60)(this, unk);
}

// slot 538
bool spiderman_rvb_player_interface::unk_538(i32 unk) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9B1760)(this, unk);
}

// slot 539
f32 spiderman_rvb_player_interface::unk_539() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_720E80)(this);
}

// sub_9A5230
void spiderman_rvb_player_interface::unk_540(f32 delta_t) {
    if (delta_t > 0.0f) {
        if (unk_0c58 > delta_t)
            unk_0c58 = (f32)((f64)unk_0c58 - (f64)delta_t);
        else
            unk_0c58 = 0.0f;
    }
}

// slot 541
f32 spiderman_rvb_player_interface::unk_541(i32 unk) {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_720F60)(this, unk);
}

// sub_720F90
vector3d spiderman_rvb_player_interface::unk_542(i32 index) {
    if (index < 4)
        return unk_0c70[index];

    return references::unk_0102f9e0.get();
}

// slot 543
bool spiderman_rvb_player_interface::unk_543() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9C08F0)(this);
}

// slot 544
i32 spiderman_rvb_player_interface::unk_544(f32 delta_t) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A5DF0)(this, delta_t);
}

// slot 545
bool spiderman_rvb_player_interface::unk_545() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9C0270)(this);
}

// slot 546
i32 spiderman_rvb_player_interface::unk_546(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9A5DD0)(this, unk);
}

// slot 547
bool spiderman_rvb_player_interface::unk_547() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9C0510)(this);
}

// sub_9A5D00
f32 spiderman_rvb_player_interface::unk_548(f32 t) {
    f32 lo;
    f32 hi;
    f32 below;
    f32 above;

    if (unk_06a4.unk_04 == 1) {
        hi    = 0.75f;
        lo    = 0.25f;
        above = 1.25f;
        below = 2.5f;
    } else {
        hi    = 0.6f;
        lo    = 0.3f;
        above = 1.5f;
        below = 15.0f;
    }

    if (t > hi)
        return above;

    if (lo > t)
        return below;

    f32 fraction = (f32)(((f64)t - (f64)lo) / ((f64)hi - (f64)lo));

    return (f32)((f64)fraction * ((f64)above - (f64)below) + (f64)below);
}

// sub_9A5C00
f32 spiderman_rvb_player_interface::unk_549(f32 t) {
    f32 lo;
    f32 hi;
    f32 below;
    f32 above;

    switch (unk_06a4.unk_04) {
        case 1:
            below = 30.0f;
            lo    = 0.25f;
            hi    = 0.75f;
            above = below;

            break;

        case 2:
            below = 20.0f;
            lo    = 0.25f;
            hi    = 0.75f;
            above = below;

            break;

        case 3:
            hi    = 1.0f;
            lo    = 0.5f;
            above = 120.0f;
            below = 30.0f;

            break;

        default:
            hi    = 0.6f;
            lo    = 0.3f;
            below = 45.0f;
            above = below;

            break;
    }

    if (t > hi)
        return above;

    if (lo > t)
        return below;

    f32 fraction = (f32)(((f64)t - (f64)lo) / ((f64)hi - (f64)lo));

    return (f32)((f64)fraction * ((f64)above - (f64)below) + (f64)below);
}

// sub_9B7A50
void spiderman_rvb_player_interface::unk_550(arch_base_vhandle* out, i32 count) {
    i32 index = 0;

    if (unk_0724.value) {
        if (count > 0)
            out[0] = unk_0724;

        index = 1;
    }

    for (i32 other = 0; index < count && other < 2; ++index, ++other)
        out[index] = unk_072c[other];

    for (; index < count; ++index)
        out[index] = arch_base_vhandle();
}

// slot 551
void* spiderman_rvb_player_interface::unk_551(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_9A7040)(this, unk);
}

// slot 552
i32 spiderman_rvb_player_interface::unk_552(void* unk, i32 unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, void*, i32))retail::sub_9B7AE0)(this, unk, unk_2);
}

// slot 553
void spiderman_rvb_player_interface::unk_553(arch_base_vhandle old_target, arch_base_vhandle new_target, arch_base_vhandle other_target) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, arch_base_vhandle, arch_base_vhandle, arch_base_vhandle))retail::sub_9B7700)(this,
                                                                                                                                       old_target,
                                                                                                                                       new_target,
                                                                                                                                       other_target);
}

// slot 554
f32 spiderman_rvb_player_interface::unk_554() {
    return ((f32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_721210)(this);
}

// slot 555
void spiderman_rvb_player_interface::unk_555(f32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9B8560)(this, unk);
}

// slot 556
void* spiderman_rvb_player_interface::unk_556(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_9A70C0)(this, unk);
}

// slot 557
void* spiderman_rvb_player_interface::unk_557(i32 unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9C1530)(this, unk);
}

// slot 558
void spiderman_rvb_player_interface::unk_558(f32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9B8610)(this, unk);
}

// sub_9B7E60
void spiderman_rvb_player_interface::unk_559(f32 delta_t) {
    if (!unk_055().value)
        return;

    unk_06ec = (f32)((f64)unk_06ec - (f64)delta_t);

    if (unk_06ec <= 0.0f)
        unk_054(arch_base_vhandle(), 4);
    else
        unk_4b0 = unk_055();
}

// slot 560
void* spiderman_rvb_player_interface::unk_560(f32 unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9B7EF0)(this, unk);
}

// sub_9A71F0
i32 spiderman_rvb_player_interface::unk_561(game_button* button, i32* state) {
    if (button->pressed()) {
        if (*state == 1 || *state == 3) {
            *state = 2;

            return 2;
        }

        return 0;
    }

    if (*state == 0) {
        *state = 1;

        return 1;
    }

    if (*state == 2)
        *state = 3;

    return 0;
}

// sub_9B9440
f32 spiderman_rvb_player_interface::unk_562(arch_base_vhandle target) {
    f32 distance = FLT_MAX;

    if (!target.value || !target.resolve())
        return distance;

    auto* target_entity = (entity*)target.resolve();

    vector3d member_position = target_entity->my_abs_po->get_position();
    bool     has_member      = false;

    if (target_entity->is_a_conglomerate() && target_entity->has_ifc(entity_ifc_damage)) {
        string_hash webcast_key;
        ((string_hash* (__thiscall*)(damage_interface*, string_hash*))retail::sub_408F90)(target_entity->damage_ifc(), &webcast_key); // the damage interface's webcast key

        vector3d found;

        if (retail::sub_62FA60((i32)target_entity, ((conglomerate*)target_entity)->get_member_index(webcast_key), (f32*)&found, 0)) { // conglomerate::get_member_abs_position
            member_position = found;
            has_member      = true;
        }
    }

    vector3d owner_position = my_actor->my_abs_po->get_position();
    vector3d closest;

    if (has_member)
        retail::sub_4366E0((i32)&closest, (u32*)target_entity, (i32)&owner_position, (i32)&member_position); // the closest point to the segment
    else
        retail::sub_439BA0((i32)&closest, (u32*)target_entity, (u32*)my_actor); // the closest point to the owner

    f32 x = (f32)((f64)closest.x - (f64)owner_position.x);
    f32 y = (f32)((f64)closest.y - (f64)owner_position.y);
    f32 z = (f32)((f64)closest.z - (f64)owner_position.z);

    distance = (f32)std::sqrt((f64)(f32)((f64)x * (f64)x + (f64)y * (f64)y + (f64)z * (f64)z));

    return distance;
}

// sub_9A7250
f32 spiderman_rvb_player_interface::unk_563(f32 level, f32 target_level) {
    f32 difference = (f32)((f64)level - (f64)target_level);
    f32 factor     = 0.0f;

    if (difference < -4.5f || (f64)difference < -3.5)
        factor = 1.0f;
    else if (difference < -2.5f)
        factor = 0.7f;
    else if (difference < -1.5f)
        factor = 0.5f;
    else if (difference < -0.5f)
        factor = 0.4f;
    else if (difference < 0.5f)
        factor = 0.3f;
    else if (difference < 1.5f)
        factor = 0.25f;
    else if (difference < 2.5f)
        factor = 0.2f;
    else if ((f64)difference < 3.5)
        factor = 0.1f;
    else if (difference < 4.5f)
        factor = 0.0f;

    return factor;
}

// slot 564
i32 spiderman_rvb_player_interface::unk_564(f32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9B9610)(this, unk);
}

// sub_9C2810
void spiderman_rvb_player_interface::unk_565(f32 delta_t) {
    unk_564(delta_t);

    unk_07e8 = (f32)((f64)unk_07e8 - (f64)delta_t);

    if (0.0f > unk_07e8)
        unk_07e8 = 0.0f;

    unk_0800 = (f32)((f64)unk_0800 - (f64)delta_t);

    if (0.0f > unk_0800)
        unk_0800 = 0.0f;

    if (unk_07e4 > 0.0f) {
        unk_07e4 = (f32)((f64)unk_07e4 - (f64)delta_t);

        if (0.0f > unk_07e4) {
            unk_07e4 = 0.0f;
            unk_0818->clear();
        }
    }

    unk_569();
}

// slot 566
void spiderman_rvb_player_interface::unk_566(i32 unk) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_9C1BE0)(this, unk);
}

// slot 567
void* spiderman_rvb_player_interface::unk_567(f32 unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9C20C0)(this, unk);
}

// slot 568
void spiderman_rvb_player_interface::unk_568() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9B8C10)(this);
}

// slot 569
void spiderman_rvb_player_interface::unk_569() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_5B4A70)(this);
}

// slot 570
bool spiderman_rvb_player_interface::unk_570(i32 unk, void* unk_2, i32 unk_3, i32 unk_4, f32 unk_5) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32, void*, i32, i32, f32))retail::sub_9B89E0)(this, unk, unk_2, unk_3, unk_4, unk_5);
}

// slot 571
i32 spiderman_rvb_player_interface::unk_571(i32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32))retail::sub_7213B0)(this, unk);
}

// slot 572
void spiderman_rvb_player_interface::unk_572(f32 unk, bool black) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, f32, bool))retail::sub_9A51A0)(this, unk, black);
}

// slot 573
i32 spiderman_rvb_player_interface::unk_573() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A8010)(this);
}

// slot 574
bool spiderman_rvb_player_interface::unk_574() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_7214E0)(this);
}

// slot 575
bool spiderman_rvb_player_interface::unk_575() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9ADA90)(this);
}

// slot 576
bool spiderman_rvb_player_interface::unk_576() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9AED60)(this);
}

// slot 577
i32 spiderman_rvb_player_interface::unk_577() {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9AE460)(this);
}

// slot 578
bool spiderman_rvb_player_interface::unk_578() {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9B06F0)(this);
}

// slot 579
i32 spiderman_rvb_player_interface::unk_579(void* unk, void* unk_2, void* unk_3, i32 unk_4, i32 unk_5) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, void*, void*, void*, i32, i32))retail::sub_9ABA60)(this, unk, unk_2, unk_3, unk_4, unk_5);
}

// sub_9A5F60
void spiderman_rvb_player_interface::unk_580(f32 delta_t) {
    for (i32 index = 0; index < max_swingers; ++index) {
        swing_data* swinger = get_swinger((e_swinger_hand)index);

        if (swinger->status != swinger_inactive) {
            unk_368(delta_t, swinger);

            return;
        }
    }
}

// slot 581
bool spiderman_rvb_player_interface::unk_581(i32 unk, i32 unk_2) {
    return ((bool (__thiscall*)(spiderman_rvb_player_interface*, i32, i32))retail::sub_9AD0E0)(this, unk, unk_2);
}

// slot 582
i32 spiderman_rvb_player_interface::unk_582(f32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9A8310)(this, unk);
}

// sub_9A6F50
i32 spiderman_rvb_player_interface::is_in_webcast_approach() {
    return unk_07b0;
}

// sub_9A6E60
f32 spiderman_rvb_player_interface::unk_584() {
    f32 fraction = (f32)((f64)unk_07e8 / (f64)0.033f);

    if (0.0f > fraction)
        return 0.0f;

    if (fraction > 1.0f)
        return 1.0f;

    return fraction;
}

// slot 585
void spiderman_rvb_player_interface::unk_585() {
    ((void (__thiscall*)(spiderman_rvb_player_interface*))retail::sub_9A53C0)(this);
}

// slot 586
void* spiderman_rvb_player_interface::unk_586(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_9AA140)(this, unk);
}

// slot 587
i32 spiderman_rvb_player_interface::unk_587(i32 unk, i32 unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32, i32))retail::sub_9AA110)(this, unk, unk_2);
}

// slot 588
i32 spiderman_rvb_player_interface::unk_588(void* unk, i32 unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, void*, i32))retail::sub_9A5450)(this, unk, unk_2);
}

// slot 589
i32 spiderman_rvb_player_interface::unk_589(i32 unk, f32 unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32, f32))retail::sub_9A54B0)(this, unk, unk_2);
}

// slot 590
void* spiderman_rvb_player_interface::unk_590(void* unk) {
    return ((void* (__thiscall*)(spiderman_rvb_player_interface*, void*))retail::sub_9AA1A0)(this, unk);
}

// slot 591
i32 spiderman_rvb_player_interface::unk_591(i32 unk, i32 unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, i32, i32))retail::sub_9AA170)(this, unk, unk_2);
}

// slot 592
i32 spiderman_rvb_player_interface::unk_592(void* unk, i32 unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, void*, i32))retail::sub_9A5790)(this, unk, unk_2);
}

// slot 593
void spiderman_rvb_player_interface::unk_593(void* unk, i32 unk_2, f32 unk_3) {
    ((void (__thiscall*)(spiderman_rvb_player_interface*, void*, i32, f32))retail::sub_9AA200)(this, unk, unk_2, unk_3);
}

// slot 594
i32 spiderman_rvb_player_interface::unk_594(void* unk, i32 unk_2) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, void*, i32))retail::sub_9A5A00)(this, unk, unk_2);
}

// slot 595
i32 spiderman_rvb_player_interface::unk_595(f32 unk) {
    return ((i32 (__thiscall*)(spiderman_rvb_player_interface*, f32))retail::sub_9BF060)(this, unk);
}

// sub_9882D0
f32 spiderman_rvb_player_interface::compute_ground_level_at_sweet_spot_position(const vector3d &position, f32 max_web_length, const region* reg) {
    line_info lif(position, position - vector3d(0.0f, max_web_length, 0.0f));

    f32 ground_level;

    if (retail::sub_798F50((char*)&lif, (i32)references::entfilter_entity_no_capsules.read(), (i32)references::obbfilter_lineseg_test.read(), nullptr, 0)) // line_info::check_collision
        ground_level = lif.hit_pos.y;
    else
        ground_level = ((f32 (__thiscall*)(const region*))retail::sub_77ECC0)(reg); // region::get_ground_level

    retail::sub_798F10((u8*)&lif); // line_info::~line_info

    return ground_level;
}
