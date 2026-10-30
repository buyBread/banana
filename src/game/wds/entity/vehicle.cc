#include "retail.hh"
#include "game/wds/entity/vehicle.hh"

using namespace treyarch;

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
