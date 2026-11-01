#include "retail.hh"
#include "game/trigger.hh"
#include "game/water_box.hh"

using namespace treyarch;

string_hash water_box::my_type_key { 0x800B70CB }; // "water_box"

// sub_668300
water_box::~water_box() {
    if (m_trigger) {
        delete m_trigger;
        m_trigger = nullptr;
    }

    if (unk_0ec) {
        delete unk_0ec;
        unk_0ec = nullptr;
    }
}

// slot 1
void water_box::construct_mashed_class() {
    ((void (__thiscall*)(water_box*))retail::sub_6572E0)(this);
}

// slot 2
void water_box::destruct_mashed_class() {
    ((void (__thiscall*)(water_box*))retail::sub_6683B0)(this);
}

// slot 3
void water_box::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(water_box*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_64F250)(this,
                                                                                                            mash_info,
                                                                                                            containing_class_ptr,
                                                                                                            buffer);
}

// slot 4
mash::virtual_types_key water_box::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const water_box*))retail::sub_6682E0)(this);
}

// slot 6
bool water_box::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const water_box*, mash::virtual_types_key))retail::sub_71C790)(this, parent_class);
}

// slot 8
i32 water_box::get_mash_sizeof() const {
    return ((i32 (__thiscall*)(const water_box*))retail::sub_6682F0)(this);
}

// slot 42
bool water_box::is_a_water_box() const {
    return ((bool (__thiscall*)(const water_box*))retail::sub_667F80)(this);
}

// slot 86
void water_box::post_unmash_fixup(i32 unk) {
    ((void (__thiscall*)(water_box*, i32))retail::sub_65CAD0)(this, unk);
}

// slot 87
u32 water_box::get_entity_size() const {
    return ((u32 (__thiscall*)(const water_box*))retail::sub_6682F0)(this);
}

// slot 158
bool water_box::point_inside(const vector3d &point) {
    return ((bool (__thiscall*)(water_box*, const vector3d&))retail::sub_604960)(this, point);
}

// slot 159
bool water_box::unk_159(const vector3d &point) {
    return ((bool (__thiscall*)(water_box*, const vector3d&))retail::sub_604980)(this, point);
}

// slot 160
bool water_box::surface_point_inside(const vector3d &point) {
    return ((bool (__thiscall*)(water_box*, const vector3d&))retail::sub_6049A0)(this, point);
}

// slot 161
bool water_box::unk_161(const vector3d &point) {
    return ((bool (__thiscall*)(water_box*, const vector3d&))retail::sub_6049C0)(this, point);
}
