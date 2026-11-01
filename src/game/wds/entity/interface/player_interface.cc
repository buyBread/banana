#include "retail.hh"
#include "game/wds/entity/interface/player_interface.hh"
#include "game/wds/ai/actions/pick_up_ai_action.hh"
#include "game/wds/ai/states/plr_combat_states.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/ai_state_machine.hh"
#include "game/wds/ai/ai_target.hh"
#include "game/wds/ai/ai_weapon_inode.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/entity/interface/damage_interface.hh"
#include "game/wds/entity/interface/manip_obj_interface.hh"
#include "game/wds/entity/interface/player_combo_counter.hh"
#include "game/wds/entity/interface/weapon_interface_extender.hh"
#include "game/wds/entity/actor.hh"
#include "game/als/als.hh"
#include "game/math/po.hh"

using namespace treyarch;

e_combat_move player_interface::previous_next_attack;

// slot 1
void player_interface::construct_mashed_class() {
    ((void (__thiscall*)(player_interface*))retail::sub_99A620)(this);
}

// slot 2
void player_interface::destruct_mashed_class() {
    ((void (__thiscall*)(player_interface*))retail::sub_99A920)(this);
}

// slot 3
void player_interface::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(player_interface*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_99ADB0)(this,
                                                                                                                   mash_info,
                                                                                                                   containing_class_ptr,
                                                                                                                   buffer);
}

// slot 4
mash::virtual_types_key player_interface::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const player_interface*))retail::sub_720160)(this);
}

// slot 6
bool player_interface::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const player_interface*, mash::virtual_types_key))retail::sub_643700)(this, parent_class);
}

// slot 8
i32 player_interface::get_mash_sizeof() const {
    return ((i32 (__thiscall*)(const player_interface*))retail::sub_720170)(this);
}

// slot 9
bool player_interface::get_ifc_num(const string_hash &name, f32* value, bool document) {
    return ((bool (__thiscall*)(player_interface*, const string_hash&, f32*, bool))retail::sub_9886C0)(this, name, value, document);
}

// slot 10
bool player_interface::set_ifc_num(const string_hash &name, f32 value, bool document) {
    return ((bool (__thiscall*)(player_interface*, const string_hash&, f32, bool))retail::sub_98C6E0)(this, name, value, document);
}

// slot 11
bool player_interface::get_ifc_vec(const string_hash &name, vector3d* value, bool document) {
    return ((bool (__thiscall*)(player_interface*, const string_hash&, vector3d*, bool))retail::sub_991760)(this, name, value, document);
}

// slot 16
e_entity_ifc player_interface::get_ifc_id() const {
    return ((e_entity_ifc (__thiscall*)(const player_interface*))retail::sub_A99FF0)(this);
}

// slot 18
void player_interface::set_owner(actor* owner) {
    ((void (__thiscall*)(player_interface*, actor*))retail::sub_996520)(this, owner);
}

// slot 19
bool player_interface::is_enabled() const {
    return ((bool (__thiscall*)(const player_interface*))retail::sub_71F800)(this);
}

// slot 20
void player_interface::set_enabled(bool value) {
    ((void (__thiscall*)(player_interface*, bool))retail::sub_71F810)(this, value);
}

// slot 21
f32 player_interface::unk_021() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720040)(this);
}

// slot 22
void player_interface::set_rpg_progress(f32 progress) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, progress);
}

// slot 23
bool player_interface::unk_023(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 24
void player_interface::unk_024(actor* owner) {
    ((void (__thiscall*)(player_interface*, actor*))retail::sub_689610)(this, owner);
}

// slot 25
void* player_interface::unk_025(f32 delta_t) {
    return ((void* (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, delta_t);
}

// slot 26
bool player_interface::unk_026() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 27
bool player_interface::unk_027(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 28
bool player_interface::unk_028(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 29
bool player_interface::unk_029() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 30
f32 player_interface::unk_030() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 31
i32 player_interface::unk_031(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71F830)(this, unk);
}

// slot 32
i32 player_interface::unk_032(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71F830)(this, unk);
}

// slot 33
void player_interface::unk_033(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 34
i32 player_interface::unk_034(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_9994F0)(this, unk);
}

// slot 35
i32 player_interface::unk_035(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 36
void* player_interface::unk_036(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_720180)(this, unk);
}

// slot 37
void player_interface::set_zip_override_anchor_num(f32 num) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, num);
}

// slot 38
f32 player_interface::unk_038() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 39
i32 player_interface::unk_039(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 40
arch_base_vhandle player_interface::unk_040() {
    arch_base_vhandle result;
    ((arch_base_vhandle* (__thiscall*)(player_interface*, arch_base_vhandle*))retail::sub_720180)(this, &result);

    return result;
}

// sub_992E00
arch_base_vhandle player_interface::unk_041(i32 index) {
    return arch_base_vhandle();
}

// slot 42
i32 player_interface::unk_042() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 43
void player_interface::unk_043(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 44
bool player_interface::unk_044(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 45
bool player_interface::unk_045() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_667F80)(this);
}

// slot 46
void player_interface::set_zlock_entity(arch_base_vhandle entity) {
    ((void (__thiscall*)(player_interface*, arch_base_vhandle))retail::sub_689610)(this, entity);
}

// slot 47
void player_interface::unk_047(u32 unk) {
    ((void (__thiscall*)(player_interface*, u32))retail::sub_689610)(this, unk);
}

// slot 48
arch_base_vhandle player_interface::unk_048() const {
    arch_base_vhandle result;
    ((arch_base_vhandle* (__thiscall*)(const player_interface*, arch_base_vhandle*))retail::sub_720180)(this, &result);

    return result;
}

// slot 49
bool player_interface::unk_049(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_71FF10)(this, unk);
}

// slot 50
i32 player_interface::unk_050(arch_base_vhandle target) {
    return ((i32 (__thiscall*)(player_interface*, arch_base_vhandle))retail::sub_689610)(this, target);
}

// slot 51
i32 player_interface::unk_051(i32 unk, i32 unk_2, i32 unk_3) {
    return ((i32 (__thiscall*)(player_interface*, i32, i32, i32))retail::sub_7AE090)(this, unk, unk_2, unk_3);
}

// slot 52
void* player_interface::unk_052(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_720180)(this, unk);
}

// slot 53
void* player_interface::unk_053(void* unk, void* unk_2) {
    return ((void* (__thiscall*)(player_interface*, void*, void*))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 54
void player_interface::unk_054(arch_base_vhandle target, u32 kind) {
    ((void (__thiscall*)(player_interface*, arch_base_vhandle, u32))retail::sub_AD2D40)(this, target, kind);
}

// slot 55
arch_base_vhandle player_interface::unk_055() {
    arch_base_vhandle result;
    ((arch_base_vhandle* (__thiscall*)(player_interface*, arch_base_vhandle*))retail::sub_720180)(this, &result);

    return result;
}

// slot 56
f32 player_interface::unk_056() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 57
i32 player_interface::unk_057() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 58
void player_interface::unk_058(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 59
void* player_interface::unk_059(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_720180)(this, unk);
}

// slot 60
i32 player_interface::unk_060(arch_base_vhandle target) {
    return ((i32 (__thiscall*)(player_interface*, arch_base_vhandle))retail::sub_689610)(this, target);
}

// slot 61
arch_base_vhandle player_interface::unk_061() {
    arch_base_vhandle result;
    ((arch_base_vhandle* (__thiscall*)(player_interface*, arch_base_vhandle*))retail::sub_720180)(this, &result);

    return result;
}

// slot 62
void* player_interface::unk_062(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_689610)(this, unk);
}

// slot 63
i32 player_interface::unk_063(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71F850)(this, unk);
}

// slot 64
i32 player_interface::unk_064(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 65
void* player_interface::unk_065(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_720180)(this, unk);
}

// slot 66
void player_interface::unk_066(i32 unk, i32 unk_2, i32 unk_3) {
    ((void (__thiscall*)(player_interface*, i32, i32, i32))retail::sub_7AE090)(this, unk, unk_2, unk_3);
}

// slot 67
i32 player_interface::unk_067(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71F830)(this, unk);
}

// slot 68
bool player_interface::unk_068(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 69
bool player_interface::unk_069() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 70
i32 player_interface::unk_070(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 71
void* player_interface::unk_071(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_720180)(this, unk);
}

// slot 72
bool player_interface::unk_072() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 73
bool player_interface::unk_073() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 74
bool player_interface::unk_074() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 75
void player_interface::set_desired_mode(i32 m) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_988640)(this, m);
}

// slot 76
i32 player_interface::unk_076() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71FB70)(this);
}

