#include "retail.hh"
#include "game/wds/entity/conglomerate.hh"
#include "game/binary_search.hh"

using namespace treyarch;

string_hash conglomerate::my_type_key { 0x6DD7D480 }; // "conglomerate"

// sub_665490
conglomerate::~conglomerate() {
    finalize(mash::ALLOCATED);
}

void conglomerate::finalize(mash::allocation_scope scope) {
    ((void (__thiscall*)(conglomerate*, mash::allocation_scope))retail::sub_661D70)(this, scope);
}

// slot 1
void conglomerate::construct_mashed_class() {
    ((void (__thiscall*)(conglomerate*))retail::sub_661C50)(this);
}

// slot 2
void conglomerate::destruct_mashed_class() {
    ((void (__thiscall*)(conglomerate*))retail::sub_667BA0)(this);
}

// slot 3
void conglomerate::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(conglomerate*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_65B5D0)(this,
                                                                                                               mash_info,
                                                                                                               containing_class_ptr,
                                                                                                               buffer);
}

// slot 4
mash::virtual_types_key conglomerate::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const conglomerate*))retail::sub_665660)(this);
}

// slot 6
bool conglomerate::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const conglomerate*, mash::virtual_types_key))retail::sub_71C790)(this, parent_class);
}

// slot 8
i32 conglomerate::get_mash_sizeof() const {
    return ((i32 (__thiscall*)(const conglomerate*))retail::sub_665650)(this);
}

// slot 13
bool conglomerate::is_a_conglomerate() const {
    return ((bool (__thiscall*)(const conglomerate*))retail::sub_667F80)(this);
}

// slot 64
void conglomerate::set_flag_recursive(u32 flag, bool on) {
    ((void (__thiscall*)(conglomerate*, u32, bool))retail::sub_6319A0)(this, flag, on);
}

// slot 65
void conglomerate::set_ext_flag_recursive(u32 flag, bool on) {
    ((void (__thiscall*)(conglomerate*, u32, bool))retail::sub_631A10)(this, flag, on);
}

// slot 67
void conglomerate::set_visible(bool visible, bool from_conglom) {
    ((void (__thiscall*)(conglomerate*, bool, bool))retail::sub_645FD0)(this, visible, from_conglom);
}

// slot 77
e_entity_flavor conglomerate::get_flavor() const {
    return ((e_entity_flavor (__thiscall*)(const conglomerate*))retail::sub_AD6450)(this);
}

// slot 86
void conglomerate::post_unmash_fixup(i32 unk) {
    ((void (__thiscall*)(conglomerate*, i32))retail::sub_6562D0)(this, unk);
}

// slot 87
u32 conglomerate::get_entity_size() const {
    return ((u32 (__thiscall*)(const conglomerate*))retail::sub_665650)(this);
}

// slot 88
void conglomerate::clear_region(region* r, i32 i_know_what_i_am_doing) {
    ((void (__thiscall*)(conglomerate*, region*, i32))retail::sub_63AE70)(this, r, i_know_what_i_am_doing);
}

// slot 90
void conglomerate::force_region(region* r) {
    ((void (__thiscall*)(conglomerate*, region*))retail::sub_6168E0)(this, r);
}

// slot 93
void conglomerate::force_regions(entity* e) {
    ((void (__thiscall*)(conglomerate*, entity*))retail::sub_6168F0)(this, e);
}

// slot 95
u32 conglomerate::possibly_render() const {
    return ((u32 (__thiscall*)(const conglomerate*))retail::sub_631AE0)(this);
}

// slot 96
u32 conglomerate::possibly_collide() const {
    return ((u32 (__thiscall*)(const conglomerate*))retail::sub_617640)(this);
}

// slot 97
bool conglomerate::possibly_camera_collide() const {
    return ((bool (__thiscall*)(const conglomerate*))retail::sub_63AEE0)(this);
}

// slot 98
bool conglomerate::possibly_walkable() const {
    return ((bool (__thiscall*)(const conglomerate*))retail::sub_63AF70)(this);
}

// slot 99
void conglomerate::region_update_poss_collide() {
    ((void (__thiscall*)(conglomerate*))retail::sub_617690)(this);
}

// slot 106
bool conglomerate::is_still_visible() const {
    return ((bool (__thiscall*)(const conglomerate*))retail::sub_62F810)(this);
}

