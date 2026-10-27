#include "retail.hh"
#include "game/wds/entity/interface/damage_interface.hh"

using namespace treyarch;

void damage_interface::set_hit_points(i32 value, bool use_signals, entity* attacker, entity* attacker_weapon) {
    retail::sub_6524F0((u32*)this, value, use_signals, (i32)attacker, (i32)attacker_weapon);
}
