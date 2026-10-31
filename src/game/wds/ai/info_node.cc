#include "retail.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/info_node.hh"
#include "game/math/references.hh"

using namespace treyarch;

// sub_4C3A70
actor* info_node::get_actor() const {
    return my_ai_core->my_actor;
}

// sub_450480
void loco_inode::set_needs_repathfind(bool needs_it, bool latch) {
    needs_repathfind = needs_it;

    if (!repathfind_latch)
        repathfind_latch = latch;
}

// sub_458440
const vector3d &loco_inode::get_goto_dest_pos() const {
    if (unk_064.size)
        return unk_064.array[unk_064.size - 1];

    if (goto_points.size)
        return goto_points.array[goto_points.size - 1];

    return references::unk_00fc3d70.get();
}

// sub_44FAD0
bool loco_inode::is_using_swift() const {
    return my_param_block.get_optional_pb_int(currently_using_swift_key, 0, nullptr) != 0;
}

// sub_44FAB0
void loco_inode::set_flag_stop_movement(bool stop) {
    my_param_block.set_pb_int(flag_stop_movement_key, stop);
}

bool std_carry_inode::add_slave(std_carry_inode* slave, bool engage_slave_mode) {
    return retail::sub_4BEA10((u32*)this, (i32*)slave, engage_slave_mode);
}

void std_carry_inode::remove_slave(std_carry_inode* slave, bool kill_slave_mode) {
    retail::sub_4BA230((u32*)this, (i32)slave, kill_slave_mode);
}

// sub_492510
bool std_carry_inode::get_scaring_informant() const {
    return my_param_block.get_optional_pb_int(scaring_informant_key, 0, nullptr) != 0;
}

// sub_492540
bool std_carry_inode::get_dropping_informant() const {
    return my_param_block.get_optional_pb_int(dropping_informant_key, 0, nullptr) != 0;
}

// sub_4923F0
bool std_carry_inode::get_allow_web_pickup() const {
    return my_param_block.get_optional_pb_int(allow_web_pickup_key, 1, nullptr) != 0;
}

// sub_490FF0
bool std_puppet_inode::get_start_rhino_ride() const {
    return my_param_block.get_optional_pb_int(start_rhino_ride_key, 0, nullptr) != 0;
}

// sub_490DD0
i32 std_puppet_inode::get_puppet_generic_in_service() const {
    return my_param_block.get_optional_pb_int(puppet_generic_in_service_key, 0, nullptr);
}

void std_fear_inode::reset_morale() {
    retail::sub_44EFF0((i32)this);
}

void std_fear_inode::break_morale_force_cower(f32 unk) {
    retail::sub_457530((i32)this, unk);
}

void std_fear_inode::break_morale_force_flee(vector3d position, f32 unk) {
    ((void (__thiscall*)(std_fear_inode*, vector3d, f32))retail::sub_457780)(this, position, unk);
}

void std_fear_inode::break_morale_force_flee(arch_base_vhandle target, f32 unk) {
    ((void (__thiscall*)(std_fear_inode*, arch_base_vhandle, f32))retail::sub_457650)(this, target, unk);
}

bool ai_car_inode::force_enter_car(arch_base_vhandle car, i32 seat, bool animate) {
    return ((bool (__thiscall*)(ai_car_inode*, arch_base_vhandle, i32, bool))retail::sub_4C15F0)(this, car, seat, animate);
}

bool ai_car_inode::force_exit_car(bool animate) {
    return retail::sub_4BFA10((i32)this, animate);
}

void crawl_layer_inode::attach_to_surface(const line_info &lif, const vector3d &desired_forward) {
    retail::sub_45AC10((u8*)this, (i32)&lif, (f32*)&desired_forward);
}

void crawl_layer_inode::release_from_surface(const vector3d &desired_forward) {
    retail::sub_45B250(this, (i32)&desired_forward);
}

// sub_452D60
vector3d teleporting_layer_inode::get_teleport_target_pos() const {
    return my_param_block.get_optional_pb_vector3d(teleport_target_pos_key, references::unk_00fc3d70.get(), nullptr);
}