// slot 77
i32 player_interface::unk_077() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 78
void player_interface::set_swing_override_anchor(arch_base_vhandle anchor) {
    ((void (__thiscall*)(player_interface*, arch_base_vhandle))retail::sub_689610)(this, anchor);
}

// slot 79
void player_interface::set_zip_override_anchor(arch_base_vhandle anchor) {
    ((void (__thiscall*)(player_interface*, arch_base_vhandle))retail::sub_689610)(this, anchor);
}

// slot 80
arch_base_vhandle player_interface::unk_080() {
    arch_base_vhandle result;
    ((arch_base_vhandle* (__thiscall*)(player_interface*, arch_base_vhandle*))retail::sub_720180)(this, &result);

    return result;
}

// slot 81
void player_interface::unk_081(f32 unk) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, unk);
}

// slot 82
i32 player_interface::unk_082(f32 unk) {
    return ((i32 (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, unk);
}

// slot 83
i32 player_interface::unk_083() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_991CC0)(this);
}

// sub_991CD0
f32 player_interface::get_movement_speed() const {
    f32 speed = player_axis[0]->get_force();

    if (speed > 0.55f && command_buttons[plr_cmd_sprint].pressed())
        speed = 2.0f;

    return speed;
}

// slot 85
f32 player_interface::unk_085() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_986AE0)(this);
}

// slot 86
void player_interface::unk_086(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 87
bool player_interface::unk_087() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 88
bool player_interface::unk_088(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 89
bool player_interface::map_controls(i32 config, bool force) {
    return ((bool (__thiscall*)(player_interface*, i32, bool))retail::sub_991C00)(this, config, force);
}

// slot 90
void player_interface::render_phase() {
    ((void (__thiscall*)(player_interface*))retail::sub_986AA0)(this);
}

// slot 91
void player_interface::update_interface() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 92
void player_interface::frame_advance(f32 delta_t) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_9978B0)(this, delta_t);
}

// slot 93
void player_interface::frame_advance_disabled(f32 delta_t) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_992120)(this, delta_t);
}

// slot 94
void player_interface::unk_094(f32 delta_t) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, delta_t);
}

// slot 95
vector3d* player_interface::unk_095() {
    return ((vector3d* (__thiscall*)(player_interface*))retail::sub_720190)(this);
}

// slot 96
bool player_interface::unk_096(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_71FF10)(this, unk);
}

// slot 97
bool player_interface::unk_097() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 98
bool player_interface::unk_098(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 99
bool player_interface::is_walking() const {
    return ((bool (__thiscall*)(const player_interface*))retail::sub_71F870)(this);
}

// slot 100
i32 player_interface::unk_100() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 101
i32 player_interface::unk_101() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 102
bool player_interface::is_wallsprinting() const {
    return ((bool (__thiscall*)(const player_interface*))retail::sub_7201A0)(this);
}

// sub_7201B0
bool player_interface::is_crawl_jumping() {
    return get_mode() == 5 && (unk_161() == 4 || unk_161() == 5);
}

// slot 104
bool player_interface::unk_104() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_7201F0)(this);
}

// slot 105
bool player_interface::unk_105() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_720200)(this);
}

// slot 106
bool player_interface::unk_106() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_720210)(this);
}

// slot 107
i32 player_interface::unk_107() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 108
i32 player_interface::unk_108(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// sub_71F880
bool player_interface::is_transition() const {
    return transition.to_bool();
}

// sub_71F890
bool player_interface::unk_110() const {
    return unk_0439;
}

// sub_71F8A0
bool player_interface::unk_111() const {
    return unk_043a;
}

// sub_987580
void player_interface::set_transition(bool t, bool unk) {
    transition = t;
    unk_0439   = t && unk;
    unk_043a   = t && !unk;
}

// slot 113
bool player_interface::exterior_transition() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 114
bool player_interface::unk_114(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 115
bool player_interface::exterior_transition_180() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 116
bool player_interface::unk_116(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// sub_71F8B0
bool player_interface::interior_transition() {
    return crawling() && is_transition() && !exterior_transition() && !exterior_transition_180();
}

// slot 118
i32 player_interface::unk_118() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 119
bool player_interface::unk_119(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 120
bool player_interface::crawling() const {
    return ((bool (__thiscall*)(const player_interface*))retail::sub_720220)(this);
}

// slot 121
i32 player_interface::unk_121() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_720230)(this);
}

// slot 122
i32 player_interface::unk_122() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_720240)(this);
}

// slot 123
i32 player_interface::unk_123() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_720250)(this);
}

// slot 124
bool player_interface::unk_124() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 125
bool player_interface::swing_r_hand() const {
    return ((bool (__thiscall*)(const player_interface*))retail::sub_71F870)(this);
}

// slot 126
bool player_interface::swing_l_hand() const {
    return ((bool (__thiscall*)(const player_interface*))retail::sub_71F870)(this);
}

// sub_71F900
bool player_interface::swing_any_hand() {
    return swing_r_hand() || swing_l_hand();
}

// sub_71F930
bool player_interface::swing_both_hand() {
    return swing_r_hand() && swing_l_hand();
}

// slot 129
bool player_interface::swap_swing_hands(bool refire) {
    return ((bool (__thiscall*)(player_interface*, bool))retail::sub_689610)(this, refire);
}

// slot 130
bool player_interface::swing_r_hand_locked() const {
    return ((bool (__thiscall*)(const player_interface*))retail::sub_71F870)(this);
}

// slot 131
bool player_interface::swing_l_hand_locked() const {
    return ((bool (__thiscall*)(const player_interface*))retail::sub_71F870)(this);
}

// sub_71F960
bool player_interface::swing_any_hand_locked() {
    return swing_r_hand_locked() || swing_l_hand_locked();
}

// sub_71F990
bool player_interface::swing_both_hand_locked() {
    return swing_r_hand_locked() && swing_l_hand_locked();
}

// slot 134
void player_interface::lock_swing_r_hand(bool l) {
    ((void (__thiscall*)(player_interface*, bool))retail::sub_689610)(this, l);
}

// slot 135
void player_interface::lock_swing_l_hand(bool l) {
    ((void (__thiscall*)(player_interface*, bool))retail::sub_689610)(this, l);
}

// slot 136
i32 player_interface::unk_136() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 137
bool player_interface::on_ground() const {
    return ((bool (__thiscall*)(const player_interface*))retail::sub_667F80)(this);
}

// slot 138
bool player_interface::on_wall() const {
    return ((bool (__thiscall*)(const player_interface*))retail::sub_71F870)(this);
}

// slot 139
bool player_interface::on_ceiling() const {
    return ((bool (__thiscall*)(const player_interface*))retail::sub_71F870)(this);
}

// slot 140
void player_interface::unk_140() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 141
i32 player_interface::unk_141() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 142
bool player_interface::unk_142(bool unk) {
    return ((bool (__thiscall*)(player_interface*, bool))retail::sub_71F9C0)(this, unk);
}

// slot 143
bool player_interface::unk_143() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F9E0)(this);
}

// slot 144
void player_interface::unk_144() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 145
void player_interface::inc_jump_charge(f32 delta_t) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, delta_t);
}

// slot 146
void player_interface::set_abs_jump_charge(f32 fraction) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, fraction);
}

// slot 147
void player_interface::clear_jump_charge(bool store_for_als) {
    ((void (__thiscall*)(player_interface*, bool))retail::sub_689610)(this, store_for_als);
}

// slot 148
f32 player_interface::get_jump_charge() const {
    return ((f32 (__thiscall*)(const player_interface*))retail::sub_720050)(this);
}

// slot 149
f32 player_interface::unk_149() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 150
bool player_interface::unk_150() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 151
void player_interface::unk_151(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 152
bool player_interface::unk_152() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 153
bool player_interface::unk_153(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 154
i32 player_interface::unk_154() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F9F0)(this);
}

// slot 155
i32 player_interface::unk_155(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71FA00)(this, unk);
}

// slot 156
bool player_interface::unk_156() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 157
i32 player_interface::predict_jump_type() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 158
i32 player_interface::unk_158(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 159
bool player_interface::unk_159(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_992AC0)(this, unk);
}

// slot 160
player_jump_info* player_interface::get_jump_info(i32 jump) {
    return ((player_jump_info* (__thiscall*)(player_interface*, i32))retail::sub_992B50)(this, jump);
}

// slot 161
i32 player_interface::unk_161() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71FA10)(this);
}

// slot 162
i32 player_interface::unk_162(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71FA20)(this, unk);
}

