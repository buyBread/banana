#include "retail.hh"
#include "treyarch/game/game_meter.hh"
#include "treyarch/game/game_meter_manager.hh"
#include "treyarch/shared/mash/mash_info.hh"

using namespace treyarch;

// sub_94D9F0
void meter_data_file::construct_mashed_class() {
    self = nullptr;

    game_meters.construct_mashed_class();

    retail::sub_93F4E0((u32*)references::game_meter_manager.read(), (u32*)this); // game_meter_manager::register_game_meters
}

// sub_93D010
void meter_data_file::destruct_mashed_class() {
    references::game_meter_manager.read()->unregister_game_meters();

    game_meters.destruct_mashed_class();
}

// sub_93D070
void meter_data_file::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    game_meters.unmash(mash_info, this, buffer);
}

// sub_933C70
void meter_behavior_file::construct_mashed_class() {
    self = nullptr;

    meter_behaviors.construct_mashed_class();
}

// sub_934C80
void meter_behavior_file::destruct_mashed_class() {
    meter_behaviors.destruct_mashed_class();
}

// sub_938040
void meter_behavior_file::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    meter_behaviors.unmash(mash_info, this, buffer);
}
