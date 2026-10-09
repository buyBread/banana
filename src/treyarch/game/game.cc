#include <cstring>

#include "retail.hh"
#include "treyarch/amalga/resource_manager.hh"
#include "treyarch/amalga/resource_partition.hh"
#include "treyarch/app/app.hh"
#include "treyarch/chuck/script_library/script_library_class.hh"
#include "treyarch/chuck/script_library/slc_manager.hh"
#include "treyarch/chuck/vm/script_manager.hh"
#include "treyarch/game/environment_progression.hh"
#include "treyarch/game/frontend/frontend_manager.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/input/input_mgr.hh"
#include "treyarch/game/level_descriptor.hh"
#include "treyarch/game/mission/mission_manager.hh"
#include "treyarch/game/post_process/post_process.hh"
#include "treyarch/game/quest_manager.hh"
#include "treyarch/game/region_pack_manager.hh"
#include "treyarch/game/region_spawn_manager.hh"
#include "treyarch/game/script/chuck_callbacks.hh"
#include "treyarch/game/shadow/shadow.hh"
#include "treyarch/game/wds/entity/actor.hh"
#include "treyarch/game/wds/entity/entity.hh"
#include "treyarch/game/wds/references.hh"
#include "treyarch/game/wds/world_dynamics_system.hh"
#include "treyarch/shared/dinkumware/vector.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/math/references.hh"
#include "treyarch/shared/math/types/matrix4x4.hh"
#include "treyarch/shared/memory/heap.hh"
#include "treyarch/shared/resource_key.hh"
#include "treyarch/soap/message_box_manager.hh"
#include "treyarch/soap/online.hh"
#include "treyarch/soap/profile.hh"
#include "treyarch/soap/soap.hh"
#include "treyarch/soap/storage.hh"

namespace treyarch { namespace references {
    // game::load_this_level's statics
    util::memory_reference<amalga::resource_partition*> unk_01111758 { 0x01111758 };
    util::memory_reference<amalga::resource_pack_slot*> unk_01111754 { 0x01111754 };

    // a static empty string only game::load_complete reads
    util::memory_reference<mash::string> unk_011116c4 { 0x011116C4 };
}} // treyarch::references

using namespace treyarch;

// sub_97D590
game::game() {
    hero_freeze_depth = 0;
    unk_078           = 0;
    unk_07c           = 0;

    unk_1cc = 0;
    unk_1cd = 0;
    unk_1ce = 0;

    retail::sub_773A80();
    retail::sub_97AB10();
    chuck_callbacks::install();
    retail::sub_83EF20();
    retail::sub_603E00();

    level_time     = 0.0f;
    frame_sequence = 0;

    mash::string level_name(references::default_level_name.get());
    strcpy(&references::level_name_buffer.get(), level_name.c_str());

    void* world_allocation = memory::heap::allocate(sizeof(world_dynamics_system));
    the_world = world_allocation ? (world_dynamics_system*)retail::sub_97A860(world_allocation) : nullptr;
    references::g_world_ptr.write(the_world);

    mb = nullptr;

    void* data_allocation = memory::heap::allocate(8);
    game_data* constructed_data = data_allocation ?
        (game_data*)retail::sub_7A4FB0((u32*)data_allocation) : nullptr;

    frame_timing.total_delta = 0.0f;
    frame_timing.flip_delta  = 0.0f;
    frame_timing.limit_delta = 0.0f;
    current_frame_delta      = 0.0f;
    data                     = constructed_data;
    unk_198                  = 0;

    disable_interface               = 0;
    disable_start_menu              = 0;
    i_quit                          = 0;
    level_is_loaded                 = 0;
    level_is_unloading              = 0;
    load_new_level                  = 0;
    unk_04d                         = 1;
    debug_single_step               = 0;
    debug_stop_physics              = 0;
    unk_050                         = 0;
    unk_054                         = 0.45f;
    game_paused                     = 0;
    use_default_hero_start_position = 1;
    wait_for_intro_scene_anim       = 0;

    base_camera         = nullptr;
    current_game_camera = nullptr;
    current_view_camera = nullptr;
    unk_078             = 0;
    string_localizer    = nullptr;

    retail::sub_738990();
    references::unk_01110e90.write('\0');

    push_process(references::main_process.get());
    push_process(references::start_process.get());

    unk_1cc = 0;
    unk_1cd = 0;
    unk_1ac = 0;

    retail::sub_95A2F0();
    retail::sub_77EA20();
    retail::sub_7B48D0();
}

