#include <new>

#include "retail.hh"
#include "treyarch/amalga/resource_manager.hh"
#include "treyarch/app/app.hh"
#include "treyarch/game/environment_progression.hh"
#include "treyarch/game/event/default_callbacks.hh"
#include "treyarch/game/event/event_manager.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/glass_house_manager.hh"
#include "treyarch/game/mission/mission_manager.hh"
#include "treyarch/game/movie_manager.hh"
#include "treyarch/game/pathfinder/obstacle_manager.hh"
#include "treyarch/game/region_spawn_manager.hh"
#include "treyarch/game/trigger_manager.hh"
#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/memory/heap.hh"
#include "treyarch/shared/os_file.hh"
#include "treyarch/shared/platform.hh"

namespace treyarch {
    namespace references {
        // the milestone logs these two as "username" and "email" right after reading DEBUG\HOST.INI
        util::memory_reference<mash::string> username { 0x00FC3284 };
        util::memory_reference<mash::string> email    { 0x00FC3350 };
    } // references
} // treyarch

using namespace treyarch;

// sub_9C98B0
void app::create_inst() {
    void* allocation = memory::heap::allocate(sizeof(app));

    instance() = allocation ? new (allocation) app() : nullptr;
}

// sub_42A760
app::app() {
    retail::sub_639100((u32*)(arch_base*)this); // arch_base::arch_base
    arch_base::vtable = (void**)&references::app_arch_base_vtable.get();
    singleton::vtable = (void**)&references::app_vtable.get();

    references::master_clock_is_up.write(1);

    mash::string host_ini = *(mash::string*)retail::sub_7EFAB0(platform_pc) + "DEBUG\\HOST.INI";

    if (os_file::file_exists(host_ini))
        retail::sub_7DF700(host_ini.data());

    // retail keeps only the by-value copies of the milestone's log arguments
    (void)mash::string(references::username.get());
    (void)mash::string(references::email.get());

    references::platform.write(platform_pc);

    event_manager::create_inst();
    amalga::resource_manager::create_inst();
    retail::sub_428B60(); // glass_house_manager::create_inst
    retail::sub_730F10((u32*)references::glass_house_manager.read(), 1); // set_glass_house_level
    retail::sub_428BC0(); // cutscene_player::create_inst
    retail::sub_5FCC10();

    if (references::region_spawns_enabled.read())
        retail::sub_429020(); // region_spawn_manager::create_inst

    retail::sub_429090(); // zombie_manager::create_inst
    references::unk_010fa26c.write(memory::heap::allocate(8));
    environment_progression_state::create_inst();
    retail::sub_42A700();
    mission_manager::create_inst();
    retail::sub_428B00(); // quest_manager::create_inst
    retail::sub_428C20(); // game_meter_manager::create_inst
    retail::sub_428C80();
    retail::sub_428D40();
    retail::sub_428CE0();

    real_clock.reset();
    frames_to_skip = 0;

    if (!references::pack_mode.read())
        retail::sub_975600();

    // retail passes this to a nullsub
    mash::string string_hash_dictionary = *(mash::string*)retail::sub_7EFAB0(platform_pc) + "debug\\string_hash_dictionary";

    retail::sub_428F00(); // mission_memory_manager::create_inst
    trigger_manager::create_inst();
    retail::sub_4290F0(); // dinput_mgr::create_inst
    retail::sub_428E90(); // input_mgr::create_inst

    if (!references::pack_mode.read())
        pathfinder::obstacle_manager::create_inst();

    retail::sub_904610();
    movie_manager::create_inst();
    retail::sub_8FD3A0();
    retail::sub_5FDFB0(0);
    retail::sub_7A78F0();
    register_default_event_callbacks();
    retail::sub_7F9D50();
    retail::sub_42A6A0(); // navmesh_obstacle_manager::create_inst
    retail::sub_823280();

    void* allocation = memory::heap::allocate(sizeof(game));
    the_game = allocation ? new (allocation) game() : nullptr;
    references::game.write(the_game);
}
