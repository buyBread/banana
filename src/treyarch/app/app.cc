#include <new>

#include "retail.hh"
#include "treyarch/app/app.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/glass_house_manager.hh"
#include "treyarch/game/event/event_manager.hh"
#include "treyarch/shared/platform.hh"
#include "treyarch/shared/singleton.hh"
#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/memory/heap.hh"

namespace treyarch {
    namespace references {
        // the milestone logs these two as "username" and "email" right after reading DEBUG\HOST.INI
        util::memory_reference<mash::string> username { 0x00FC3284 };
        util::memory_reference<mash::string> email    { 0x00FC3350 };

        util::memory_reference<void*> unk_010fa26c { 0x010FA26C };
    } // references
} // treyarch

using namespace treyarch;

// sub_9C98B0
void app::create_inst() {
    void* allocation = memory::heap::allocate(sizeof(app));

    set(allocation ? new (allocation) app() : nullptr);
}

// sub_42A760
app::app() {
    singleton_vtable = &references::singleton_vtable.get();
    retail::sub_639100((u32*)&arch_base_vtable);
    arch_base_vtable = &references::app_arch_base_vtable.get();
    singleton_vtable = &references::app_vtable.get();

    references::master_clock_is_up.write(1);

    mash::string host_ini = *(mash::string*)retail::sub_7EFAB0(platform_pc) + "DEBUG\\HOST.INI";

    if (retail::sub_9C8D90((i32)&host_ini))
        retail::sub_7DF700(host_ini.data());

    // retail keeps only the by-value copies of the milestone's log arguments
    (void)mash::string(references::username.get());
    (void)mash::string(references::email.get());

    references::platform.write(platform_pc);

    event_manager::create_inst();
    retail::sub_770160();
    retail::sub_428B60(); // glass_house_manager::create_inst
    retail::sub_730F10((u32*)references::glass_house_manager.read(), 1); // set_glass_house_level
    retail::sub_428BC0(); // cutscene_player::create_inst
    retail::sub_5FCC10();

    if (references::region_spawns_enabled.read())
        retail::sub_429020(); // region_spawn_manager::create_inst

    retail::sub_429090(); // zombie_manager::create_inst
    references::unk_010fa26c.write(memory::heap::allocate(8));
    retail::sub_428F60();
    retail::sub_42A700();
    retail::sub_428AA0(); // references::game_state
    retail::sub_428B00();
    retail::sub_428C20();
    retail::sub_428C80();
    retail::sub_428D40();
    retail::sub_428CE0();

    real_clock.reset();
    frames_to_skip = 0;

    if (!references::pack_mode.read())
        retail::sub_975600();

    // retail passes this to an empty function (nullsub_1)
    mash::string string_hash_dictionary = *(mash::string*)retail::sub_7EFAB0(platform_pc) + "debug\\string_hash_dictionary";

    retail::sub_428F00(); // mission_memory_manager::create_inst
    retail::sub_428A10();
    retail::sub_4290F0(); // dinput_mgr::create_inst
    retail::sub_428E90(); // input_mgr::create_inst

    if (!references::pack_mode.read())
        retail::sub_428FC0();

    retail::sub_904610();
    retail::sub_429150(); // movie_manager::create_inst
    retail::sub_8FD3A0();
    retail::sub_5FDFB0(0);
    retail::sub_7A78F0();
    retail::sub_667060(); // event_manager default callbacks
    retail::sub_7F9D50();
    retail::sub_42A6A0(); // navmesh_obstacle_manager::create_inst
    retail::sub_823280();

    void* allocation = memory::heap::allocate(sizeof(game));

    the_game = allocation ? new (allocation) game() : nullptr;
    references::game.write(the_game);
}
