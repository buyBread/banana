#include "retail.hh"
#include "game/wds/entity/entity_base.hh"

using namespace treyarch;

// sub_4E1D40
entity_base::~entity_base() {
    finalize(mash::ALLOCATED);

    ((void (__thiscall*)(entity**, mash::allocation_scope))retail::sub_689610)(&my_parent, mash::ALLOCATED); // my_parent's destructor
}

void entity_base::finalize(mash::allocation_scope scope) {
    ((void (__thiscall*)(entity_base*, mash::allocation_scope))retail::sub_660F10)(this, scope);
}

// slot 1
void entity_base::construct_mashed_class() {
    ((void (__thiscall*)(entity_base*))retail::sub_653CB0)(this);
}

// slot 2
void entity_base::destruct_mashed_class() {
    ((void (__thiscall*)(entity_base*))retail::sub_666A80)(this);
}

// slot 3
void entity_base::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(entity_base*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_64D140)(this,
                                                                                                               mash_info,
                                                                                                               containing_class_ptr,
                                                                                                               buffer);
}

// slot 4
mash::virtual_types_key entity_base::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const entity_base*))retail::sub_4E1EA0)(this);
}

// slot 6
bool entity_base::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const entity_base*, mash::virtual_types_key))retail::sub_4E1EB0)(this, parent_class);
}

// slot 8
i32 entity_base::get_mash_sizeof() const {
    return ((i32 (__thiscall*)(const entity_base*))retail::sub_4E1E90)(this);
}

// slot 10
bool entity_base::is_an_entity_base() const {
    return ((bool (__thiscall*)(const entity_base*))retail::sub_667F80)(this);
}

// slot 53
vector3d entity_base::get_last_position() const {
    vector3d result;
    ((vector3d* (__thiscall*)(const entity_base*, vector3d*))retail::sub_667F30)(this, &result);

    return result;
}

// slot 54
i32 entity_base::po_changed() {
    return ((i32 (__thiscall*)(entity_base*))retail::sub_A944D0)(this);
}

// slot 55
i32 entity_base::unk_055(i32 unk, i32 unk_2, i32 unk_3, i32 unk_4, bool unk_5, bool unk_6, bool unk_7) {
    return ((i32 (__thiscall*)(entity_base*, i32, i32, i32, i32, bool, bool, bool))retail::sub_64AFE0)(this, unk, unk_2, unk_3, unk_4, unk_5, unk_6, unk_7);
}

// slot 56
u32 entity_base::spawn_effect_at_entity_position(string_hash name, const vector4d &position, i32 unk) {
    return ((u32 (__thiscall*)(entity_base*, string_hash, const vector4d&, i32))retail::sub_611F70)(this, name, position, unk);
}

// slot 57
u32 entity_base::spawn_effect(string_hash name, i32 unk) {
    return ((u32 (__thiscall*)(entity_base*, string_hash, i32))retail::sub_611D00)(this, name, unk);
}

// slot 58
u32 entity_base::spawn_effect_at_world_position(string_hash name, const vector4d &position, i32 unk) {
    return ((u32 (__thiscall*)(entity_base*, string_hash, const vector4d&, i32))retail::sub_612060)(this, name, position, unk);
}

// slot 59
u32 entity_base::unk_059(string_hash name, i32 unk) {
    return ((u32 (__thiscall*)(entity_base*, string_hash, i32))retail::sub_611DD0)(this, name, unk);
}

// slot 60
u32 entity_base::spawn_looping_effect_at_entity_position(string_hash name, const vector4d &position, i32 unk) {
    return ((u32 (__thiscall*)(entity_base*, string_hash, const vector4d&, i32))retail::sub_612240)(this, name, position, unk);
}

// slot 61
u32 entity_base::spawn_looping_effect(string_hash name, i32 unk) {
    return ((u32 (__thiscall*)(entity_base*, string_hash, i32))retail::sub_611EA0)(this, name, unk);
}

// slot 62
u32 entity_base::unk_062(string_hash name, const vector4d &position, i32 unk) {
    return ((u32 (__thiscall*)(entity_base*, string_hash, const vector4d&, i32))retail::sub_612150)(this, name, position, unk);
}

// slot 63
void entity_base::stop_looping_effect(u32 effect_id) {
    ((void (__thiscall*)(entity_base*, u32))retail::sub_626DA0)(this, effect_id);
}

// slot 64
void entity_base::set_flag_recursive(u32 flag, bool on) {
    ((void (__thiscall*)(entity_base*, u32, bool))retail::sub_4E1DB0)(this, flag, on);
}

// slot 65
void entity_base::set_ext_flag_recursive(u32 flag, bool on) {
    ((void (__thiscall*)(entity_base*, u32, bool))retail::sub_4E1DE0)(this, flag, on);
}

// slot 66
void entity_base::set_active(bool active) {
    ((void (__thiscall*)(entity_base*, bool))retail::sub_4E1E10)(this, active);
}

