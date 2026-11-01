#include "retail.hh"
#include "game/wds/entity/conglomerate_clone.hh"

using namespace treyarch;

string_hash conglomerate_clone::my_type_key         { 0xCF5BE1D0 }; // "conglomerate_clone"
string_hash dynamic_conglomerate_clone::my_type_key { 0xFAECB914 }; // "dynamic_conglomerate_clone"

// sub_6656E0
conglomerate_clone::~conglomerate_clone() {
    if (copy_data) {
        delete copy_data;
        copy_data = nullptr;
    }
}

// slot 1
void conglomerate_clone::construct_mashed_class() {
    ((void (__thiscall*)(conglomerate_clone*))retail::sub_654170)(this);
}

// slot 2
void conglomerate_clone::destruct_mashed_class() {
    ((void (__thiscall*)(conglomerate_clone*))retail::sub_667560)(this);
}

// slot 3
void conglomerate_clone::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    using unmash_method = void (__thiscall*)(conglomerate_clone*, mash::mash_info_struct*, void*, mash::buffer_type);

    ((unmash_method)retail::sub_654190)(this,
                                        mash_info,
                                        containing_class_ptr,
                                        buffer);
}

// slot 4
mash::virtual_types_key conglomerate_clone::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const conglomerate_clone*))retail::sub_665760)(this);
}

// slot 6
bool conglomerate_clone::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const conglomerate_clone*, mash::virtual_types_key))retail::sub_71C790)(this, parent_class);
}

// slot 8
i32 conglomerate_clone::get_mash_sizeof() const {
    return ((i32 (__thiscall*)(const conglomerate_clone*))retail::sub_665770)(this);
}

// slot 14
bool conglomerate_clone::is_a_conglomerate_clone() const {
    return ((bool (__thiscall*)(const conglomerate_clone*))retail::sub_667F80)(this);
}

// slot 77
e_entity_flavor conglomerate_clone::get_flavor() const {
    return ((e_entity_flavor (__thiscall*)(const conglomerate_clone*))retail::sub_A99CD0)(this);
}

// slot 86
void conglomerate_clone::post_unmash_fixup(i32 unk) {
    ((void (__thiscall*)(conglomerate_clone*, i32))retail::sub_62A560)(this, unk);
}

// slot 87
u32 conglomerate_clone::get_entity_size() const {
    return ((u32 (__thiscall*)(const conglomerate_clone*))retail::sub_665770)(this);
}

// slot 95
u32 conglomerate_clone::possibly_render() const {
    return ((u32 (__thiscall*)(const conglomerate_clone*))retail::sub_6151B0)(this);
}

// slot 108
void conglomerate_clone::unk_108(f32 unk) {
    ((void (__thiscall*)(conglomerate_clone*, f32))retail::sub_6398E0)(this, unk);
}

// slot 113
f32 conglomerate_clone::get_visual_radius() const {
    return ((f32 (__thiscall*)(const conglomerate_clone*))retail::sub_615200)(this);
}

// slot 114
vector3d conglomerate_clone::get_visual_center() const {
    vector3d result;
    ((vector3d* (__thiscall*)(const conglomerate_clone*, vector3d*))retail::sub_667F30)(this, &result);

    return result;
}

// slot 158
void conglomerate_clone::set_clone_members_extra_info() {
    ((void (__thiscall*)(conglomerate_clone*))retail::sub_5B4A70)(this);
}

// sub_667600
dynamic_conglomerate_clone::~dynamic_conglomerate_clone() {
    finalize(mash::ALLOCATED);
}

void dynamic_conglomerate_clone::finalize(mash::allocation_scope scope) {
    ((void (__thiscall*)(dynamic_conglomerate_clone*, mash::allocation_scope))retail::sub_645E40)(this, scope);
}

// slot 1
void dynamic_conglomerate_clone::construct_mashed_class() {
    ((void (__thiscall*)(dynamic_conglomerate_clone*))retail::sub_654220)(this);
}

// slot 2
void dynamic_conglomerate_clone::destruct_mashed_class() {
    ((void (__thiscall*)(dynamic_conglomerate_clone*))retail::sub_6676B0)(this);
}

// slot 3
void dynamic_conglomerate_clone::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    using unmash_method = void (__thiscall*)(dynamic_conglomerate_clone*, mash::mash_info_struct*, void*, mash::buffer_type);

    ((unmash_method)retail::sub_654270)(this,
                                        mash_info,
                                        containing_class_ptr,
                                        buffer);
}

// slot 4
mash::virtual_types_key dynamic_conglomerate_clone::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const dynamic_conglomerate_clone*))retail::sub_667590)(this);
}

// slot 6
bool dynamic_conglomerate_clone::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const dynamic_conglomerate_clone*, mash::virtual_types_key))retail::sub_6675A0)(this, parent_class);
}

// slot 8
i32 dynamic_conglomerate_clone::get_mash_sizeof() const {
    return ((i32 (__thiscall*)(const dynamic_conglomerate_clone*))retail::sub_6675F0)(this);
}

// slot 15
bool dynamic_conglomerate_clone::is_a_dynamic_conglomerate_clone() const {
    return ((bool (__thiscall*)(const dynamic_conglomerate_clone*))retail::sub_667F80)(this);
}

// slot 77
e_entity_flavor dynamic_conglomerate_clone::get_flavor() const {
    return ((e_entity_flavor (__thiscall*)(const dynamic_conglomerate_clone*))retail::sub_A99FF0)(this);
}

// slot 87
u32 dynamic_conglomerate_clone::get_entity_size() const {
    return ((u32 (__thiscall*)(const dynamic_conglomerate_clone*))retail::sub_6675F0)(this);
}

// slot 95
u32 dynamic_conglomerate_clone::possibly_render() const {
    return ((u32 (__thiscall*)(const dynamic_conglomerate_clone*))retail::sub_615420)(this);
}

// slot 96
u32 dynamic_conglomerate_clone::possibly_collide() const {
    return ((u32 (__thiscall*)(const dynamic_conglomerate_clone*))retail::sub_62A5F0)(this);
}

// slot 97
bool dynamic_conglomerate_clone::possibly_camera_collide() const {
    return ((bool (__thiscall*)(const dynamic_conglomerate_clone*))retail::sub_639B10)(this);
}

// slot 98
bool dynamic_conglomerate_clone::possibly_walkable() const {
    return ((bool (__thiscall*)(const dynamic_conglomerate_clone*))retail::sub_639B60)(this);
}

// slot 108
void dynamic_conglomerate_clone::unk_108(f32 unk) {
    ((void (__thiscall*)(dynamic_conglomerate_clone*, f32))retail::sub_639BB0)(this, unk);
}

// slot 148
f32 dynamic_conglomerate_clone::get_colgeom_radius() const {
    return ((f32 (__thiscall*)(const dynamic_conglomerate_clone*))retail::sub_615490)(this);
}

// slot 149
vector3d dynamic_conglomerate_clone::get_colgeom_center() const {
    vector3d result;
    ((vector3d* (__thiscall*)(const dynamic_conglomerate_clone*, vector3d*))retail::sub_615510)(this, &result);

    return result;
}

// slot 158
void dynamic_conglomerate_clone::set_clone_members_extra_info() {
    ((void (__thiscall*)(dynamic_conglomerate_clone*))retail::sub_654480)(this);
}