// slot 163
void player_interface::unk_163() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 164
void player_interface::unk_164() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 165
i32 player_interface::unk_165() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 166
i32 player_interface::unk_166() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 167
bool player_interface::unk_167(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 168
bool player_interface::unk_168() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 169
bool player_interface::unk_169(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 170
bool player_interface::unk_170() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 171
bool player_interface::unk_171(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 172
bool player_interface::unk_172() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 173
bool player_interface::unk_173(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 174
bool player_interface::unk_174() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 175
bool player_interface::unk_175(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 176
bool player_interface::unk_176() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 177
bool player_interface::unk_177(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 178
bool player_interface::unk_178() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 179
bool player_interface::unk_179(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 180
bool player_interface::unk_180() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 181
i32 player_interface::unk_181(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_986AF0)(this, unk);
}

// slot 182
bool player_interface::unk_182() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71FA30)(this);
}

// slot 183
bool player_interface::unk_183(bool unk) {
    return ((bool (__thiscall*)(player_interface*, bool))retail::sub_71FA40)(this, unk);
}

// slot 184
bool player_interface::unk_184() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71FA60)(this);
}

// slot 185
bool player_interface::unk_185(bool unk) {
    return ((bool (__thiscall*)(player_interface*, bool))retail::sub_71FA70)(this, unk);
}

// slot 186
bool player_interface::unk_186() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71FA90)(this);
}

// slot 187
void player_interface::unk_187(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_71FAA0)(this, unk);
}

// slot 188
bool player_interface::unk_188() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71FAC0)(this);
}

// slot 189
bool player_interface::unk_189(bool unk) {
    return ((bool (__thiscall*)(player_interface*, bool))retail::sub_71FAD0)(this, unk);
}

// slot 190
bool player_interface::unk_190() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71FAF0)(this);
}

// slot 191
bool player_interface::unk_191(bool unk) {
    return ((bool (__thiscall*)(player_interface*, bool))retail::sub_71FB00)(this, unk);
}

// slot 192
bool player_interface::unk_192() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 193
bool player_interface::unk_193(bool unk) {
    return ((bool (__thiscall*)(player_interface*, bool))retail::sub_689610)(this, unk);
}

// slot 194
bool player_interface::handle_jump_charge(f32 delta_t) {
    return ((bool (__thiscall*)(player_interface*, f32))retail::sub_71FF10)(this, delta_t);
}

// slot 195
i32 player_interface::unk_195() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 196
vector3d player_interface::unk_196() {
    vector3d result;
    ((vector3d* (__thiscall*)(player_interface*, vector3d*))retail::sub_71F830)(this, &result);

    return result;
}

// slot 197
i32 player_interface::unk_197() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 198
void player_interface::unk_198() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 199
i32 player_interface::unk_199(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71F830)(this, unk);
}

// slot 200
void player_interface::unk_200(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 201
i32 player_interface::unk_201(f32 unk) {
    return ((i32 (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, unk);
}

// slot 202
i32 player_interface::unk_202() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 203
void player_interface::unk_203() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 204
vector3d player_interface::unk_204() {
    vector3d result;
    ((vector3d* (__thiscall*)(player_interface*, vector3d*))retail::sub_71F830)(this, &result);

    return result;
}

// slot 205
void* player_interface::unk_205(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_720180)(this, unk);
}

// slot 206
i32 player_interface::unk_206(f32 unk, entity* unk_2, i32 unk_3) {
    return ((i32 (__thiscall*)(player_interface*, f32, entity*, i32))retail::sub_7AE090)(this, unk, unk_2, unk_3);
}

// slot 207
void player_interface::unk_207() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 208
void player_interface::unk_208(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 209
void player_interface::unk_209(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 210
f32 player_interface::unk_210() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720010)(this);
}

// sub_986C40
void player_interface::force_attack(e_combat_move move, const vc8::vector<ai_target>* targs, i32 script_move) {
    clear_attacks(true);

    attacks[move]->forced_set_up(this, move, targs, script_move);
    next_attack = move;
}

// sub_986C00
void player_interface::force_attack(e_combat_move move, const ai_target* targ, i32 script_move) {
    clear_attacks(true);

    attacks[move]->forced_set_up(this, move, targ, script_move);
    next_attack = move;
}

// sub_71FB20
const plr_combat_attack* player_interface::get_attack(e_combat_move att) const {
    return (att >= 0 && att < combat_num_moves) ? attacks[att] : nullptr;
}

// sub_71FB40
bool player_interface::attack_possible(e_combat_move att) const {
    return (att >= 0 && att < combat_num_moves && attacks[att]) ? attacks[att]->has_potential_targets(this, true) : false;
}

// slot 215
i32 player_interface::unk_215() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71FB70)(this);
}

// sub_71FB80
e_combat_move player_interface::get_next_attack_enum() const {
    return next_attack;
}

// sub_71FB90
const plr_combat_attack* player_interface::get_next_attack() const {
    return get_attack(get_next_attack_enum());
}

// sub_99A4B0
void player_interface::init_attacks() {
    for (plr_combat_attack* &attack : attacks) {
        delete attack;
        attack = nullptr;
    }

    attacks[combat_move_script_move] = new plr_combat_attack();

    clear_attacks(true);
}

// sub_99A570
void player_interface::clear_attacks(bool all) {
    for (plr_combat_attack* attack : attacks)
        if (attack)
            attack->clear();

    if (all)
        unk_0318->clear();
    else if (!unk_0318->empty())
        unk_0318->pop_front();

    next_attack = combat_num_moves;
}

// sub_986C80
bool player_interface::should_execute_combat(f32, f32 tolerance) {
    if (next_attack != combat_num_moves && attacks[next_attack])
        return attacks[next_attack]->should_execute(this, tolerance);

    return false;
}

// sub_99AA90
void player_interface::update_attacks(f32 delta_t) {
    i32 cur_mode = get_mode();

    // the first combat frame keeps last frame's next attack
    if (cur_mode != 9 || mode_last_frame == 9) {
        auto* my_als = (als_inode*)my_actor->get_ai_core()->get_info_node(info_node_type_als);

        if (cur_mode != 9  &&
            cur_mode != 16 &&
            my_als->my_als->get_als_layer(als::base_layer)->did_transition_succeed())

            clear_attacks(true);

        previous_next_attack = next_attack;
        next_attack          = combat_num_moves;

        f32 best_time = -1.0f;

        if (unk_265()) {
            next_attack = combat_move_counter_attack;

            for (i32 i = 0; i < combat_num_moves; ++i)
                if (i != combat_move_counter_attack && attacks[i])
                    attacks[i]->clear();
        } else if (attacks[combat_move_dodge]                                         &&
                   !(my_actor->damage_ifc()->flags & damage_ifc_flag_unk_10000000)     &&
                   attacks[combat_move_dodge]->update(combat_move_dodge,
                                                      this,
                                                      my_als,
                                                      delta_t,
                                                      &best_time,
                                                      combat_num_moves,
                                                      nullptr,
                                                      nullptr)                         &&
                   best_time >= 0.0f) {

            next_attack = combat_move_dodge;

            for (i32 i = 0; i < combat_num_moves; ++i)
                if (i != combat_move_dodge && attacks[i])
                    attacks[i]->clear();
        } else if (!is_carrying_or_carried()) {
            for (i32 i = 0; i < combat_num_moves; ++i) {
                if (i == combat_move_dodge || !attacks[i])
                    continue;

                const plr_combat_attack* current_attack = next_attack != combat_num_moves ? attacks[next_attack] : nullptr;

                if (attacks[i]->update((e_combat_move)i, this, my_als, delta_t, &best_time, next_attack, current_attack, nullptr))
                    next_attack = (e_combat_move)i;
            }
        }
    }

    if (next_attack != combat_move_dodge) {
        unk_312(false);
        unk_314(false);
    }

    auto* my_als = (als_inode*)my_actor->get_ai_core()->get_info_node(info_node_type_als);

    f32 hit_chain = (f32)(unk_165() + 1.0);
    my_als->my_als->get_als_layer(als::base_layer)->set_desired_param(als::param_webcast_hit_chain_number, hit_chain);

    f32 crit_chain = (f32)(unk_166() + 1.0);
    my_als->my_als->get_als_layer(als::base_layer)->set_desired_param(als::param_webcast_crit_chain_number, crit_chain);
}

// slot 222
void player_interface::unk_222(i32 unk, i32 unk_2, i32 unk_3) {
    ((void (__thiscall*)(player_interface*, i32, i32, i32))retail::sub_7AE090)(this, unk, unk_2, unk_3);
}

