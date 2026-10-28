#include "game/wds/ai/fight_group.hh"

using namespace treyarch;

// sub_4CDA20
void fight_group::set_actor_group_flags(arch_base_vhandle actor, u32 group_flags) { // the actor has to be a member
    find_member(actor)->group_flags |= group_flags;
}

// sub_4CDA70
void fight_group::clear_actor_group_flags(arch_base_vhandle actor, u32 group_flags) { // same
    find_member(actor)->group_flags &= ~group_flags;
}
