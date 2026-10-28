#include <new>

#include "retail.hh"
#include "game/wds/entity/interface/damage_interface.hh"
#include "treyarch/mm/mm.hh"

using namespace treyarch;

// sub_548470
hit_react_record::hit_react_record(i32 hit_react_level, f32 unk_04) : hit_react_level(hit_react_level), unk_04(unk_04) {
    retail::sub_5AF1E0((u32*)&scales[0]); // hit_react_scale::hit_react_scale
    retail::sub_5AF1E0((u32*)&scales[1]);
    retail::sub_5AF1E0((u32*)&scales[2]);

    unk_38 = 0;
}

// sub_6367C0
void damage_interface::set_max_hit_points(i32 max) {
    hit_points.set_max(max);

    if (hit_points.get() > hit_points.get_max())
        hit_points.set(hit_points.get_max());
}

void damage_interface::set_hit_points(i32 value, bool use_signals, entity* attacker, entity* attacker_weapon) {
    retail::sub_6524F0((u32*)this, value, use_signals, (i32)attacker, (i32)attacker_weapon);
}

// sub_83D9A0
void damage_interface::set_armor_points(i32 value) {
    armor_points.set(value);
}

// sub_636850
void damage_interface::set_max_armor_points(i32 max) {
    armor_points.set_max(max);

    if (armor_points.get() > armor_points.get_max())
        armor_points.set(armor_points.get_max());
}

// sub_40A880
bool damage_interface::is_stunned() const {
    i32 hp = hit_points.get();

    return hp > 0 && stun && (hp <= stun->stun_hp || (flags & damage_ifc_flag_stunned));
}

// sub_5FE170
bool damage_interface::is_life_bar_hidden() const {
    return life_bar && (ext_flags & damage_ifc_ext_flag_life_bar_hidden);
}

// sub_5FE190
void damage_interface::set_life_bar_hidden(bool hidden) {
    if (hidden)
        ext_flags |= damage_ifc_ext_flag_life_bar_hidden;
    else
        ext_flags &= ~damage_ifc_ext_flag_life_bar_hidden;
}

// sub_5FE660
void damage_interface::cancel_web_subdue() {
    if (!web_subdue)
        return;

    web_subdue->web_hit_points    = web_subdue->max_web_hit_points;
    web_subdue->recover_remaining = web_subdue->recover_duration;
}

// sub_60A4A0
void damage_interface::allocate_webcast() {
    if (webcast)
        return;

    ext_flags |= damage_ifc_ext_flag_has_webcast;

    auto allocation = (damage_webcast*)mm::allocate(sizeof(damage_webcast));

    webcast = allocation ? new (allocation) damage_webcast() : nullptr;
}

// sub_621880
void damage_interface::set_webcast_level(f32 level) {
    allocate_webcast();

    webcast->level = level;
}

// sub_6218A0
void damage_interface::set_webcast_style(i32 style) {
    allocate_webcast();

    webcast->style = style;
}

// sub_6218C0
void damage_interface::set_webcast_sync_enabled(bool enabled) {
    allocate_webcast();

    webcast->sync_enabled = enabled;
}

// sub_6218E0
void damage_interface::set_webcast_counter_chance(f32 chance, bool is_override) {
    allocate_webcast();

    if (is_override)
        webcast->counter_chance_override = chance;
    else
        webcast->counter_chance = chance;
}

// sub_621920
void damage_interface::set_webcast_counter_scale(f32 scale) {
    allocate_webcast();

    webcast->counter_scale = scale;
}

// sub_5FE5F0
void damage_interface::set_damage_scale(i32 type, f32 scale) {
    damage_scales[type] = scale;
}

// sub_5FE610
void damage_interface::set_damage_scale(f32 scale) {
    damage_scales[0] = scale;
    damage_scales[1] = scale;
    damage_scales[2] = scale;
    damage_scales[3] = scale;
    damage_scales[4] = scale;
}

// sub_5FE650
f32 damage_interface::get_damage_scale(i32 type) const {
    return damage_scales[type];
}

// sub_6095B0
entity* damage_interface::get_damage_attacker() const {
    return (entity*)dmg_info.attacker.resolve();
}

i32 damage_interface::apply_damage(      entity*           attacker,
                                         entity*           attacker_weapon,
                                         i32               damage,
                                         f32               web_damage,
                                         i32               experience_kind,
                                         i32               type,
                                   const hit_react_record* react,
                                   const hit_react_record* second_react,
                                   const vector3d*         loc,
                                   const vector3d*         dir,
                                         i32               dmg_flags,
                                   const string_hash*      wounded_category,
                                   const vector3d*         override_dir,
                                         e_phys_bones      bone_idx,
                                   const combo_move_data*  combo_move,
                                         u8*               applied,
                                         bool              force_react) {

    return ((i32 (__thiscall*)(damage_interface*,
                               entity*,
                               entity*,
                               i32,
                               f32,
                               i32,
                               i32,
                               const hit_react_record*,
                               const hit_react_record*,
                               const vector3d*,
                               const vector3d*,
                               i32,
                               const string_hash*,
                               const vector3d*,
                               e_phys_bones,
                               const combo_move_data*,
                               u8*,
                               bool))retail::sub_675490)(this,
                                                         attacker,
                                                         attacker_weapon,
                                                         damage,
                                                         web_damage,
                                                         experience_kind,
                                                         type,
                                                         react,
                                                         second_react,
                                                         loc,
                                                         dir,
                                                         dmg_flags,
                                                         wounded_category,
                                                         override_dir,
                                                         bone_idx,
                                                         combo_move,
                                                         applied,
                                                         force_react);
}

i32 damage_interface::apply_damage_with_combo_web_damage(      entity*           attacker,
                                                               entity*           attacker_weapon,
                                                               i32               damage,
                                                               i32               experience_kind,
                                                               i32               type,
                                                         const hit_react_record* react,
                                                         const hit_react_record* second_react,
                                                         const vector3d*         loc,
                                                         const vector3d*         dir,
                                                               i32               dmg_flags,
                                                         const string_hash*      wounded_category,
                                                         const vector3d*         override_dir,
                                                               e_phys_bones      bone_idx,
                                                         const combo_move_data*  combo_move,
                                                               u8*               applied,
                                                               bool              force_react) {

    return ((i32 (__thiscall*)(damage_interface*,
                               entity*,
                               entity*,
                               i32,
                               i32,
                               i32,
                               const hit_react_record*,
                               const hit_react_record*,
                               const vector3d*,
                               const vector3d*,
                               i32,
                               const string_hash*,
                               const vector3d*,
                               e_phys_bones,
                               const combo_move_data*,
                               u8*,
                               bool))retail::sub_6798D0)(this,
                                                         attacker,
                                                         attacker_weapon,
                                                         damage,
                                                         experience_kind,
                                                         type,
                                                         react,
                                                         second_react,
                                                         loc,
                                                         dir,
                                                         dmg_flags,
                                                         wounded_category,
                                                         override_dir,
                                                         bone_idx,
                                                         combo_move,
                                                         applied,
                                                         force_react);
}