// slot 67
void entity_base::set_visible(bool visible, bool from_conglom) {
    ((void (__thiscall*)(entity_base*, bool, bool))retail::sub_4E1E30)(this, visible, from_conglom);
}

// slot 68
collision_geometry* entity_base::get_colgeom() const {
    return ((collision_geometry* (__thiscall*)(const entity_base*))retail::sub_A944D0)(this);
}

// slot 69
void entity_base::set_collisions_active(bool active, bool recursive) {
    ((void (__thiscall*)(entity_base*, bool, bool))retail::sub_AD2D40)(this, active, recursive);
}

// slot 70
bool entity_base::are_character_collisions_active() const {
    return ((bool (__thiscall*)(const entity_base*))retail::sub_71F870)(this);
}

// slot 71
bool entity_base::are_moving_terrain_only_collisions_active() const {
    return ((bool (__thiscall*)(const entity_base*))retail::sub_71F870)(this);
}

// slot 72
void entity_base::set_character_collisions_active(bool active) {
    ((void (__thiscall*)(entity_base*, bool))retail::sub_4E1E50)(this, active);
}

// slot 73
void entity_base::set_terrain_collisions_active(bool active) {
    ((void (__thiscall*)(entity_base*, bool))retail::sub_4E1E70)(this, active);
}

// slot 74
void entity_base::set_moving_terrain_only_collisions_active(bool active) {
    ((void (__thiscall*)(entity_base*, bool))retail::sub_689610)(this, active);
}

// slot 75
bool entity_base::was_occluded_last_frame() {
    return ((bool (__thiscall*)(entity_base*))retail::sub_667F80)(this);
}

// slot 76
bool entity_base::is_alive() const {
    return ((bool (__thiscall*)(const entity_base*))retail::sub_667F80)(this);
}

// slot 77
e_entity_flavor entity_base::get_flavor() const {
    return ((e_entity_flavor (__thiscall*)(const entity_base*))retail::sub_8139C0)(this);
}

// slot 78
bool entity_base::is_a_placeholder_vehicle() const {
    return ((bool (__thiscall*)(const entity_base*))retail::sub_71F870)(this);
}

// slot 79
bool entity_base::get_ifc_num(const string_hash &name, f32* value, bool document) {
    return ((bool (__thiscall*)(entity_base*, const string_hash&, f32*, bool))retail::sub_612510)(this, name, value, document);
}

// slot 80
bool entity_base::set_ifc_num(const string_hash &name, f32 value, bool document) {
    return ((bool (__thiscall*)(entity_base*, const string_hash&, f32, bool))retail::sub_612620)(this, name, value, document);
}

// slot 81
bool entity_base::get_ifc_vec(const string_hash &name, vector3d* value, bool document) {
    return ((bool (__thiscall*)(entity_base*, const string_hash&, vector3d*, bool))retail::sub_612730)(this, name, value, document);
}

// slot 82
bool entity_base::set_ifc_vec(const string_hash &name, const vector3d &value, bool document) {
    return ((bool (__thiscall*)(entity_base*, const string_hash&, const vector3d&, bool))retail::sub_612840)(this, name, value, document);
}

// slot 83
bool entity_base::get_ifc_str(const string_hash &name, const char** value, bool document) {
    return ((bool (__thiscall*)(entity_base*, const string_hash&, const char**, bool))retail::sub_612950)(this, name, value, document);
}

// slot 84
bool entity_base::set_ifc_str(const string_hash &name, const char* value, bool document) {
    return ((bool (__thiscall*)(entity_base*, const string_hash&, const char*, bool))retail::sub_612A60)(this, name, value, document);
}

// slot 85
info_node* entity_base::get_voice() {
    return ((info_node* (__thiscall*)(entity_base*))retail::sub_611A90)(this);
}

// slot 86
void entity_base::post_unmash_fixup(i32 unk) {
    ((void (__thiscall*)(entity_base*, i32))retail::sub_627160)(this, unk);
}

// slot 87
u32 entity_base::get_entity_size() const {
    return ((u32 (__thiscall*)(const entity_base*))retail::sub_4E1E90)(this);
}

// sub_6267E0
generic_interface* entity_base::find_ifc_by_name(string_hash name) {
    if (!my_ifc_storage)
        return nullptr;

    return my_ifc_storage->find_ifc_by_name(name);
}

script_data_interface* entity_base::create_script_data_ifc() {
    return (script_data_interface*)retail::sub_64A8B0((i32*)this);
}

physical_interface* entity_base::create_physical_ifc() {
    return (physical_interface*)retail::sub_660CB0((i32*)this);
}

damage_interface* entity_base::create_damage_ifc() {
    return (damage_interface*)retail::sub_660B80((i32*)this);
}

time_interface* entity_base::create_time_ifc() {
    return (time_interface*)retail::sub_65ABE0((i32*)this);
}
