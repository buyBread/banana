#include "game/game_meter.hh"
#include "game/game_meter_manager.hh"
#include "game/summon_progression.hh"
#include "game/summon_state.hh"

using namespace treyarch;

// sub_934E10
void summon_state::deduct_meter_for_special_attack() {
    vc8::map<i32, game_meter_base*> &meters = references::game_meter_manager.read()->meter_dict;

    game_meter_base* current = meters.find(0)->value.second;
    game_meter_base* meter   = meters.find(0)->value.second;

    // slot 13
    ((void (__thiscall*)(game_meter_base*, f32))(*(void***)meter)[13])(meter, (f32)((f64)current->level - (f64)active_summon->unk_40));
}