// sub_9922E0
void player_interface::unk_223(entity_base* ent, i32) {
    if (!ent || !ent->is_an_actor())
        return;

    auto* the_actor = (actor*)ent->my_handle.resolve();

    if (!the_actor || !the_actor->get_ai_core())
        return;

    auto* theirs = (combat_target_inode*)the_actor->get_ai_core()->get_info_node(info_node_type_combat_target);
    auto* mine   = (combat_target_inode*)my_actor ->get_ai_core()->get_info_node(info_node_type_combat_target);

    if (theirs && mine)
        retail::sub_483380((u32*)mine, (i32)theirs); // combat_target_inode hostiles removal
}

// sub_71FBB0
const plr_trick_move* player_interface::get_trick(e_combat_move trk) const {
    return (trk > combat_max_moves && trk < combat_and_tricks_num_moves) ? tricks[trk - combat_max_moves - 1] : nullptr;
}

// sub_71FBD0
e_combat_move player_interface::get_next_trick_enum() const {
    return next_trick;
}

// sub_71FBE0
const plr_trick_move* player_interface::get_next_trick() const {
    return get_trick(get_next_trick_enum());
}

// sub_71FC00
i32 player_interface::get_trick_array_index(e_combat_move trk) const {
    return trk - combat_max_moves - 1;
}

// sub_986CD0
void player_interface::init_tricks() {
    for (plr_trick_move* &trick : tricks) {
        delete trick;
        trick = nullptr;
    }

    clear_tricks();
}

// sub_986D10
void player_interface::clear_tricks() {
    for (plr_trick_move* trick : tricks)
        if (trick)
            trick->clear();

    next_trick = combat_and_tricks_num_moves;
}

// sub_986B00
void player_interface::update_tricks(f32 delta_t) {
    f32 best_time = 0.0f;

    for (i32 i = 0; i < 4; ++i) {
        if (!tricks[i])
            continue;

        bool has_next_trick = next_trick != combat_and_tricks_num_moves &&
                              tricks[get_trick_array_index(next_trick)];

        if (tricks[i]->update(delta_t, &best_time, has_next_trick))
            next_trick = (e_combat_move)(combat_move_trick_a + i);
    }
}

// sub_99B330
bool player_interface::check_for_dodge(      entity_base*               attacker,
                                             i32,
                                       const combo_move_data*           combo_move,
                                             weapon_interface_extender* weapon,
                                             entity_base*               weapon_entity,
                                             i32                        usage_type) {

    if (!reflexes.is_enabled() || !attacks[combat_move_dodge])
        return false;

    attacks[combat_move_dodge]->clear();

    ai_core* core = my_actor->get_ai_core();

    if (!core || !core->my_base_machine || !core->my_base_machine->my_curr_state)
        return false;

    if (mode.current != 9 &&
        mode.current != 3 &&
        !((enhanced_state*)core->my_base_machine->my_curr_state)->can_handle_exit_code(state_exit_code_dodge, false))

        return false;

    ai_target target;
    target.set_target(attacker->my_handle);

    plr_combat_attack* dodge = attacks[combat_move_dodge];

    dodge->attack_dir = my_actor->my_abs_po->get_z_facing();
    dodge->attack_dir.normalize();

    dodge->attack_up = my_actor->my_abs_po->get_y_facing();
    dodge->attack_up.normalize();

    f32 score = !weapon && !attacker->has_manip_obj_ifc() ? dodge->compute_target_score(&target, this, 0) : 1.0f;

    target.score = score;

    if (!((f64)score > (f64)0.0001f))
        return false;

    auto* my_als = (als_inode*)core->get_info_node(info_node_type_als);

    dodge->clear();
    dodge->targets->push_back(target);

    f32 als_time = my_als->get_current_time(als::base_layer);
    dodge->timer = (f32)(als_time + (f64)0.15f);

    if (combo_move) {
        preset_dodge_anim = combo_move->get_dodge_anim(nullptr);
        preset_dodge_type = combo_move->get_dodge_type(nullptr);

        if (preset_dodge_type == -1 && !(combo_move->flags_2 & combo_flag_allow_counter_attack))
            preset_dodge_type = -2;
    } else if (weapon) {
        preset_dodge_anim = "";

        switch (usage_type) {
            case 0:
            case 1:
            case 3:
                preset_dodge_type = weapon->unk_020();

                break;

            case 2:
                preset_dodge_type = -6;

                break;

            default:
                break;
        }

        ai_target weapon_target;
        weapon_target.set_target(weapon_entity ? weapon_entity->my_handle : arch_base_vhandle());

        if (weapon_target.target.resolve()) {
            weapon_target.score = 1.0f;

            dodge->targets->push_back(weapon_target);
        }
    } else if (attacker->has_manip_obj_ifc()) {
        preset_dodge_anim = "";
        preset_dodge_type = attacker->manip_obj_ifc()->unk_088;

        ai_target thrower_target;
        thrower_target.set_target(attacker->manip_obj_ifc()->thrower);

        if (thrower_target.target.resolve()) {
            thrower_target.score = 1.0f;

            dodge->targets->push_back(thrower_target);
        }
    } else {
        preset_dodge_anim = "";
        preset_dodge_type = -1;
    }

    if (my_actor->has_combo_ifc()) {
        f32 counter_value;
        f32 delay;

        if (combo_move) {
            counter_value = combo_move->get_combo_counter_value(nullptr);
            delay         = combo_move->get_combo_counter_delay(nullptr);
        } else {
            counter_value = 1.0f;
            delay         = -1.0f;
        }

        retail::sub_626630((i32)my_actor->combo_ifc(), counter_value, 6);
        retail::sub_601640(my_actor->combo_ifc(), delay);
    }

    return true;
}

// sub_986D50
bool player_interface::should_execute_dodge(f32) {
    return next_attack == combat_move_dodge;
}

// sub_71FC10
const char* player_interface::get_dodge_anim() const {
    return preset_dodge_anim.c_str();
}

// sub_71FC20
void player_interface::override_dodge_anim(const char* a) {
    preset_dodge_anim = a;
}

// sub_71FC30
i32 player_interface::get_dodge_type() const {
    return preset_dodge_type;
}

// sub_71FC40
void player_interface::override_dodge_type(i32 t) {
    preset_dodge_type = t;
}

// slot 237
bool player_interface::check_for_block(      entity_base*               attacker,
                                             i32                        chain_size,
                                       const combo_move_data*           combo_move,
                                             hit_react_record*          react,
                                             weapon_interface_extender* weapon,
                                             entity_base*               weapon_entity,
                                             i32                        usage_type) {

    return ((bool (__thiscall*)(player_interface*,
                                entity_base*,
                                i32,
                                const combo_move_data*,
                                hit_react_record*,
                                weapon_interface_extender*,
                                entity_base*,
                                i32))retail::sub_986D60)(this,
                                                         attacker,
                                                         chain_size,
                                                         combo_move,
                                                         react,
                                                         weapon,
                                                         weapon_entity,
                                                         usage_type);
}

// sub_986D70
bool player_interface::should_execute_block(f32) {
    return next_attack == combat_move_block;
}

// sub_71FC50
const char* player_interface::get_block_anim() const {
    return preset_block_anim.c_str();
}

// sub_71FC60
void player_interface::override_block_anim(const char* a) {
    preset_block_anim = a;
}

// sub_71FC70
i32 player_interface::get_block_type() const {
    return preset_block_type;
}

// sub_71FC80
void player_interface::override_block_type(i32 t) {
    preset_block_type = t;
}

// slot 243
arch_base_vhandle player_interface::unk_243() {
    arch_base_vhandle result;
    ((arch_base_vhandle* (__thiscall*)(player_interface*, arch_base_vhandle*))retail::sub_720180)(this, &result);

    return result;
}

// slot 244
arch_base_vhandle player_interface::unk_244() {
    arch_base_vhandle result;
    ((arch_base_vhandle* (__thiscall*)(player_interface*, arch_base_vhandle*))retail::sub_720180)(this, &result);

    return result;
}

// slot 245
i32 player_interface::unk_245() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 246
f32 player_interface::unk_246() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 247
f32 player_interface::unk_247() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 248
bool player_interface::unk_248() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 249
void player_interface::unk_249(bool unk) {
    ((void (__thiscall*)(player_interface*, bool))retail::sub_689610)(this, unk);
}

// slot 250
f32 player_interface::unk_250() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 251
f32 player_interface::get_block_strength() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 252
void player_interface::unk_252(f32 unk) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, unk);
}

// slot 253
f32 player_interface::unk_253() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 254
void player_interface::unk_254(f32 unk) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, unk);
}

// slot 255
void player_interface::unk_255(f32 delta_t) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, delta_t);
}

