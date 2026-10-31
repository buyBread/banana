#include "retail.hh"
#include "game/wds/entity/interface/thrown_weapon_interface.hh"

using namespace treyarch;

// slot 1
void thrown_weapon_interface::construct_mashed_class() {
    ((void (__thiscall*)(thrown_weapon_interface*))retail::sub_65D280)(this);
}

// slot 2
void thrown_weapon_interface::destruct_mashed_class() {
    ((void (__thiscall*)(thrown_weapon_interface*))retail::sub_65D340)(this);
}

// slot 3
void thrown_weapon_interface::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(thrown_weapon_interface*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_641770)(this,
                                                                                                                          mash_info,
                                                                                                                          containing_class_ptr,
                                                                                                                          buffer);
}

// slot 4
mash::virtual_types_key thrown_weapon_interface::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const thrown_weapon_interface*))retail::sub_65D130)(this);
}

// slot 6
bool thrown_weapon_interface::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const thrown_weapon_interface*, mash::virtual_types_key))retail::sub_65D140)(this, parent_class);
}

// slot 8
i32 thrown_weapon_interface::get_mash_sizeof() const {
    return ((i32 (__thiscall*)(const thrown_weapon_interface*))retail::sub_65D180)(this);
}

// slot 16
e_entity_ifc thrown_weapon_interface::get_ifc_id() const {
    return ((e_entity_ifc (__thiscall*)(const thrown_weapon_interface*))retail::sub_A70E70)(this);
}

// slot 18
void thrown_weapon_interface::set_owner(entity* owner) {
    ((void (__thiscall*)(thrown_weapon_interface*, entity*))retail::sub_672E90)(this, owner);
}

// slot 19
i32 thrown_weapon_interface::unk_019(void* unk) {
    return ((i32 (__thiscall*)(thrown_weapon_interface*, void*))retail::sub_604CB0)(this, unk);
}

// slot 20
i32 thrown_weapon_interface::unk_020() {
    return ((i32 (__thiscall*)(thrown_weapon_interface*))retail::sub_65D0B0)(this);
}

// slot 21
i32 thrown_weapon_interface::unk_021() {
    return ((i32 (__thiscall*)(thrown_weapon_interface*))retail::sub_65D0C0)(this);
}

// slot 22
i32 thrown_weapon_interface::unk_022() {
    return ((i32 (__thiscall*)(thrown_weapon_interface*))retail::sub_65D0D0)(this);
}

// slot 23
i32 thrown_weapon_interface::unk_023() {
    return ((i32 (__thiscall*)(thrown_weapon_interface*))retail::sub_65D0E0)(this);
}

// slot 24
i32 thrown_weapon_interface::unk_024() {
    return ((i32 (__thiscall*)(thrown_weapon_interface*))retail::sub_65D0F0)(this);
}

// slot 25
i32 thrown_weapon_interface::unk_025() {
    return ((i32 (__thiscall*)(thrown_weapon_interface*))retail::sub_65D100)(this);
}

// slot 26
i32 thrown_weapon_interface::unk_026(i32 unk, i32 unk_2, i32 unk_3) {
    return ((i32 (__thiscall*)(thrown_weapon_interface*, i32, i32, i32))retail::sub_61B510)(this, unk, unk_2, unk_3);
}

// slot 27
i32 thrown_weapon_interface::unk_027(i32 unk) {
    return ((i32 (__thiscall*)(thrown_weapon_interface*, i32))retail::sub_65D110)(this, unk);
}

// slot 28
void* thrown_weapon_interface::spawn_grenade(bool unk, void* unk_2, void* unk_3, f32 unk_4, bool unk_5, i32 unk_6) {
    return ((void* (__thiscall*)(thrown_weapon_interface*, bool, void*, void*, f32, bool, i32))retail::sub_66CE80)(this,
                                                                                                                   unk,
                                                                                                                   unk_2,
                                                                                                                   unk_3,
                                                                                                                   unk_4,
                                                                                                                   unk_5,
                                                                                                                   unk_6);
}
