#include "retail.hh"
#include "treyarch/game/wds/entity/capsule_alter_sys.hh"

using namespace treyarch;

// sub_7F7200
void capsule_alter_sys::set_mode(u32 new_mode) {
    ((void (__thiscall*)(capsule_alter_sys*, u32))retail::sub_7F7200)(this, new_mode);
}

// sub_7F7280
void capsule_alter_sys::set_custom_capsule(const vector3d &base, const vector3d &end, f32 radius) {
    using set_custom_capsule_t = void (__thiscall*)(capsule_alter_sys*, const vector3d*, const vector3d*, f32);

    ((set_custom_capsule_t)retail::sub_7F7280)(this, &base, &end, radius);
}

// sub_7EDD20
void capsule_alter_sys::update(bool unk) {
    ((void (__thiscall*)(capsule_alter_sys*, bool))retail::sub_7EDD20)(this, unk);
}
