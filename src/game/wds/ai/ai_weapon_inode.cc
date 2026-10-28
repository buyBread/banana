#include "retail.hh"
#include "game/wds/ai/ai_weapon_inode.hh"
#include "game/arch_base.hh"

using namespace treyarch;

bool treyarch::fire_weapon_entity(entity*            shooter,
                                  arch_base_vhandle  weapon,
                                  e_weapon_fire_kind kind,
                                  bool               if_empty,
                                  arch_base_vhandle  target,
                                  vector3d           position) {

    using fire_weapon_entity_t = bool (__cdecl*)(entity*, arch_base_vhandle, e_weapon_fire_kind, bool, arch_base_vhandle, vector3d);

    return ((fire_weapon_entity_t)retail::sub_4E8C30)(shooter, weapon, kind, if_empty, target, position);
}

e_ai_weapon_state ai_weapon::get_state() const {
    return (e_ai_weapon_state)retail::sub_4E7A40((u8*)this);
}

// sub_4ECBE0
bool ai_weapon::fire(entity*            shooter,
                     ai_weapon*         weapon,
                     e_weapon_fire_kind kind,
                     bool               if_empty,
                     arch_base_vhandle  target,
                     vector3d           position) {

    if (!weapon || !weapon->has_weapon_entity())
        return false;

    return fire_weapon_entity(shooter, weapon->weapon_entity.resolve()->my_handle, kind, if_empty, target, position);
}

// sub_4E7C20
e_ai_weapon_state ai_weapon_inode::get_hand_state(i32 hand) {
    engine_lock_scope lock(&weapon_lock);

    ai_weapon* weapon = drawn_weapon[hand];

    return weapon && weapon->has_weapon_entity() ? weapon->get_state() : ai_weapon_holstered;
}

// sub_4E7D50
ai_weapon* ai_weapon_inode::get_drawn_weapon(i32 hand) {
    engine_lock_scope lock(&weapon_lock);

    ai_weapon* weapon = drawn_weapon[hand];

    return weapon && weapon->has_weapon_entity() ? weapon : nullptr;
}

bool ai_weapon_inode::is_weapon_available(const string_hash &name) {
    return retail::sub_4EC020((u32*)this, (u32*)&name);
}

ai_weapon* ai_weapon_inode::find_weapon(const string_hash &name) {
    return (ai_weapon*)retail::sub_4EC170((u32*)this, (u32*)&name);
}

void ai_weapon_inode::enable_weapon(const string_hash &name, bool enable) {
    retail::sub_4F0C60((u32*)this, (u32*)&name, enable);
}

void ai_weapon_inode::enable_all_weapons(bool enable) {
    retail::sub_4F0D60((u32*)this, enable);
}

void ai_weapon_inode::draw_weapon(const string_hash &name, const string_hash &other_name, bool immediate) {
    retail::sub_4EC840((u32*)this, (u32*)&name, (u32*)&other_name, immediate);
}

void ai_weapon_inode::holster_weapon(const string_hash &name, const string_hash &other_name, bool immediate) {
    retail::sub_4EC410((u32*)this, (u32*)&name, (u32*)&other_name, immediate);
}
