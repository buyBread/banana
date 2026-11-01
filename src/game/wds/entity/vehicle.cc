#include "retail.hh"
#include "game/wds/entity/vehicle.hh"

using namespace treyarch;

string_hash vehicle::my_type_key { 0x6E864420 }; // "vehicle"

void vehicle_unk_1e4::finalize(mash::allocation_scope scope) {
    ((void (__thiscall*)(vehicle_unk_1e4*, mash::allocation_scope))retail::sub_901A00)(this, scope);
}

// sub_681E20
vehicle::~vehicle() {
    ((void (__thiscall*)(u8*, mash::allocation_scope))retail::sub_689610)(unk_2e4, mash::ALLOCATED); // unk_2e4's destructor
}

// slot 1
void vehicle::construct_mashed_class() {
    ((void (__thiscall*)(vehicle*))retail::sub_682020)(this);
}

// slot 2
void vehicle::destruct_mashed_class() {
    ((void (__thiscall*)(vehicle*))retail::sub_668150)(this);
}

// slot 3
void vehicle::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(vehicle*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_65C3C0)(this,
                                                                                                          mash_info,
                                                                                                          containing_class_ptr,
                                                                                                          buffer);
}

// slot 4
mash::virtual_types_key vehicle::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const vehicle*))retail::sub_681E00)(this);
}

// slot 6
bool vehicle::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const vehicle*, mash::virtual_types_key))retail::sub_667DA0)(this, parent_class);
}

// slot 8
i32 vehicle::get_mash_sizeof() const {
    return ((i32 (__thiscall*)(const vehicle*))retail::sub_681E10)(this);
}

// slot 41
bool vehicle::is_a_vehicle() const {
    return ((bool (__thiscall*)(const vehicle*))retail::sub_667F80)(this);
}

// slot 78
bool vehicle::is_a_placeholder_vehicle() const {
    return ((bool (__thiscall*)(const vehicle*))retail::sub_71F870)(this);
}

// slot 105
void vehicle::frame_advance(f32 t) {
    ((void (__thiscall*)(vehicle*, f32))retail::sub_67C440)(this, t);
}

void vehicle::set_model(string_hash name) {
    retail::sub_6569F0((i32)this, name.source_hash_code);
}

void vehicle::set_mass(f32 mass) {
    retail::sub_6325F0((i32)this, mass);
}

void vehicle::enable_physics(bool enable) {
    retail::sub_63BF20((i32)this, enable);
}

void vehicle::set_condition(i32 condition) {
    retail::sub_6048C0((i32)this, condition);
}

// sub_6046D0
void vehicle::set_self_damage(f32 damage, f32 unk_a, f32 unk_b, f32 unk_c) {
    self_damage = damage;
    unk_240     = 0.0f > unk_a ? damage : unk_a;
    unk_244     = 0.0f > unk_b ? 0.0f   : unk_b;
    unk_248     = 0.0f > unk_c ? 0.0f   : unk_b; // unk_b, not unk_c
}

// sub_619890
void vehicle::set_other_damage(f32 damage, f32 unk_a, f32 unk_b, f32 unk_c) {
    other_damage = damage;
    unk_250      = damage > unk_a ? damage : unk_a;
    unk_254      = unk_b;
    unk_258      = unk_b > unk_c ? unk_b : unk_c;
}

// sub_619A80
void vehicle::damage_member(i32 member_idx) {
    if (!is_visible())
        return;

    my_undamaged_members.array[member_idx]->set_visible(false, false);
    my_undamaged_members.array[member_idx]->set_active(false);
    my_damaged_members  .array[member_idx]->set_visible(true, false);
    my_damaged_members  .array[member_idx]->set_active(true);
}

// sub_619B10
void vehicle::damage_member(string_hash member_id) {
    for (u32 i = 0; i < my_undamaged_members.size; i++) {
        if (my_undamaged_members.array[i]->id == member_id) {
            damage_member((i32)i);
            return;
        }
    }
}

// sub_619B60
void vehicle::damage_all_members() {
    for (u32 i = 0; i < my_undamaged_members.size; i++)
        damage_member((i32)i);
}

// sub_619B90
void vehicle::repair_member(i32 member_idx) {
    my_undamaged_members.array[member_idx]->set_visible(true, false);
    my_undamaged_members.array[member_idx]->set_active(true);
    my_damaged_members  .array[member_idx]->set_visible(false, false);
    my_damaged_members  .array[member_idx]->set_active(false);
}

// sub_619C00
void vehicle::repair_member(string_hash member_id) {
    for (u32 i = 0; i < my_undamaged_members.size; i++) {
        if (my_undamaged_members.array[i]->id == member_id) {
            repair_member((i32)i);
            return;
        }
    }
}

// sub_619C50
void vehicle::repair_all_members() {
    for (u32 i = 0; i < my_undamaged_members.size; i++)
        repair_member((i32)i);
}
