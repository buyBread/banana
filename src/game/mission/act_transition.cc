#include <cstdio>
#include <cstring>

#include "retail.hh"
#include "game/amalga/resource_manager.hh"
#include "game/amalga/resource_memory_map.hh"
#include "game/amalga/resource_partition.hh"
#include "game/app.hh"
#include "game/event/event_manager.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_loading_screen.hh"
#include "game/game.hh"
#include "game/mission/mission_manager.hh"
#include "game/quest_manager.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/memory_reference.hh"

namespace treyarch { namespace references {
    util::memory_reference<string_hash> current_act_pre_update  { 0x0102CC24 };
    util::memory_reference<string_hash> current_act_updated     { 0x0102C708 };
    util::memory_reference<string_hash> current_act_post_update { 0x0102CAD4 };

    util::memory_reference<u8> unk_00bcd135 { 0x00BCD135 };
}} // treyarch::references

using namespace treyarch;

// sub_980570
void mission_manager::set_current_act(i32 new_act, bool force) {
    if ((new_act != act || force) && !act_transition_state) {
        act_transition_state = 1;
        act_force_reload     = force;
        requested_act        = new_act;

        begin_act_transition();

        if (force)
            load_act_pack();
    }
}

// sub_9806F0
void mission_manager::advance_act_transition() {
    ++act_transition_frames;

    switch (act_transition_state) {
        case 1:
            begin_act_transition();

            break;

        case 2:
            if (--act_transition_countdown <= 0)
                act_transition_state = 3;

            break;

        case 3:
            load_act_pack();

            break;

        case 4: {
            auto* partition = amalga::resource_manager::get_partition_pointer(amalga::resource_partition_act);

            if (partition->pack_slots[0]->slot_state != amalga::slot_state_ready)
                break;

            if (!_stricmp(&references::level_name_buffer.get(), "megacity") &&
                current_act == 2 && references::unk_00bcd135.read())

                retail::sub_9436F0();

            act_transition_state = 5;

            break;
        }

        case 5:
            if (sky_swap_state)
                break;

            act_transition_frames = 0;

            retail::sub_9055A0(act);
            event_manager::raise_event(references::current_act_post_update.read(), arch_base_vhandle());

            act_transition_state = 6;

            break;

        case 6:
            if (!retail::sub_902810()) // is_bank_loader_idle
                break;

            if (!(flags & mission_manager_flag_mission_ready) && state != mission_manager_state_running_mission &&
                (!auto_launch_instances || !auto_launch_instances->size()) &&
                !quest_manager::inst()->unk_000)

                retail::sub_6A9D60((u8*)references::frontend.get().igo->loading_screen, 0.25f, 0);

            act_transition_state = 0;

            break;
    }
}

// sub_97E420
void mission_manager::begin_act_transition() {
    event_manager::raise_event(references::current_act_pre_update.read(), arch_base_vhandle());

    act_transition_countdown = 3;
    act_transition_state     = 2;

    retail::sub_707E90((u8*)references::frontend.get().igo->loading_screen, 0.1f, 0, 1, 0, 0, 45.0f);
}

// sub_97E490
void mission_manager::load_act_pack() {
    if (current_act == 2 && references::unk_00bcd135.read())
        retail::sub_933FD0();

    act         = requested_act;
    current_act = act < 2 ? act : 2;

    quest_manager::inst()->update_progression();
    event_manager::raise_event(references::current_act_updated.read(), arch_base_vhandle());

    const char* level     = &references::level_name_buffer.get();
    const char* separator = "_";

    if (!_stricmp(level, "megacity")) {
        level     = "";
        separator = "";
    }

    char pack_name[256];
    std::sprintf(pack_name, "%s%sACT%d", level, separator, act + 1);

    string_hash pack_hash;
    pack_hash.initialize(mash::ALLOCATED, pack_name);

    if (!amalga::resource_manager::references::amalgatoc.read()->find_pack_entry(pack_hash)) {
        event_manager::raise_event(references::current_act_post_update.read(), arch_base_vhandle());

        act_transition_state = 6;

        return;
    }

    const char* loaded = (const char*)retail::sub_744E10();

    if (act_force_reload || !loaded || _stricmp(loaded, pack_name)) {
        retail::sub_76FA00((u32*)references::game.read());
        retail::sub_76F960((u32*)references::game.read(), pack_name, 0);
    }

    act_transition_state = 4;
}

// sub_97EE80
i32 mission_manager::get_mission_act(const mission_info&) const {
    i32 mission_act = -1;

    // looks at current_mission whatever it's given
    string_hash pack_hash;
    pack_hash.initialize(mash::ALLOCATED, current_mission.name.c_str());

    amalga::resource_amalgatoc* amalgatoc = amalga::resource_manager::references::amalgatoc.read();

    amalga::resource_amalgatoc_pack_entry* pack = amalgatoc->find_pack_entry(pack_hash);

    if (!pack)
        return mission_act;

    amalga::resource_amalgatoc_pack_entry* group = amalgatoc->find_pack_entry(pack->group_hash);

    if (!group)
        return mission_act;

    mash::string group_name(group->name);
    group_name.to_lower();

    i32 index = group_name.find("act");

    if (index >= 0) {
        char digit = group_name.c_str()[index + 3];

        if ((u8)(digit - '1') <= 4)
            mission_act = digit - '1';
    }

    return mission_act;
}