// slot 256
void player_interface::unk_256() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 257
bool player_interface::unk_257() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 258
arch_base_vhandle player_interface::unk_258() {
    arch_base_vhandle result;
    ((arch_base_vhandle* (__thiscall*)(player_interface*, arch_base_vhandle*))retail::sub_720180)(this, &result);

    return result;
}

// slot 259
bool player_interface::unk_259(f32 delta_t) {
    return ((bool (__thiscall*)(player_interface*, f32))retail::sub_999660)(this, delta_t);
}

// slot 260
bool player_interface::unk_260(f32 delta_t) {
    return ((bool (__thiscall*)(player_interface*, f32))retail::sub_999760)(this, delta_t);
}

// slot 261
bool player_interface::unk_261(void* unk, void* unk_2, void* unk_3) {
    return ((bool (__thiscall*)(player_interface*, void*, void*, void*))retail::sub_999840)(this, unk, unk_2, unk_3);
}

// slot 262
i32 player_interface::unk_262(i32 unk, i32 unk_2) {
    return ((i32 (__thiscall*)(player_interface*, i32, i32))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 263
bool player_interface::unk_263(void* unk, i32 unk_2) {
    return ((bool (__thiscall*)(player_interface*, void*, i32))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 264
bool player_interface::unk_264(f32 delta_t, void* unk_2, i32 unk_3, i32 unk_4, i32 unk_5) {
    return ((bool (__thiscall*)(player_interface*, f32, void*, i32, i32, i32))retail::sub_71FC90)(this, delta_t, unk_2, unk_3, unk_4, unk_5);
}

// slot 265
bool player_interface::unk_265() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 266
void player_interface::force_evade(vector3d v) {
    ((void (__thiscall*)(player_interface*, vector3d))retail::sub_7AE090)(this, v);
}

// slot 267
void player_interface::unk_267() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 268
bool player_interface::unk_268() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 269
vector3d player_interface::unk_269() {
    vector3d result;
    ((vector3d* (__thiscall*)(player_interface*, vector3d*))retail::sub_71F830)(this, &result);

    return result;
}

// slot 270
bool player_interface::unk_270() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 271
bool player_interface::unk_271() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 272
bool player_interface::unk_272() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 273
entity* player_interface::unk_273(entity* target) {
    return ((entity* (__thiscall*)(player_interface*, entity*))retail::sub_9CAAD0)(this, target);
}

// slot 274
bool player_interface::should_pick_up_ai(f32 delta_t) {
    return ((bool (__thiscall*)(player_interface*, f32))retail::sub_9923E0)(this, delta_t);
}

// slot 275
bool player_interface::unk_275(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_996800)(this, unk);
}

// slot 276
void* player_interface::unk_276(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_71FCA0)(this, unk);
}

// sub_9925A0
bool player_interface::unk_277(ai_core* core) {
    auto* action_proc = (ai_action_processor_inode*)core->get_info_node(info_node_type_ai_action_processor);

    if (action_proc->has_actions())
        return false;

    auto* carry = (std_carry_inode*)core->get_info_node(info_node_type_std_carry);

    if (!carry || !carry->desired_slave)
        return false;

    auto* action = new pick_up_ai_action(core);
    action->setup(carry->desired_slave);

    action_proc->add_action(action);
    carry->desired_slave = nullptr;

    return true;
}

// slot 278
bool player_interface::unk_278(f32 delta_t) {
    return ((bool (__thiscall*)(player_interface*, f32))retail::sub_9926D0)(this, delta_t);
}

// slot 279
bool player_interface::unk_279(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_9968A0)(this, unk);
}

// slot 280
void* player_interface::unk_280(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_71FCB0)(this, unk);
}

// slot 281
bool player_interface::unk_281(ai_core* core) {
    return ((bool (__thiscall*)(player_interface*, ai_core*))retail::sub_992800)(this, core);
}

// slot 282
bool player_interface::unk_282(ai_core* core) {
    return ((bool (__thiscall*)(player_interface*, ai_core*))retail::sub_992930)(this, core);
}

// sub_986D80
bool player_interface::is_current_combat_target(arch_base_vhandle targ) const {
    if (!targ.resolve())
        return false;

    base_state* state = my_actor->get_ai_core()->my_base_machine->my_curr_state;

    if (state && state->get_virtual_type_key() == plr_combat_state::my_type_key.source_hash_code)
        return ((bool (__thiscall*)(base_state*, arch_base_vhandle))retail::sub_416D60)(state, targ); // the combat state's hit list

    return false;
}

// slot 284
bool player_interface::unk_284() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// sub_71FCC0
player_combo_counter* player_interface::unk_285() {
    return unk_042c;
}

// sub_71FCD0
player_combo_counter* player_interface::get_combo_counter_display() const {
    return combo_counter_display;
}

// sub_986B90
void player_interface::inc_combo_counter(i32 num) {
    set_combo_counter(get_combo_counter() + num);
}

// sub_996680
void player_interface::set_combo_counter(i32 num) {
    f32 value = (f32)num;

    if ((f64)value > (f64)0.1f && get_combo_counter() != num) {
        combo_counter_display->scale_text(vector2d(1.15f, 1.15f), 0.75f);
        combo_counter_display->flash_color(color32(0x00, 0xFF, 0x00), 0.75f);
    } else if ((f64)-0.1f > (f64)value && get_combo_counter() != num) {
        combo_counter_display->scale_text(vector2d(1.0f, 1.0f), 0.75f);
        combo_counter_display->flash_color(color32(0xFF, 0x00, 0x00), 0.75f);
    }

    combo_counter_display->combo_counter = num;
}

// sub_71FCE0
player_combo_counter* player_interface::unk_289(i32 unk) {
    combo_counter_display->unk_50 = unk;

    return combo_counter_display;
}

// sub_71FCF0
player_combo_counter* player_interface::unk_290(i32 unk) {
    combo_counter_display->unk_4c = unk;

    return combo_counter_display;
}

// sub_986BC0
void player_interface::set_combo_counter_delay(f32 num, bool force) { // only rises unless forced
    if (0.0f > num)
        num = 1.0f;

    if ((f64)num > (f64)combo_counter_delay.time || force)
        combo_counter_delay = num;
}

// sub_71FD00
i32 player_interface::get_combo_counter() const {
    return combo_counter_display->combo_counter;
}

// sub_71FD10
i32 player_interface::unk_293() {
    return combo_counter_display->unk_4c;
}

// sub_71FD20
i32 player_interface::unk_294() {
    return combo_counter_display->unk_50;
}

// slot 295
f32 player_interface::unk_295() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_71FD30)(this);
}

// slot 296
void player_interface::unk_296(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_71FD40)(this, unk);
}

// slot 297
f32 player_interface::unk_297() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_71FD60)(this);
}

// slot 298
void player_interface::unk_298(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_71FD70)(this, unk);
}

// sub_71FD90
vector3d player_interface::get_initial_jump_force_applied() const {
    return initial_jump_force_applied;
}

// sub_71FDB0
void player_interface::set_initial_jump_force_applied(vector3d f) {
    initial_jump_force_applied = f;
}

// sub_71FDE0
vector3d player_interface::get_jump_start() const {
    return jump_start;
}

// sub_71FE00
void player_interface::set_jump_start(vector3d s) {
    jump_start = s;
}

// slot 303
void* player_interface::unk_303(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_71FE30)(this, unk);
}

// slot 304
i32 player_interface::unk_304(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71FE60)(this, unk);
}

// slot 305
bool player_interface::is_in_rvb_black_mode() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 306
i32 player_interface::unk_306() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 307
i32 player_interface::unk_307() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 308
f32 player_interface::unk_308() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 309
void player_interface::unk_309(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 310
f32 player_interface::unk_310() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 311
bool player_interface::unk_311() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71FE80)(this);
}

// slot 312
bool player_interface::unk_312(bool unk) {
    return ((bool (__thiscall*)(player_interface*, bool))retail::sub_71FE90)(this, unk);
}

// slot 313
bool player_interface::unk_313() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71FEA0)(this);
}

// slot 314
bool player_interface::unk_314(bool unk) {
    return ((bool (__thiscall*)(player_interface*, bool))retail::sub_71FEB0)(this, unk);
}

// slot 315
f32 player_interface::unk_315() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 316
i32 player_interface::unk_316() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71FB70)(this);
}

// slot 317
f32 player_interface::unk_317() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720040)(this);
}

// slot 318
f32 player_interface::unk_318() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720040)(this);
}

// slot 319
bool player_interface::unk_319(f32 delta_t, game_button* button, i32 unk_3) {
    return ((bool (__thiscall*)(player_interface*, f32, game_button*, i32))retail::sub_5FCC20)(this, delta_t, button, unk_3);
}

