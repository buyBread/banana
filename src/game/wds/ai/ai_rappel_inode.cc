#include "retail.hh"
#include "game/wds/ai/ai_rappel_inode.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/conglomerate.hh"
#include "game/math/references.hh"

using namespace treyarch;

// sub_454FC0
f32 ai_rappel_rope::get_constraint() const {
    return constraint;
}

// sub_454FD0
void ai_rappel_rope::set_rope_length(f32 length) {
    rope_length = length;
}

// sub_470A60
void ai_rappel_rope::set_rope_pivot(string_hash pivot) {
    if (rope_pivot != string_hash() && pivot == string_hash())
        return;

    rope_pivot = pivot;

    if (pivot == string_hash())
        return;

    if (!owner->get_actor()->is_a_conglomerate() ||
        !((conglomerate*)owner->get_actor())->has_member_for_linking(rope_pivot))

        rope_pivot = string_hash();
}

// sub_455150
void ai_rappel_inode::set_should_fire_rope(bool fire) {
    my_param_block.set_pb_int(should_fire_rope_key, fire);
}

// sub_4551A0
void ai_rappel_inode::set_desired_attach_pos(vector3d pos) {
    my_param_block.set_pb_vector3d(desired_attach_pos_key, pos);
}

// sub_4551D0
void ai_rappel_inode::set_desired_attach_entity(arch_base_vhandle entity) {
    my_param_block.set_pb_entity(desired_attach_entity_key, entity);
}

// sub_455240
void ai_rappel_inode::set_desired_rope_pivot(string_hash pivot) {
    my_param_block.set_pb_hash(desired_rope_pivot_key, pivot);
}

// sub_455270
void ai_rappel_inode::set_desired_rope_length(f32 length) {
    my_param_block.set_pb_float(desired_rope_length_key, length);
}

// sub_4552A0
void ai_rappel_inode::set_desired_rope_fire_mode(bool fire) {
    my_param_block.set_pb_int(desired_rope_fire_mode_key, fire);
}

// sub_4552C0
void ai_rappel_inode::set_desired_fire_speed(f32 fire_speed) {
    my_param_block.set_pb_float(desired_fire_speed_key, fire_speed);
}

// sub_4552F0
void ai_rappel_inode::set_desired_fire_accel(f32 accel) {
    my_param_block.set_pb_float(desired_fire_accel_key, accel);
}

// sub_455320
void ai_rappel_inode::set_desired_fire_curve_rad(f32 rad) {
    my_param_block.set_pb_float(desired_fire_curve_rad_key, rad);
}

// sub_455170
vector3d ai_rappel_inode::get_desired_attach_pos() const {
    return my_param_block.get_optional_pb_vector3d(desired_attach_pos_key, references::unk_00fc3d70.get(), nullptr);
}

// sub_455200
string_hash ai_rappel_inode::get_desired_rope_pivot() const {
    string_hash none;
    none.initialize(mash::ALLOCATED);

    return my_param_block.get_optional_pb_hash(desired_rope_pivot_key, none, nullptr);
}

// sub_424DC0
bool ai_rappel_inode::is_engaged() const {
    return current_rope && current_rope->attached;
}

// sub_482710
void ai_rappel_inode::release_rope(f32 unk_7c, f32 unk_74) {
    if (current_rope) {
        ((void (__thiscall*)(ai_rappel_rope*, bool, string_hash))retail::sub_47ED30)(current_rope, false, string_hash()); // rope attach/pendulum

        current_rope->unk_7c = unk_7c;
        current_rope->unk_74 = unk_74;
        current_rope->unk_78 = unk_74;

        released_ropes.push_back(current_rope);

        current_rope = nullptr;
    }

    speed = 0.0f;
}