// sub_97DD10
void game::one_time_init_stuff() {
    if (soap::references::enable_profiles.read() ||
        soap::references::enable_storage.read()  ||
        soap::references::enable_online.read()) {

        {
            dinkumware::vector<soap::message_box_definition> boxes;

            retail::sub_97D790((i32)&boxes);
            retail::sub_9EE930((i32)&boxes);
        }

        if (soap::references::enable_profiles.read())
            soap::profile::inst()->initialize(1, false);

        if (soap::references::enable_storage.read())
            soap::storage::inst()->initialize(4, 10, 20000);

        if (soap::references::enable_online.read())
            soap::online::inst()->initialize();
    }

    if (data)
        retail::sub_7A7710((u32*)data); // game_data::initialize

    retail::sub_76B950(); // retail hands the result to an empty call (nullsub_1)
    retail::sub_76CD80((u32**)retail::sub_76B430()); // achievement_tracker::inst

    frontend_manager &frontend = references::frontend.get();

    retail::sub_6B12C0((u8*)&frontend);
    retail::sub_717BC0((u32*)&frontend);

    if (!references::pack_mode.read())
        retail::sub_9874E0();

    // retail follows with an empty call (nullsub_1)

    retail::sub_97ABE0();
    retail::sub_97AD30((u32*)this); // game::message_board_init
    retail::sub_953EA0((i32)references::raw_delta_consumer.read());
    retail::sub_6F76B0((u32*)references::unk_0102ce40.read());
    retail::sub_95D520();
}

// sub_97CEA0
void game::unload_current_level() {
    if (!level_is_loaded) {
        retail::sub_900F70();

        return;
    }

    level_is_unloading = 1;

    retail::sub_7F0920((u32*)quest_manager::inst());

    if (references::region_spawns_enabled.read())
        retail::sub_951C40((i32)region_pack_manager::inst(), 1);

    retail::sub_824830((u32*)amalga::resource_manager::references::unk_010f7760.read(), 0);

    if (the_world->the_terrain)
        retail::sub_7970F0((u32*)the_world->the_terrain);

    retail::sub_598BA0();
    retail::sub_5998A0();
    retail::sub_7C2DF0();

    references::unk_00fc7a90.write(nullptr);

    retail::sub_75F0F0();
    retail::sub_968320((i32)the_world, 1);

    unk_07c = 0;

    engine_recursive_lock &lock = amalga::resource_manager::references::unk_010300a8.get();

    lock.acquire();
    amalga::resource_manager::references::resource_context_stack_mutex.read()->acquire();

    retail::sub_757240(); // pop_resource_context

    amalga::resource_manager::references::resource_context_stack_mutex.read()->release();
    lock.release();

    auto* common = (amalga::resource_partition*)retail::sub_739C40(amalga::resource_partition_common);

    common->streamer.flush((amalga::resource_pack_streamer::flush_callback)retail::sub_771950);
    common->streamer.unload_all();
    common->streamer.flush((amalga::resource_pack_streamer::flush_callback)retail::sub_771950);

    level_is_loaded = 0;

    retail::sub_900F70();
    retail::sub_7F7340((u32*)input_mgr::inst()->unk_004);

    while (hero_freeze_depth > 0)
        freeze_hero(false);

    if (references::script_controller.read())
        references::script_controller.read()->clear_script_callbacks(nullptr);

    app::inst()->clear_script_callbacks(nullptr);
    retail::sub_7BCDF0();

    world_dynamics_system* world = the_world;

    if (world) {
        retail::sub_97A400(world); // ~world_dynamics_system
        memory::heap::free(world);
    }

    the_world = nullptr;
    references::g_world_ptr.write(nullptr);

    chuck::vm::script_manager &scripts = *chuck::vm::script_manager::inst();

    retail::sub_A1C430((u32*)&scripts);   // script_manager::unload_all
    retail::sub_A1BF30((void**)&scripts); // releases both variable containers
    retail::sub_645E10();

    // retail follows with an empty call (nullsub_1)

    base_camera         = nullptr;
    current_view_camera = nullptr;
    current_game_camera = nullptr;

    chuck::script_library::slc_manager::destroy();

    retail::sub_97CDC0((u32*)&references::unk_010769cc.get());

    level_is_loaded = 0;

    input_mgr::inst()->unk_80c = 0;

    // the next level gets a fresh world
    void* world_allocation = memory::heap::allocate(sizeof(world_dynamics_system));
    the_world = world_allocation ? (world_dynamics_system*)retail::sub_97A860(world_allocation) : nullptr;
    references::g_world_ptr.write(the_world);

    references::unk_00f4cd30.write(1);

    retail::sub_7B48D0();

    post_process::release_device_resources();
    shadow::release_device_resources();

    level_is_unloading = 0;
    references::unk_01111391.write(0);
}