// slot 108
void conglomerate::unk_108(f32 unk) {
    ((void (__thiscall*)(conglomerate*, f32))retail::sub_64DAE0)(this, unk);
}

// slot 109
void conglomerate::render(f32 camera_distance, f32 entity_translucency_pct) {
    ((void (__thiscall*)(conglomerate*, f32, f32))retail::sub_630690)(this, camera_distance, entity_translucency_pct);
}

// slot 113
f32 conglomerate::get_visual_radius() const {
    return ((f32 (__thiscall*)(const conglomerate*))retail::sub_63A830)(this);
}

// slot 114
vector3d conglomerate::get_visual_center() const {
    vector3d result;
    ((vector3d* (__thiscall*)(const conglomerate*, vector3d*))retail::sub_63A660)(this, &result);

    return result;
}

// slot 115
vector3d conglomerate::unk_115() {
    vector3d result;
    ((vector3d* (__thiscall*)(conglomerate*, vector3d*))retail::sub_62F670)(this, &result);

    return result;
}

// slot 118
void conglomerate::set_render_color(color32 color) {
    ((void (__thiscall*)(conglomerate*, color32))retail::sub_6305A0)(this, color);
}

// slot 120
void conglomerate::set_render_alpha_mod(f32 alpha) {
    ((void (__thiscall*)(conglomerate*, f32))retail::sub_630610)(this, alpha);
}

// slot 122
void conglomerate::set_render_scale(const vector3d &scale) {
    ((void (__thiscall*)(conglomerate*, const vector3d&))retail::sub_631C00)(this, scale);
}

// slot 126
void* conglomerate::get_light_set() const {
    return ((void* (__thiscall*)(const conglomerate*))retail::sub_603DB0)(this);
}

// slot 131
void conglomerate::set_recursive_age(f32 age) {
    ((void (__thiscall*)(conglomerate*, f32))retail::sub_631B90)(this, age);
}

// slot 143
void conglomerate::set_colgeom_flag_recursive(u32 flag, bool set) {
    ((void (__thiscall*)(conglomerate*, u32, bool))retail::sub_631A70)(this, flag, set);
}

// slot 148
f32 conglomerate::get_colgeom_radius() const {
    return ((f32 (__thiscall*)(const conglomerate*))retail::sub_63AC20)(this);
}

// slot 149
vector3d conglomerate::get_colgeom_center() const {
    vector3d result;
    ((vector3d* (__thiscall*)(const conglomerate*, vector3d*))retail::sub_665670)(this, &result);

    return result;
}

// slot 150
void conglomerate::radius_changed(bool force) {
    ((void (__thiscall*)(conglomerate*, bool))retail::sub_603C30)(this, force);
}

// slot 158
void conglomerate::unk_158(i32 unk, i32 unk_2) {
    ((void (__thiscall*)(conglomerate*, i32, i32))retail::sub_616A10)(this, unk, unk_2);
}

// slot 159
void conglomerate::frame_done() {
    ((void (__thiscall*)(conglomerate*))retail::sub_665640)(this);
}

// slot 160
void conglomerate::frame_done_including_members() {
    ((void (__thiscall*)(conglomerate*))retail::sub_5B4A70)(this);
}

// sub_62F880
e_conglom_member_index conglomerate::get_member_index(const string_hash &nodename) const {
    // a bone miss leaves its insertion point for the members search
    i32 found = -1;

    if (bone_indices.size() &&
        binary_search_array(nodename, bone_indices.get_vector().array, (i32)bone_indices.size(), &found))

        return (e_conglom_member_index)(members.size + found);

    if (members.size && binary_search_array_deref(nodename, members.array, (i32)members.size, &found))
        return (e_conglom_member_index)found;

    return nodename == id ? base_conglom_index : invalid_conglom_member;
}

// sub_616950
entity* conglomerate::get_member(e_conglom_member_index index) const {
    if (index >= 0 && (u32)index < members.size)
        return members.array[index];

    return index == base_conglom_index ? (entity*)this : nullptr;
}

// sub_6169D0
bool conglomerate::has_member(const string_hash &nodename) const {
    if (!members.size)
        return false;

    i32 index;

    return binary_search_array_deref(nodename, members.array, (i32)members.size, &index);
}

// sub_40A810
bool conglomerate::has_member_for_linking(const string_hash &nodename) const {
    e_conglom_member_index index = get_member_index(nodename);

    return index >= 0 && (u32)index < bone_indices.size() + members.size;
}
