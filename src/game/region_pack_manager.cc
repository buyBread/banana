#include "retail.hh"
#include "treyarch/mm/mm.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/mission/mission_memory_manager.hh"
#include "game/region_pack_manager.hh"
#include "game/region_spawn_manager.hh"
#include "game/spawn/quest_giver_spawn.hh"
#include "game/spawn/spawn_table_manager.hh"
#include "game/spawn_table_interface.hh"
#include "game/summon_state.hh"

namespace treyarch {
    using construct_held_pack_function = held_pack*    (__thiscall*)(void*         self,
                                                                     mash::string  pack_name,
                                                                     string_hash   unk_00,
                                                                     mash::string  unk_10,
                                                                     i32           sub_pool); // owns both strings
} // treyarch

using namespace treyarch;

// sub_90F670
void region_pack_manager::despawn_and_lock_quest_giver_in_mission(bool lock) {
    if (quest_giver_locked_in_mission == lock)
        return;

    quest_giver_locked_in_mission = lock;

    if (swap_requested)
        return;

    if (!unk_094)
        swap_requested = true;

    next_quest_giver_spawn = nullptr;
}

// sub_90F6E0
// inlined @ sub_92E970
bool region_pack_manager::hold_quest_giver_spawn(quest_giver_spawn* spawn) { // queued behind the held one, refused while locked or swapping
    if (quest_giver_locked_in_mission)
        return false;

    if (held_quest_giver_spawn == spawn || next_quest_giver_spawn == spawn)
        return true;

    if (swap_requested)
        return false;

    if (!unk_094)
        swap_requested = true;

    next_quest_giver_spawn = spawn;

    return true;
}

// inlined @ sub_955830
void region_pack_manager::load_selection_pack(spawn_table_unk_68_entry* entry, i32 sub_pool) {
    ++unk_008;

    void*       memory = mm::allocate(sizeof(held_pack));
    string_hash hash;

    held_pack* pack = memory ?
        ((construct_held_pack_function)retail::sub_933F30)(memory,
                                                           entry->pack_name,
                                                           *(string_hash*)retail::sub_48E150(&hash, (i32)&entry->unk_0c),
                                                           entry->unk_18,
                                                           sub_pool) :
        nullptr;

    unk_03c.push_back(pack);

    retail::sub_866470((u32*)mission_memory_manager::inst(), (i32)&entry->pack_name, sub_pool, 3); // loads the pack
}

