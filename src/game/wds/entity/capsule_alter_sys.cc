#include "retail.hh"
#include "game/wds/entity/capsule_alter_sys.hh"
#include "treyarch/mash/mash_info.hh"

using namespace treyarch;

void capsule_alter_sys::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type) {
    mash_info->read_from_buffer(mash::SHARED_BUFFER, mode);
}

// sub_7F71F0
void capsule_alter_sys::construct_mashed_class() {
    flags    |= capsule_alter_flag_from_mash;
    prev_mode = mode;
}

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