// sub_97BD40
void game::freeze_hero(bool freeze) {
    if (the_world && !the_world->hero_ptr)
        return;

    const bool was_frozen = hero_freeze_depth > 0;

    hero_freeze_depth = freeze ? hero_freeze_depth + 1 : hero_freeze_depth - 1;

    if (hero_freeze_depth < 0)
        hero_freeze_depth = 0;

    // only the first freeze and the last unfreeze do anything
    if (freeze) {
        if (was_frozen || hero_freeze_depth <= 0)
            return;
    } else if (!was_frozen || hero_freeze_depth)
        return;

    if (!the_world || !the_world->hero_ptr)
        return;

    entity* hero = the_world->hero_ptr;

    retail::sub_548450((u32*)hero, freeze); // the invulnerable flag

    if (freeze)
        ((void (__thiscall*)(entity*, i32))hero->vtable[0x1D0 / 4])(hero, 1);
    else
        ((void (__thiscall*)(entity*, i32))hero->vtable[0x1D4 / 4])(hero, 1);

    hero->player_ifc()->set_enabled(!freeze);

    if (((actor*)hero)->get_ai_core()) {
        if (freeze)
            retail::sub_4DF470((i32*)((actor*)hero)->get_ai_core(), 0); // ai_core::push_ai_disable
        else
            retail::sub_4DF4B0((i32*)((actor*)hero)->get_ai_core(), 0); // ai_core::pop_ai_disable
    }
}

// sub_97B1A0
void game::load_complete() {
    matrix4x4 transform;
    retail::sub_4015B0((u64*)&transform);

    terrain* the_terrain = references::g_world_ptr.read()->the_terrain;

    // a spawn name nothing ever sets; it starts out empty
    const char* spawn_name = references::unk_011116c4.get().c_str();
    void*       spawn      = nullptr;

    if (std::strncmp(spawn_name, "", 0xFFFF)) {
        string_hash spawn_hash;
        spawn_hash.initialize(mash::ALLOCATED, spawn_name);

        spawn = (void*)retail::sub_7A1990((u32*)the_terrain, (i32)spawn_hash.source_hash_code);
    }

    game* the_game = references::game.read();

    if (spawn) {
        transform.w = vector4(*(vector3*)((u8*)spawn + 0xD0), 1.0f);
    } else if (the_game->use_default_hero_start_position) {
        string_hash hero_start;
        hero_start.initialize(mash::ALLOCATED, "HERO_START");

        transform = ((entity*)retail::sub_6414A0((u32*)&hero_start))->my_abs_po->matrix;
    } else
        transform.w = vector4(the_game->level.hero_start_position, 1.0f);

    // anything not standing upright is rebuilt on the default axes
    const vector3 &up = math::references::unk_00f4d1ec.get();

    if (transform.y.x != up.x || transform.y.y != up.y || transform.y.z != up.z) {
        matrix4x4 upright;
        retail::sub_5FC820((u64*)&upright,
                           (i32)&math::references::unk_00f4d1e0.get(),
                           (i32)&up,
                           (u32*)&math::references::unk_00f4d1f8.get(),
                           (u32*)&transform.w);

        transform = upright;
    }

    world_dynamics_system* world = references::g_world_ptr.read();

    if (world->hero_ptr)
        retail::sub_626BE0((i32)world->hero_ptr, (u64*)&transform); // entity_base::set_rel_po
    else if (world->camera_mgr.marky_camera)
        retail::sub_626BE0((i32)world->camera_mgr.marky_camera, (u64*)&transform);

    the_game->level.load_complete_called = 1;
}