// slot 320
void* player_interface::unk_320() {
    return ((void* (__thiscall*)(player_interface*))retail::sub_71FEC0)(this);
}

// slot 321
void* player_interface::unk_321(const line_info* unk) {
    return ((void* (__thiscall*)(player_interface*, const line_info*))retail::sub_689610)(this, unk);
}

// slot 322
bool player_interface::unk_322(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 323
void* player_interface::unk_323(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_689610)(this, unk);
}

// slot 324
i32 player_interface::unk_324(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71F830)(this, unk);
}

// slot 325
f32 player_interface::unk_325() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 326
bool player_interface::unk_326() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 327
bool player_interface::unk_327(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 328
void* player_interface::unk_328() {
    return ((void* (__thiscall*)(player_interface*))retail::sub_71FED0)(this);
}

// slot 329
void* player_interface::unk_329(void* unk) {
    return ((void* (__thiscall*)(player_interface*, void*))retail::sub_71FEE0)(this, unk);
}

// slot 330
void player_interface::unk_330(f32 duration, bool force) {
    ((void (__thiscall*)(player_interface*, f32, bool))retail::sub_AD2D40)(this, duration, force);
}

// slot 331
void player_interface::unk_331(f32 duration, bool force) {
    ((void (__thiscall*)(player_interface*, f32, bool))retail::sub_AD2D40)(this, duration, force);
}

// slot 332
void player_interface::unk_332(f32 duration, bool force) {
    ((void (__thiscall*)(player_interface*, f32, bool))retail::sub_AD2D40)(this, duration, force);
}

// slot 333
void player_interface::unk_333(f32 duration, bool force) {
    ((void (__thiscall*)(player_interface*, f32, bool))retail::sub_AD2D40)(this, duration, force);
}

// slot 334
void player_interface::unk_334(f32 duration, bool force) {
    ((void (__thiscall*)(player_interface*, f32, bool))retail::sub_AD2D40)(this, duration, force);
}

// slot 335
void player_interface::unk_335(f32 duration, bool force) {
    ((void (__thiscall*)(player_interface*, f32, bool))retail::sub_AD2D40)(this, duration, force);
}

// slot 336
void player_interface::unk_336(f32 duration, bool force) {
    ((void (__thiscall*)(player_interface*, f32, bool))retail::sub_AD2D40)(this, duration, force);
}

// slot 337
i32 player_interface::unk_337() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 338
i32 player_interface::unk_338() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 339
i32 player_interface::unk_339() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 340
bool player_interface::unk_340() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 341
bool player_interface::unk_341() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 342
i32 player_interface::unk_342() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 343
i32 player_interface::unk_343() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 344
void player_interface::unk_344(f32 unk) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, unk);
}

// slot 345
void player_interface::unk_345(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 346
void player_interface::unk_346(f32 unk) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, unk);
}

// slot 347
void player_interface::unk_347(f32 unk) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, unk);
}

// slot 348
void player_interface::unk_348(f32 unk) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, unk);
}

// slot 349
void player_interface::unk_349(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 350
void player_interface::unk_350(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 351
void player_interface::unk_351(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 352
void player_interface::unk_352(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 353
void player_interface::unk_353(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 354
void player_interface::unk_354(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 355
void player_interface::unk_355(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 356
void player_interface::unk_356(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 357
void player_interface::unk_357(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 358
void player_interface::unk_358(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 359
void player_interface::unk_359(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 360
void player_interface::unk_360(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 361
void player_interface::unk_361(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 362
void player_interface::unk_362(i32 unk, i32 unk_2, i32 unk_3, i32 unk_4) {
    ((void (__thiscall*)(player_interface*, i32, i32, i32, i32))retail::sub_9CD780)(this, unk, unk_2, unk_3, unk_4);
}

// slot 363
void player_interface::get_sweet_spot_params(f32* incline_sin,
                                             f32* incline_cos,
                                             f32* sweet_cone_cos,
                                             f32* distance,
                                             f32* min_len_squared,
                                             f32* max_len_squared) {

    ((void (__thiscall*)(player_interface*, f32*, f32*, f32*, f32*, f32*, f32*))retail::sub_71FF60)(this,
                                                                                                    incline_sin,
                                                                                                    incline_cos,
                                                                                                    sweet_cone_cos,
                                                                                                    distance,
                                                                                                    min_len_squared,
                                                                                                    max_len_squared);
}

// slot 364
void player_interface::get_dynamic_sweet_spot_params(f32* min_length,
                                                     f32* max_length,
                                                     f32* overdrive_length,
                                                     f32* overdrive_velocity,
                                                     f32* max_no_anchor_length,
                                                     f32* min_angle,
                                                     f32* max_angle) {

    ((void (__thiscall*)(player_interface*, f32*, f32*, f32*, f32*, f32*, f32*, f32*))retail::sub_71FEF0)(this,
                                                                                                          min_length,
                                                                                                          max_length,
                                                                                                          overdrive_length,
                                                                                                          overdrive_velocity,
                                                                                                          max_no_anchor_length,
                                                                                                          min_angle,
                                                                                                          max_angle);
}

// slot 365
i32 player_interface::unk_365(void* unk, void* unk_2) {
    return ((i32 (__thiscall*)(player_interface*, void*, void*))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 366
void* player_interface::unk_366(void* unk, void* unk_2, void* unk_3, void* unk_4) {
    return ((void* (__thiscall*)(player_interface*, void*, void*, void*, void*))retail::sub_9CD780)(this, unk, unk_2, unk_3, unk_4);
}

// slot 367
i32 player_interface::unk_367(void* unk, void* unk_2) {
    return ((i32 (__thiscall*)(player_interface*, void*, void*))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 368
void player_interface::unk_368(f32 unk, swing_data* swinger) {
    ((void (__thiscall*)(player_interface*, f32, swing_data*))retail::sub_AD2D40)(this, unk, swinger);
}

// slot 369
void player_interface::unk_369(i32 unk, i32 unk_2) {
    ((void (__thiscall*)(player_interface*, i32, i32))retail::sub_5BD2C0)(this, unk, unk_2);
}

// slot 370
bool player_interface::break_web(e_swinger_hand web_to_break, bool add_momentum, bool add_dangler) {
    return ((bool (__thiscall*)(player_interface*, e_swinger_hand, bool, bool))retail::sub_5FCC20)(this, web_to_break, add_momentum, add_dangler);
}

// slot 371
i32 player_interface::unk_371(i32 unk, i32 unk_2) {
    return ((i32 (__thiscall*)(player_interface*, i32, i32))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 372
entity* player_interface::get_last_anchor() const {
    return ((entity* (__thiscall*)(const player_interface*))retail::sub_A944D0)(this);
}

// slot 373
i32 player_interface::unk_373() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 374
f32 player_interface::unk_374() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 375
f32 player_interface::unk_375() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 376
i32 player_interface::unk_376() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 377
bool player_interface::unk_377(vector3d* point) {
    return ((bool (__thiscall*)(player_interface*, vector3d*))retail::sub_71FF10)(this, point);
}

// slot 378
bool player_interface::unk_378() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 379
i32 player_interface::unk_379() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 380
i32 player_interface::unk_380(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_A90D90)(this, unk);
}

// slot 381
f32 player_interface::unk_381(void* unk, i32 unk_2, f32 unk_3) {
    return ((f32 (__thiscall*)(player_interface*, void*, i32, f32))retail::sub_71FF00)(this, unk, unk_2, unk_3);
}

// slot 382
bool player_interface::unk_382(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_71FF10)(this, unk);
}

// slot 383
i32 player_interface::unk_383() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 384
bool player_interface::unk_384() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 385
i32 player_interface::unk_385() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 386
bool player_interface::unk_386(f32 unk, bool unk_2) {
    return ((bool (__thiscall*)(player_interface*, f32, bool))retail::sub_5BD2C0)(this, unk, unk_2);
}

// slot 387
void* player_interface::unk_387() {
    return ((void* (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 388
bool player_interface::unk_388() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 389
bool player_interface::unk_389(void* unk, f32 unk_2) {
    return ((bool (__thiscall*)(player_interface*, void*, f32))retail::sub_5BD2C0)(this, unk, unk_2);
}

// slot 390
bool player_interface::unk_390() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 391
bool player_interface::unk_391(i32 unk, i32 unk_2) {
    return ((bool (__thiscall*)(player_interface*, i32, i32))retail::sub_5BD2C0)(this, unk, unk_2);
}

// slot 392
bool player_interface::unk_392(bool skip) {
    return ((bool (__thiscall*)(player_interface*, bool))retail::sub_71FF10)(this, skip);
}

// slot 393
bool player_interface::unk_393(f32 unk) {
    return ((bool (__thiscall*)(player_interface*, f32))retail::sub_71FF10)(this, unk);
}

// slot 394
i32 player_interface::unk_394() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 395
i32 player_interface::unk_395() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71FB70)(this);
}

// slot 396
i32 player_interface::unk_396(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_71FF20)(this, unk);
}

// slot 397
i32 player_interface::unk_397() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71FF30)(this);
}

// slot 398
i32 player_interface::unk_398(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_A90D90)(this, unk);
}

// slot 399
swing_data* player_interface::get_swinger(e_swinger_hand i) {
    return ((swing_data* (__thiscall*)(player_interface*, e_swinger_hand))retail::sub_A90D90)(this, i);
}

// slot 400
void player_interface::unk_400(polytube* tube, f32 radius) {
    ((void (__thiscall*)(player_interface*, polytube*, f32))retail::sub_AD2D40)(this, tube, radius);
}

// slot 401
i32 player_interface::get_als_swing_hand_value() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 402
i32 player_interface::unk_402(i32 unk, i32 unk_2) {
    return ((i32 (__thiscall*)(player_interface*, i32, i32))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 403
f32 player_interface::unk_403() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720040)(this);
}

// slot 404
f32 player_interface::unk_404(f32 progress) {
    return ((f32 (__thiscall*)(player_interface*, f32))retail::sub_71FF40)(this, progress);
}

// slot 405
f32 player_interface::unk_405(f32 progress) {
    return ((f32 (__thiscall*)(player_interface*, f32))retail::sub_71FF50)(this, progress);
}

// slot 406
f32 player_interface::unk_406(f32 progress) {
    return ((f32 (__thiscall*)(player_interface*, f32))retail::sub_71FF50)(this, progress);
}

// slot 407
f32 player_interface::unk_407(f32 progress) {
    return ((f32 (__thiscall*)(player_interface*, f32))retail::sub_71FF50)(this, progress);
}

// slot 408
void player_interface::unk_408(i32 unk, i32 unk_2) {
    ((void (__thiscall*)(player_interface*, i32, i32))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 409
void player_interface::unk_409(i32 unk, i32 unk_2, i32 unk_3) {
    ((void (__thiscall*)(player_interface*, i32, i32, i32))retail::sub_7AE090)(this, unk, unk_2, unk_3);
}

// slot 410
void player_interface::unk_410(i32 unk, i32 unk_2, i32 unk_3, i32 unk_4) {
    ((void (__thiscall*)(player_interface*, i32, i32, i32, i32))retail::sub_9CD780)(this, unk, unk_2, unk_3, unk_4);
}

// slot 411
i32 player_interface::unk_411(i32 unk, i32 unk_2) {
    return ((i32 (__thiscall*)(player_interface*, i32, i32))retail::sub_AD2D40)(this, unk, unk_2);
}

// slot 412
void player_interface::unk_412(i32 unk, i32 unk_2, i32 unk_3, i32 unk_4, i32 unk_5, i32 unk_6) {
    ((void (__thiscall*)(player_interface*, i32, i32, i32, i32, i32, i32))retail::sub_71FF60)(this, unk, unk_2, unk_3, unk_4, unk_5, unk_6);
}

// slot 413
i32 player_interface::unk_413(i32 unk, i32 unk_2, void* unk_3) {
    return ((i32 (__thiscall*)(player_interface*, i32, i32, void*))retail::sub_71FF70)(this, unk, unk_2, unk_3);
}

// slot 414
f32 player_interface::unk_414() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 415
f32 player_interface::unk_415() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 416
f32 player_interface::unk_416() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 417
i32 player_interface::unk_417() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 418
i32 player_interface::unk_418() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 419
i32 player_interface::unk_419() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 420
i32 player_interface::unk_420() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 421
i32 player_interface::unk_421() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 422
i32 player_interface::unk_422() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 423
i32 player_interface::unk_423() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 424
f32 player_interface::compute_swing_angle() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_987000)(this);
}

// sub_71FF90
f32 player_interface::get_als_swing_angle() {
    return (f32)(compute_swing_angle() * 57.29577791868205);
}

// slot 426
f32 player_interface::compute_swing_side_angle() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_987160)(this);
}

// sub_71FFB0
f32 player_interface::get_als_swing_side_angle() {
    return (f32)(compute_swing_side_angle() * 57.29577791868205);
}

// slot 428
f32 player_interface::compute_swing_to_anchor_angle() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_992CB0)(this);
}

// sub_71FFD0
f32 player_interface::get_als_swing_to_anchor_angle() {
    return (f32)(compute_swing_to_anchor_angle() * 57.29577791868205);
}

// slot 430
f32 player_interface::unk_430() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_9872D0)(this);
}

// sub_71FFF0
f32 player_interface::unk_431() {
    return (f32)(unk_430() * 57.29577791868205);
}

// slot 432
f32 player_interface::unk_432() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 433
f32 player_interface::unk_433() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 434
f32 player_interface::unk_434() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 435
f32 player_interface::unk_435() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720010)(this);
}

// slot 436
f32 player_interface::unk_436() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 437
f32 player_interface::unk_437() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720020)(this);
}

// slot 438
f32 player_interface::unk_438() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720020)(this);
}

// slot 439
void player_interface::unk_439(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 440
f32 player_interface::unk_440() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 441
f32 player_interface::unk_441() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 442
f32 player_interface::unk_442() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720030)(this);
}

// slot 443
f32 player_interface::unk_443() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 444
f32 player_interface::unk_444() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 445
f32 player_interface::unk_445() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720040)(this);
}