// sub_955830
void region_pack_manager::frame_advance(f32) { // one step of the requested pack work, a state at a time
    switch (state) {
        case 2:
            if (retail::sub_8248E0((u32*)mission_memory_manager::inst())) // packs are unloading
                return;

            if (released_packs.size()) {
                if (!retail::sub_938B30((u32*)this, (i32)*released_packs.begin()))
                    return;

                held_pack* pack = *released_packs.begin();

                released_packs.erase(released_packs.begin());

                if (pack) {
                    retail::sub_9345D0((u32*)pack); // ~held_pack
                    mm::free(pack);
                }

                return;
            }

            spawn_table_manager::inst()->unk_049 = false;

            release_requested = false;
            state             = 0;

            return;

        case 3: {
            if (retail::sub_8248E0((u32*)mission_memory_manager::inst()))
                return;

            retail::sub_92E640((u32*)region_spawn_manager::inst());

            spawn_table_manager::inst()->unk_04c->rebuild_spawn_tables();

            bool loaded = true;

            // one missing pack a frame; the forcing flags walk on past the end
            if (unk_08c && selection_load_requested) {
                spawn_table_unk_68_entry** it = unk_08c->entries.begin();

                for (; it != unk_08c->entries.end() || references::unk_010fb37d.read(); ++it)
                    if (!retail::sub_83F720((u32*)mission_memory_manager::inst(), (i32)&(*it)->pack_name)) {
                        load_selection_pack(*it, *unk_05c > 0 ? unk_070 : -1);
                        break;
                    }

                if (it != unk_08c->entries.end() && !references::unk_010fb37d.read())
                    loaded = false;
            }

            if (unk_090 && loaded && selection_load_requested) {
                spawn_table_unk_68_entry** it = unk_090->entries.begin();

                for (; it != unk_090->entries.end() || references::unk_010fb37c.read(); ++it)
                    if (!retail::sub_83F720((u32*)mission_memory_manager::inst(), (i32)&(*it)->pack_name)) {
                        load_selection_pack(*it, *unk_05c > 0 ? unk_074 : -1);
                        break;
                    }

                if (it != unk_090->entries.end() && !references::unk_010fb37c.read())
                    loaded = false;
            }

            if (unk_080 && loaded && selection_load_requested) {
                spawn_table_unk_68_entry** it = unk_080->entries.begin();

                for (; it != unk_080->entries.end(); ++it)
                    if (!retail::sub_83F720((u32*)mission_memory_manager::inst(), (i32)&(*it)->pack_name)) {
                        load_selection_pack(*it, -1);
                        break;
                    }

                if (it != unk_080->entries.end())
                    return;

                if (retail::sub_8248E0((u32*)mission_memory_manager::inst()))
                    return;
            } else if (!loaded)
                return;

            selection_load_requested = false;

            retail::sub_94F010((i32)this); // update_pack_selection

            state = 0;

            return;
        }

        case 4: {
            if (retail::sub_8248E0((u32*)mission_memory_manager::inst()))
                return;

            held_pack** it = unk_02c.begin();

            for (; it != unk_02c.end(); ++it) {
                alignas(4) u8 storage[sizeof(mash::string)];

                mash::string* name    = (*it)->get_pack_name((mash::string*)storage);
                bool          missing = !retail::sub_83F720((u32*)mission_memory_manager::inst(), (i32)name);

                name->finalize(mash::ALLOCATED);

                if (missing) {
                    retail::sub_866470((u32*)mission_memory_manager::inst(), (i32)&(*it)->pack_name, -1, 3); // loads the pack
                    break;
                }
            }

            if (it != unk_02c.end())
                return;

            if (retail::sub_8248E0((u32*)mission_memory_manager::inst()))
                return;

            held_load_requested = false;
            state               = 0;

            return;
        }

        case 5:
            if (retail::sub_8248E0((u32*)mission_memory_manager::inst()))
                return;

            retail::sub_866420((u32*)mission_memory_manager::inst(), unk_05c); // lays partition 3 out in sub-pools

            layout_requested = false;
            state            = 0;

            return;

        case 6: {
            if (retail::sub_8248E0((u32*)mission_memory_manager::inst()))
                return;

            if (unk_094) {
                if (retail::sub_83F720((u32*)mission_memory_manager::inst(), (i32)&unk_094->pack_name)) {
                    retail::sub_90FC10((u32*)references::summon_state.read());
                    retail::sub_866500((u32*)mission_memory_manager::inst(), (i32)&unk_094->pack_name, -2); // unloads the pack
                } else
                    unk_094 = nullptr;

                return;
            }

            // the held giver goes first: its entity, then its pack
            if (held_quest_giver_spawn) {
                mash::string held_pack_name = held_quest_giver_spawn->get_pack_name();

                if (!retail::sub_83F720((u32*)mission_memory_manager::inst(), (i32)&held_pack_name)) {
                    held_quest_giver_spawn = nullptr;
                    return;
                }

                if (held_quest_giver_spawn->spawned_entity) {
                    held_quest_giver_spawn->despawn_entity();
                    return;
                }

                if (!retail::sub_68BF20((i32)references::frontend.get().igo->conversation_menu_system))
                    retail::sub_6F9250((i32)references::frontend.get().igo->conversation_menu_system); // ends the conversation without an event

                mash::string pack_name = held_quest_giver_spawn->get_pack_name();

                retail::sub_866500((u32*)mission_memory_manager::inst(), (i32)&pack_name, -2); // unloads the pack

                return;
            }

            if (unk_098 && !unk_0b4) {
                if (!retail::sub_83F720((u32*)mission_memory_manager::inst(), (i32)&unk_098->pack_name)) {
                    retail::sub_87A0C0((u32*)mission_memory_manager::inst(), (i32)&unk_098->pack_name); // loads the pack
                    return;
                }

                unk_094 = unk_098;
                unk_098 = nullptr;

                swap_requested = false;
                state          = 0;

                return;
            }

            if (!next_quest_giver_spawn || quest_giver_locked_in_mission) {
                swap_requested = false;
                state          = 0;

                return;
            }

            mash::string next_pack_name = next_quest_giver_spawn->get_pack_name();

            if (!retail::sub_83F720((u32*)mission_memory_manager::inst(), (i32)&next_pack_name)) {
                mash::string pack_name = next_quest_giver_spawn->get_pack_name();

                retail::sub_87A0C0((u32*)mission_memory_manager::inst(), (i32)&pack_name); // loads the pack

                return;
            }

            held_quest_giver_spawn = next_quest_giver_spawn;
            next_quest_giver_spawn = nullptr;

            swap_requested = false;

            if (!held_quest_giver_spawn->spawned_entity)
                held_quest_giver_spawn->spawn_entity();

            state = 0;

            return;
        }

        default:
            if (unk_0ac) {
                if (release_requested)
                    state = 2;
                else if (layout_requested)
                    state = 5;
                else if (swap_requested)
                    state = 6;

                return;
            }

            if (swap_requested) {
                state = 6;
                return;
            }

            if (unk_0b1) {
                if (retail::sub_92F310((u32*)spawn_table_manager::inst()))
                    unk_0b1 = false;

                return;
            }

            if (release_requested)
                state = 2;
            else if (layout_requested)
                state = 5;
            else if (selection_load_requested) {
                references::unk_010fb37d.write(false);
                references::unk_010fb37c.write(false);

                state = 3;
            } else if (held_load_requested)
                state = 4;
            else
                retail::sub_947090((u32*)this);

            return;
    }
}
