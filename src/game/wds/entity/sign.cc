#include "retail.hh"
#include "game/wds/entity/sign.hh"

using namespace treyarch;

string_hash sign::my_type_key { 0x0040DBF1 }; // "sign"

// sub_6680B0
sign::~sign() {
    ((mash::string*)unk_0e8)->finalize(mash::ALLOCATED);
}

// slot 1
void sign::construct_mashed_class() {
    ((void (__thiscall*)(sign*))retail::sub_656910)(this);
}

// slot 2
void sign::destruct_mashed_class() {
    ((void (__thiscall*)(sign*))retail::sub_668130)(this);
}

// sub_64F220
void sign::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    actor::unmash(mash_info, containing_class_ptr, buffer);

    ((mash::string*)unk_0e8)->unmash(mash_info, this, buffer);
}

// slot 4
mash::virtual_types_key sign::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const sign*))retail::sub_6680A0)(this);
}

// slot 6
bool sign::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const sign*, mash::virtual_types_key))retail::sub_71C790)(this, parent_class);
}

// slot 8
i32 sign::get_mash_sizeof() const {
    return ((i32 (__thiscall*)(const sign*))retail::sub_668090)(this);
}

// slot 21
bool sign::is_a_sign() const {
    return ((bool (__thiscall*)(const sign*))retail::sub_667F80)(this);
}

// slot 77
e_entity_flavor sign::get_flavor() const {
    return ((e_entity_flavor (__thiscall*)(const sign*))retail::sub_668080)(this);
}

// slot 87
u32 sign::get_entity_size() const {
    return ((u32 (__thiscall*)(const sign*))retail::sub_668090)(this);
}
