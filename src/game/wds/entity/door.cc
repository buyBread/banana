#include "retail.hh"
#include "game/wds/entity/door.hh"

using namespace treyarch;

string_hash door::my_type_key { 0x0038BCD4 }; // "door"

// slot 1
void door::construct_mashed_class() {
    ((void (__thiscall*)(door*))retail::sub_662080)(this);
}

// slot 2
void door::destruct_mashed_class() {
    ((void (__thiscall*)(door*))retail::sub_667E30)(this);
}

// slot 3
void door::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(door*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_65BB60)(this,
                                                                                                       mash_info,
                                                                                                       containing_class_ptr,
                                                                                                       buffer);
}

// slot 4
mash::virtual_types_key door::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const door*))retail::sub_667D90)(this);
}

// slot 6
bool door::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const door*, mash::virtual_types_key))retail::sub_667DA0)(this, parent_class);
}

// slot 8
i32 door::get_mash_sizeof() const {
    return ((i32 (__thiscall*)(const door*))retail::sub_667DF0)(this);
}

// slot 34
bool door::is_a_door() const {
    return ((bool (__thiscall*)(const door*))retail::sub_667F80)(this);
}

// slot 86
void door::post_unmash_fixup(i32 unk) {
    ((void (__thiscall*)(door*, i32))retail::sub_66C5F0)(this, unk);
}
