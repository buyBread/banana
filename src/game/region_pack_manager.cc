#include "game/region_pack_manager.hh"

using namespace treyarch;

// sub_90F670
void region_pack_manager::despawn_and_lock_quest_giver_in_mission(bool lock) {
    if (quest_giver_locked_in_mission == lock)
        return;

    quest_giver_locked_in_mission = lock;

    if (unk_0b2)
        return;

    if (!unk_094)
        unk_0b2 = 1;

    unk_088 = 0;
}