// slot 446
f32 player_interface::unk_446() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 447
f32 player_interface::unk_447() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 448
f32 player_interface::unk_448() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720040)(this);
}

// slot 449
f32 player_interface::unk_449() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720040)(this);
}

// slot 450
f32 player_interface::unk_450() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720040)(this);
}

// slot 451
f32 player_interface::unk_451() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720040)(this);
}

// slot 452
f32 player_interface::unk_452() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 453
i32 player_interface::unk_453(i32 unk, i32 unk_2, i32 unk_3) {
    return ((i32 (__thiscall*)(player_interface*, i32, i32, i32))retail::sub_7AE090)(this, unk, unk_2, unk_3);
}

// slot 454
f32 player_interface::unk_454() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720060)(this);
}

// slot 455
i32 player_interface::unk_455(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 456
bool player_interface::unk_456() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 457
bool player_interface::unk_457(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 458
void player_interface::unk_458(void* unk, void* unk_2, f32 unk_3) {
    ((void (__thiscall*)(player_interface*, void*, void*, f32))retail::sub_7AE090)(this, unk, unk_2, unk_3);
}

// slot 459
i32 player_interface::unk_459() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_720070)(this);
}

// slot 460
i32 player_interface::unk_460(f32 unk) {
    return ((i32 (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, unk);
}

// slot 461
bool player_interface::should_do_strength_test() const {
    return ((bool (__thiscall*)(const player_interface*))retail::sub_71F870)(this);
}

// slot 462
bool player_interface::is_a_scripted_strength_test() const {
    return ((bool (__thiscall*)(const player_interface*))retail::sub_71F870)(this);
}

// slot 463
bool player_interface::unk_463() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 464
i32 player_interface::unk_464() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 465
void player_interface::unk_465() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 466
bool player_interface::unk_466() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 467
void* player_interface::unk_467() {
    return ((void* (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 468
void player_interface::unk_468() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 469
bool player_interface::unk_469() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 470
bool player_interface::unk_470() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 471
i32 player_interface::unk_471() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 472
void player_interface::unk_472() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// sub_720080
arch_base_vhandle player_interface::get_combat_vehicle() const {
    return combat_vehicle;
}

// sub_720260
void player_interface::set_combat_vehicle(arch_base_vhandle v) {
    combat_vehicle = v;
}

// slot 475
f32 player_interface::unk_475() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720090)(this);
}

// slot 476
void player_interface::unk_476(i32 unk) {
    ((void (__thiscall*)(player_interface*, i32))retail::sub_7200A0)(this, unk);
}

// sub_7200C0
void player_interface::set_post_process_flag(e_post_process_action f, bool set) {
    if (set)
        post_process_flags |= f;
    else
        post_process_flags &= ~f;
}

// slot 478
bool player_interface::unk_478(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_7200F0)(this, unk);
}

// slot 479
bool player_interface::unk_479() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_720100)(this);
}

// slot 480
bool player_interface::unk_480(bool unk) {
    return ((bool (__thiscall*)(player_interface*, bool))retail::sub_720110)(this, unk);
}

// slot 481
bool player_interface::unk_481() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_720130)(this);
}

