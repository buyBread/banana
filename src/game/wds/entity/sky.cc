#include "retail.hh"
#include "game/wds/entity/sky.hh"

using namespace treyarch;

string_hash sky::my_type_key { 0x0001F777 }; // "sky"

// sub_71C720
sky::~sky() {
    finalize(mash::ALLOCATED);
}

void sky::finalize(mash::allocation_scope scope) {
    ((void (__thiscall*)(sky*, mash::allocation_scope))retail::sub_72CD50)(this, scope);
}

// slot 1
void sky::construct_mashed_class() {
    ((void (__thiscall*)(sky*))retail::sub_738A10)(this);
}

// slot 2
void sky::destruct_mashed_class() {
    ((void (__thiscall*)(sky*))retail::sub_738A20)(this);
}

// slot 3
void sky::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(sky*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_72CD40)(this,
                                                                                                      mash_info,
                                                                                                      containing_class_ptr,
                                                                                                      buffer);
}

// slot 4
mash::virtual_types_key sky::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const sky*))retail::sub_71C780)(this);
}

// slot 6
bool sky::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const sky*, mash::virtual_types_key))retail::sub_71C790)(this, parent_class);
}

// slot 20
bool sky::is_a_sky() const {
    return ((bool (__thiscall*)(const sky*))retail::sub_667F80)(this);
}

// slot 77
e_entity_flavor sky::get_flavor() const {
    return ((e_entity_flavor (__thiscall*)(const sky*))retail::sub_610970)(this);
}
