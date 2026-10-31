#include "retail.hh"
#include "game/wds/entity/interface/generic_interface.hh"

using namespace treyarch;

// slot 9
bool generic_interface::get_ifc_num(const string_hash &name, f32* value, bool document) {
    return ((bool (__thiscall*)(generic_interface*, const string_hash&, f32*, bool))retail::sub_5FCC20)(this, name, value, document);
}

// slot 10
bool generic_interface::set_ifc_num(const string_hash &name, f32 value, bool document) {
    return ((bool (__thiscall*)(generic_interface*, const string_hash&, f32, bool))retail::sub_5FCC20)(this, name, value, document);
}

// slot 11
bool generic_interface::get_ifc_vec(const string_hash &name, vector3d* value, bool document) {
    return ((bool (__thiscall*)(generic_interface*, const string_hash&, vector3d*, bool))retail::sub_5FCC20)(this, name, value, document);
}

// slot 12
bool generic_interface::set_ifc_vec(const string_hash &name, const vector3d &value, bool document) {
    return ((bool (__thiscall*)(generic_interface*, const string_hash&, const vector3d&, bool))retail::sub_5FCC20)(this, name, value, document);
}

// slot 13
bool generic_interface::get_ifc_str(const string_hash &name, const char** value, bool document) {
    return ((bool (__thiscall*)(generic_interface*, const string_hash&, const char**, bool))retail::sub_5FCC20)(this, name, value, document);
}

// slot 14
bool generic_interface::set_ifc_str(const string_hash &name, const char* value, bool document) {
    return ((bool (__thiscall*)(generic_interface*, const string_hash&, const char*, bool))retail::sub_5FCC20)(this, name, value, document);
}

// slot 15
const char* actor_interface::get_ifc_type_str() const {
    return ((const char* (__thiscall*)(const actor_interface*))retail::sub_5FCC40)(this);
}

// slot 17
e_entity_interface_base actor_interface::get_base_type() {
    return ((e_entity_interface_base (__thiscall*)(actor_interface*))retail::sub_719110)(this);
}

// slot 18
void actor_interface::set_owner(actor* owner) {
    ((void (__thiscall*)(actor_interface*, actor*))retail::sub_5FCB10)(this, owner);
}

// slot 15
const char* entity_interface::get_ifc_type_str() const {
    return ((const char* (__thiscall*)(const entity_interface*))retail::sub_5FCC30)(this);
}

// slot 17
e_entity_interface_base entity_interface::get_base_type() {
    return ((e_entity_interface_base (__thiscall*)(entity_interface*))retail::sub_8139C0)(this);
}

// slot 18
void entity_interface::set_owner(entity* owner) {
    ((void (__thiscall*)(entity_interface*, entity*))retail::sub_5FCB10)(this, owner);
}
