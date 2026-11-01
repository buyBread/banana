#include "game/wds/ai/cpu_combat_inode.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/entity/actor.hh"

using namespace treyarch;

// sub_4EB8B0
void cpu_combat_inode::set_script_chain_size(i32 chain_size) {
    (*script_nums)[4] = (f32)chain_size;
}

// sub_4EB750
void cpu_combat_inode::set_script_strength_test_value(f32 value) {
    (*script_nums)[24] = value;
}

// sub_4EB770
void cpu_combat_inode::set_script_strength_test_status(f32 status) {
    (*script_nums)[25] = status;
}

// sub_409A00
void cpu_combat_inode::set_attack_blocked(bool blocked) {
    if (blocked)
        flags |= cpu_combat_inode_flag_attack_blocked;
    else
        flags &= ~cpu_combat_inode_flag_attack_blocked;
}

// sub_4E64C0
void treyarch::add_incoming_attack(entity_base* target, entity_base* attacker, const combo_move_data* combo_move, i32 chain_position) {
    if (!target || !target->is_an_actor() || !((actor*)target)->get_ai_core())
        return;

    auto* cmbt = (cpu_combat_inode*)((actor*)target)->get_ai_core()->get_info_node(info_node_type_cpu_combat);

    if (cmbt)
        cmbt->unk_025(attacker, combo_move, chain_position);
}