// slot 482
bool player_interface::unk_482(bool unk) {
    return ((bool (__thiscall*)(player_interface*, bool))retail::sub_720140)(this, unk);
}

// sub_992E00
arch_base_vhandle player_interface::unk_483(const plr_combat_attack*) const {
    return arch_base_vhandle();
}

// slot 484
bool player_interface::unk_484() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 485
bool player_interface::unk_485() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 486
bool player_interface::unk_486(bool unk, f32 delay) {
    return ((bool (__thiscall*)(player_interface*, bool, f32))retail::sub_AD2D40)(this, unk, delay);
}

// slot 487
bool player_interface::unk_487() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 488
bool player_interface::unk_488(bool unk, f32 delay) {
    return ((bool (__thiscall*)(player_interface*, bool, f32))retail::sub_AD2D40)(this, unk, delay);
}

// slot 489
i32 player_interface::unk_489() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 490
i32 player_interface::unk_490() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 491
f32 player_interface::unk_491() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 492
void player_interface::unk_492(bool start) {
    ((void (__thiscall*)(player_interface*, bool))retail::sub_689610)(this, start);
}

// slot 493
i32 player_interface::unk_493() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 494
i32 player_interface::unk_494() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 495
i32 player_interface::is_in_webcast_approach_normal_hit_range() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 496
i32 player_interface::is_in_webcast_approach_critical_hit_range() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 497
i32 player_interface::is_in_approach_counter() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 498
i32 player_interface::is_in_webcast_approach_early_range() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 499
i32 player_interface::is_in_webcast_approach_counter_range() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 500
i32 player_interface::unk_500() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 501
i32 player_interface::unk_501() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 502
f32 player_interface::approach_norm_to_crit_percent() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 503
i32 player_interface::unk_503() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 504
bool player_interface::is_webcasting_entity(arch_base_vhandle entity) {
    return ((bool (__thiscall*)(player_interface*, arch_base_vhandle))retail::sub_71FF10)(this, entity);
}

// slot 505
f32 player_interface::unk_505() {
    return ((f32 (__thiscall*)(player_interface*))retail::sub_720050)(this);
}

// slot 506
void player_interface::unk_506(bool start) {
    ((void (__thiscall*)(player_interface*, bool))retail::sub_689610)(this, start);
}

// slot 507
const vector3d* player_interface::unk_507(const vector3d* v) {
    return ((const vector3d* (__thiscall*)(player_interface*, const vector3d*))retail::sub_689610)(this, v);
}

// sub_9874C0
vector3d player_interface::unk_508() const {
    return my_actor->my_abs_po->get_z_facing();
}

// slot 509
i32 player_interface::unk_509() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 510
void player_interface::unk_510(bool start) {
    ((void (__thiscall*)(player_interface*, bool))retail::sub_689610)(this, start);
}

// slot 511
bool player_interface::unk_511() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 512
bool player_interface::unk_512(bool unk) {
    return ((bool (__thiscall*)(player_interface*, bool))retail::sub_689610)(this, unk);
}

// slot 513
bool player_interface::unk_513(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_71FF10)(this, unk);
}

// slot 514
void* player_interface::unk_514(i32 unk) {
    return ((void* (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 515
bool player_interface::is_in_wallmode() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 516
i32 player_interface::unk_516(i32 unk, void* unk_2, bool unk_3) {
    return ((i32 (__thiscall*)(player_interface*, i32, void*, bool))retail::sub_992E10)(this, unk, unk_2, unk_3);
}

// slot 517
i32 player_interface::unk_517() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 518
bool player_interface::unk_518(i32 unk) {
    return ((bool (__thiscall*)(player_interface*, i32))retail::sub_689610)(this, unk);
}

// slot 519
bool player_interface::unk_519() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 520
bool player_interface::unk_520() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 521
void player_interface::unk_521() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 522
i32 player_interface::unk_522() {
    return ((i32 (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 523
void player_interface::lock_suit(bool lock, bool black) {
    ((void (__thiscall*)(player_interface*, bool, bool))retail::sub_AD2D40)(this, lock, black);
}

// slot 524
bool player_interface::unk_524() {
    return ((bool (__thiscall*)(player_interface*))retail::sub_71F870)(this);
}

// slot 525
void player_interface::mode_lock_suit(bool lock, f32 time) {
    ((void (__thiscall*)(player_interface*, bool, f32))retail::sub_AD2D40)(this, lock, time);
}

// slot 526
void player_interface::unk_526() {
    ((void (__thiscall*)(player_interface*))retail::sub_5B4A70)(this);
}

// slot 527
void player_interface::unk_527(f32 delta_t) {
    ((void (__thiscall*)(player_interface*, f32))retail::sub_689610)(this, delta_t);
}

// slot 528
line_info* player_interface::unk_528() {
    return ((line_info* (__thiscall*)(player_interface*))retail::sub_A944D0)(this);
}

// slot 529
void player_interface::unk_529(const line_info* line) {
    ((void (__thiscall*)(player_interface*, const line_info*))retail::sub_689610)(this, line);
}

// slot 530
bool player_interface::unk_530(f32 delta_t) {
    return ((bool (__thiscall*)(player_interface*, f32))retail::sub_71FF10)(this, delta_t);
}

// slot 531
bool player_interface::unk_531(const line_info* line) {
    return ((bool (__thiscall*)(player_interface*, const line_info*))retail::sub_71FF10)(this, line);
}

// slot 532
bool player_interface::unk_532(const line_info* unk) {
    return ((bool (__thiscall*)(player_interface*, const line_info*))retail::sub_71FF10)(this, unk);
}

// slot 533
i32 player_interface::unk_533(i32 unk) {
    return ((i32 (__thiscall*)(player_interface*, i32))retail::sub_9A6650)(this, unk);
}

// slot 534
void player_interface::unk_534(arch_base_vhandle target, f32 time) {
    ((void (__thiscall*)(player_interface*, arch_base_vhandle, f32))retail::sub_AD2D40)(this, target, time);
}

// sub_99AFE0
entity_base* player_interface::select_lockon_target(const vector3d &base_pos,
                                                          f32       min_range,
                                                          f32       max_range,
                                                          f32       min_dotp,
                                                    const vector3d &dir,
                                                    const vector3d &up) const {

    ai_target         candidate;
    arch_base_vhandle best;
    f32               best_score = -1.0f;

    retail::sub_5FE3D0(); // damageable search lock

    if (retail::sub_643530((u32*)&base_pos, max_range, 3) > 0) { // damage_interface::find_damageable, alive and visible
        vc8::list<damage_interface*>* found = references::found_damageable.read();

        for (auto* node = found->begin(); node != found->end(); node = node->next) {
            candidate.set_target(node->value->my_actor->my_handle);

            f32 score = candidate.compute_score(my_actor,
                                                base_pos,
                                                dir,
                                                up,
                                                min_range,
                                                max_range,
                                                -max_range,
                                                max_range,
                                                min_dotp,
                                                -1.1f,
                                                0x80041,
                                                nullptr,
                                                0,
                                                0,
                                                0x14);

            if (score > best_score) {
                best       = candidate.target;
                best_score = score;
            }
        }
    }

    retail::sub_5FE3F0(); // damageable search unlock

    return (entity_base*)best.resolve();
}

// sub_992A60
bool player_interface::is_carrying_or_carried() const {
    auto* carry = (std_carry_inode*)my_actor->get_ai_core()->get_info_node(info_node_type_std_carry);

    return carry && (carry->is_carrying() || carry->is_being_carried());
}

// sub_783750
void player_interface::clear_buttons() { // every button but the first of each array
    for (i32 ctrl = 1; ctrl < 25; ++ctrl)
        buttons[ctrl].clear();

    for (i32 cmd = 1; cmd < plr_num_cmd; ++cmd)
        command_buttons[cmd].clear();
}

void player_interface::clear_controller(bool unk) {
    retail::sub_991FC0((i32*)this, unk);
}

// sub_83D9E0
void player_interface::override_button(i32 ctrl, f32 value, f32 t) {
    buttons[ctrl].update_state(t, value, value);
}

// sub_83DA10
void player_interface::override_command(e_player_button_command cmd, f32 value, f32 t) {
    command_buttons[cmd].update_state(t, value, value);
}

// sub_83DA40
void player_interface::set_button(i32 ctrl, f32 value) {
    buttons[ctrl].state  = value;
    buttons[ctrl].unk_1c = value;
}

// sub_83DA70
void player_interface::set_axis(i32 ctrl, bool ud, f32 value) {
    game_button* button = ud ? control_axis[ctrl]->axis_ud : control_axis[ctrl]->axis_lr;

    button->state  = value;
    button->unk_1c = value;
}