// sub_97B440
void game::load_this_level(bool start_only, bool finish_only) {
    if (!finish_only) {
        frame_clock.reset();

        disable_start_menu = 0;
        disable_interface  = 0;
        game_paused        = 0;
        unk_059            = 0;
        level_time         = 0.0f;

        clear_screen();

        app::inst()->skip_some_frames(10);

        unk_1c8 = 0.0f;
        blur    = 0.0f;
        unk_1c4 = 0.0f;

        post_process::create_device_resources();
        shadow::create_device_resources();
        retail::sub_97AAB0(); // slc_manager::setup

        auto* common = (amalga::resource_partition*)retail::sub_739C40(amalga::resource_partition_common);

        references::unk_01111758.write(common);
        references::unk_01111754.write(common->pack_slots[0]);

        common->streamer.load(level.descriptor->level_name.c_str(), 0);

        if (start_only)
            return;
    }

    references::unk_01111758.read()->streamer.flush((amalga::resource_pack_streamer::flush_callback)retail::sub_771950);

    {
        auto* slot = (amalga::resource_pack_slot*)retail::sub_74D900(amalga::resource_partition_common); // get_best_context

        engine_recursive_lock &lock = amalga::resource_manager::references::unk_010300a8.get();

        lock.acquire();
        amalga::resource_manager::references::resource_context_stack_mutex.read()->acquire();

        retail::sub_767760((i32)slot); // push_resource_context

        amalga::resource_manager::references::resource_context_stack_mutex.read()->release();
        lock.release();
    }

    retail::sub_968670((i32**)&references::g_world_ptr.read()->camera_mgr);

    // the milestone asserts these class value refs exist before any script links; retail drops the results
    chuck::script_library::references::slc_entity.read()->find_instance(mash::string("MARKY_CAM"));
    chuck::script_library::references::slc_entity.read()->find_instance(mash::string("HERO_START"));
    chuck::script_library::references::slc_entity.read()->find_instance(mash::string("ANIMATED_CAM"));
    chuck::script_library::references::slc_script_controller.read()->find_instance(mash::string("CONTROLLER_1"));

    chuck::vm::script_manager &scripts = *chuck::vm::script_manager::inst();

    retail::sub_A1B8C0((i32)&scripts); // loads the master game and shared variable containers
    retail::sub_97E060((u32*)mission_manager::inst()); // mission_manager::setup_game_var_refs

    {
        mash::string busy_teaching("busy_teaching");

        references::g_world_ptr.read()->gv_busy_teaching =
            (f32*)retail::sub_A1A1B0((u32*)&scripts, (u32*)&busy_teaching, nullptr);
    }

    if (retail::sub_A1A170((u32*)&scripts, "init_gv")) {
        string_hash init_gv;
        init_gv.initialize(mash::ALLOCATED, "init_gv");

        string_hash init_sv;
        init_sv.initialize(mash::ALLOCATED, "init_sv");

        retail::sub_A1BC60((i32)&scripts, (i32*)&init_gv, 2, (i32)references::unk_01111754.read(), 0); // script_manager load
        retail::sub_A1BC60((i32)&scripts, (i32*)&init_sv, 2, (i32)references::unk_01111754.read(), 0);

        scripts.run_single_exec(init_gv, string_hash(), 0.0f, false);
        scripts.run_single_exec(init_sv, string_hash(), 0.0f, false);

        retail::sub_A1C2D0((u32*)&scripts, (i32*)&init_gv, 0, 0); // script_manager unload
        retail::sub_A1C2D0((u32*)&scripts, (i32*)&init_sv, 0, 0);

        // retail follows with an empty call (nullsub_1)
    }

    amalga::resource_pack_slot* common_slot = references::unk_01111754.read();

    {
        resource_key sin_key;
        retail::sub_739B10((u32*)&sin_key, level.name.c_str(), 2);
        retail::sub_73DDE0(retail::sub_75E2C0((u32*)common_slot->pack_directory, (u32*)&sin_key, nullptr, nullptr));
    }

    const char* level_name = level.descriptor->level_name.c_str();

    {
        string_hash level_hash;
        level_hash.initialize(mash::ALLOCATED, level_name);

        resource_key key;
        key.hash = string_hash();
        key.set(level_hash, 7);

        unk_07c = (u32)retail::sub_7629D0((u32*)&key, nullptr, nullptr);

        if (unk_07c)
            retail::sub_7D9CA0((u32*)unk_07c);
    }

    retail::sub_980000((i32*)mission_manager::inst(), (void*)level_name, (i32)references::unk_01111754.read());
    retail::sub_7F7760((i32)quest_manager::inst(), (i32)level_name, (i32)references::unk_01111754.read());
    retail::sub_90FCB0((i32*)references::unk_010fa26c.read(), (i32)level_name, (i32)references::unk_01111754.read());
    retail::sub_7EB4D0((i32*)references::environment_progression_state.read(), (i32)level_name, (i32)references::unk_01111754.read());

    if (references::region_spawns_enabled.read() && region_spawn_manager::inst())
        retail::sub_954750((u32*)region_spawn_manager::inst(), (i32)level_name, (i32)references::unk_01111754.read());

    {
        string_hash vehicles_hash;
        vehicles_hash.initialize(mash::ALLOCATED, "vehicles");

        resource_key key;
        key.hash = string_hash();
        key.set(vehicles_hash, 28);

        references::unk_00fc7a90.write((void*)retail::sub_75E2C0((u32*)references::unk_01111754.read()->pack_directory,
                                                                (u32*)&key,
                                                                nullptr,
                                                                nullptr));
    }

    retail::sub_7B7570();
    retail::sub_5A0F70();
    retail::sub_592FF0();

    retail::sub_7ACDC0((u32*)the_world->the_terrain, (i32 (*)())&game::load_complete);
    retail::sub_77BAB0((u32*)references::g_world_ptr.read()->the_terrain);

    {
        string_hash global_script;
        global_script.initialize(mash::ALLOCATED, references::g_world_ptr.read()->script_mgr.global_script_filename.c_str());

        auto* slot = (amalga::resource_pack_slot*)retail::sub_74D900(amalga::resource_partition_common); // get_best_context

        retail::sub_A1BC60((i32)&scripts, (i32*)&global_script, 1, (i32)slot, 0); // script_manager load
    }

    retail::sub_95CC60((i32*)&the_world->render_mgr);

    {
        mash::string hero_name((const char*)retail::sub_77CBC0((u32*)data));

        matrix4x4 start_transform;
        u64* hero_transform = (u64*)retail::sub_95C730((u32*)the_world, (i32)&start_transform); // the HERO_START transform

        retail::sub_967E00(the_world, (u32*)&hero_name, hero_transform); // creates the hero
    }

    if (i_quit)
        return;

    current_game_camera = (camera*)retail::sub_95C430((u32*)the_world); // chase camera

    auto* chase = (camera*)retail::sub_95C430((u32*)the_world);

    if (chase) {
        current_view_camera = chase;

        bool (__thiscall* slot_29)(camera*) = (bool (__thiscall*)(camera*))chase->vtable[0x74 / 4];

        if (slot_29(chase) && *(i32*)((u8*)chase + 0xEC) < 1)
            *(i32*)((u8*)chase + 0xEC) = 1;

        if (!current_game_camera && slot_29(chase))
            current_game_camera = chase;

        retail::sub_96A350();
    } else {
        current_view_camera = nullptr;
        current_game_camera = nullptr;
    }

    retail::sub_904F00((i32)&frame_clock);

    level.level_clock.reset();

    retail::sub_6723B0((char**)retail::sub_6370C0());
    retail::sub_6627D0();

    references::unk_01111391.write(1);
}
