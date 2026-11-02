#include <cstring>

#include "retail.hh"
#include "game/amalga/resource_directory.hh"
#include "game/amalga/resource_manager.hh"
#include "game/amalga/resource_partition.hh"
#include "game/app.hh"
#include "treyarch/chuck/script_library/script_library_class.hh"
#include "treyarch/chuck/script_library/slc_manager.hh"
#include "treyarch/chuck/vm/script_manager.hh"
#include "game/environment_progression.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_scrapbook.hh"
#include "game/game.hh"
#include "game/geometry/geometry_manager.hh"
#include "game/input/input_mgr.hh"
#include "game/level_descriptor.hh"
#include "game/mission/mission_manager.hh"
#include "game/mission/mission_memory_manager.hh"
#include "game/post_process/post_process.hh"
#include "game/quest_manager.hh"
#include "game/region_pack_manager.hh"
#include "game/region_spawn_manager.hh"
#include "game/script/chuck_callbacks.hh"
#include "game/script/chuck_lib_controller.hh"
#include "game/script/chuck_lib_entity.hh"
#include "game/shadow/shadow.hh"
#include "game/ui_character_info_list.hh"
#include "game/vehicle_model.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"
#include "vc8/vector.hh"
#include "treyarch/mash/string_hash.hh"
#include "game/math/algebra.hh"
#include "game/math/references.hh"
#include "treyarch/mm/mm.hh"
#include "game/resource_key.hh"
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

    // game::handle_cameras' debug cameras: 0 the chase cam, 1 the user cam, 2 the edit cam
    util::memory_reference<i32> user_cam_mode { 0x011113A0 };

    // picks the user cam's control set; input_mgr's control queries read it too
    util::memory_reference<u8> user_cam_controls { 0x01111393 };

    // set after the hero was moved to the user cam, so physics stops again next frame
    util::memory_reference<u8> user_cam_hero_moved { 0x0111139D };

    // set when handle_cameras switched or reseated a camera; nothing reads it
    util::memory_reference<u8> camera_changed { 0x0111139C };

    // the three camera controls' states last frame
    util::memory_reference<f32> last_camera_control_0 { 0x011113A4 };
    util::memory_reference<f32> last_camera_control_1 { 0x011113A8 };
    util::memory_reference<f32> last_camera_control_2 { 0x011113AC };
}} // treyarch::references

using namespace treyarch;

// sub_97D590
game::game() {
    hero_freeze_depth = 0;
    unk_078           = 0;
    speedtrees        = nullptr;

    unk_1cc = 0;
    unk_1cd = 0;
    unk_1ce = 0;

    retail::sub_773A80();
    chuck::vm::script_manager::setup();
    chuck_callbacks::install();
    retail::sub_83EF20();
    retail::sub_603E00();

    level_time     = 0.0f;
    frame_sequence = 0;

    mash::string level_name(references::default_level_name.get());
    strcpy(&references::level_name_buffer.get(), level_name.c_str());

    void* world_allocation = mm::allocate(sizeof(world_dynamics_system));
    the_world = world_allocation ? new (world_allocation) world_dynamics_system() : nullptr;
    references::g_world_ptr.write(the_world);

    mb = nullptr;

    void* data_allocation = mm::allocate(8);
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
            vc8::vector<soap::message_box_definition> boxes;

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

    // the front end's retail constructor built these
    frontend.igo->loading_screen->stamp_source_vtable();
    frontend.igo->scrapbook->stamp_source_vtable();
    frontend.igo->conversation_menu_system->stamp_source_vtables();

    if (!references::pack_mode.read())
        retail::sub_9874E0();

    // retail follows with an empty call (nullsub_1)

    retail::sub_97ABE0();
    retail::sub_97AD30((u32*)this); // game::message_board_init
    retail::sub_953EA0((i32)references::upgrade_manager.read());
    retail::sub_6F76B0((u32*)references::ui_character_info_map.read());
    retail::sub_95D520();
}

// sub_97CEA0
void game::unload_current_level() {
    if (!level_is_loaded) {
        retail::sub_900F70(); // sound shutdown

        return;
    }

    level_is_unloading = 1;

    retail::sub_7F0920((u32*)quest_manager::inst());

    if (references::region_spawns_enabled.read())
        retail::sub_951C40((i32)region_pack_manager::inst(), 1);

    retail::sub_824830((u32*)mission_memory_manager::inst(), 0);

    if (the_world->the_terrain)
        retail::sub_7970F0((u32*)the_world->the_terrain);

    retail::sub_598BA0();
    retail::sub_5998A0();
    retail::sub_7C2DF0();

    references::vehicle_model_depot.write(nullptr);

    retail::sub_75F0F0();
    retail::sub_968320((i32)the_world, 1);

    speedtrees = nullptr;

    engine_recursive_lock &lock = amalga::resource_manager::references::unk_010300a8.get();

    lock.acquire();
    amalga::resource_manager::references::resource_context_stack_mutex.read()->acquire();

    amalga::resource_manager::pop_resource_context();

    amalga::resource_manager::references::resource_context_stack_mutex.read()->release();
    lock.release();

    auto* common = amalga::resource_manager::get_partition_pointer(amalga::resource_partition_common);

    common->streamer.flush((amalga::resource_pack_streamer::flush_callback)retail::sub_771950);
    common->streamer.unload_all();
    common->streamer.flush((amalga::resource_pack_streamer::flush_callback)retail::sub_771950);

    level_is_loaded = 0;

    retail::sub_900F70(); // sound shutdown
    retail::sub_7F7340((u32*)input_mgr::inst()->rumble_ptr);

    while (hero_freeze_depth > 0)
        freeze_hero(false);

    if (references::script_controller.read())
        references::script_controller.read()->clear_script_callbacks(nullptr);

    app::inst()->clear_script_callbacks(nullptr);
    retail::sub_7BCDF0();

    world_dynamics_system* world = the_world;

    if (world) {
        world->~world_dynamics_system();
        mm::free(world);
    }

    the_world = nullptr;
    references::g_world_ptr.write(nullptr);

    chuck::vm::script_manager &scripts = *chuck::vm::script_manager::inst();

    scripts.clear();
    scripts.destroy_game_var();
    retail::sub_645E10();

    // retail follows with an empty call (nullsub_1)

    base_camera         = nullptr;
    current_view_camera = nullptr;
    current_game_camera = nullptr;

    chuck::script_library::slc_manager::destroy();

    retail::sub_97CDC0((u32*)&references::unk_010769cc.get());

    level_is_loaded = 0;

    input_mgr::inst()->playing_scene_anim = false;

    // the next level gets a fresh world
    void* world_allocation = mm::allocate(sizeof(world_dynamics_system));
    the_world = world_allocation ? new (world_allocation) world_dynamics_system() : nullptr;
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

    hero->set_invulnerable(freeze);

    if (freeze)
        hero->suspend(true);
    else
        hero->unsuspend(true);

    hero->player_ifc()->set_enabled(!freeze);

    if (((actor*)hero)->get_ai_core()) {
        if (freeze)
            ((actor*)hero)->get_ai_core()->push_disable(0);
        else
            ((actor*)hero)->get_ai_core()->pop_disable(0);
    }
}

void game::enable_marky_cam(bool enable, bool sync, f32 priority, f32 interp_time) {
    retail::sub_97AE30((i32)this, enable, sync, priority, interp_time);
}

// sub_97C060
void game::handle_cameras(input_mgr* input_manager, f32* time_inc) {
    references::cameras_handled.write(1);

    if (!level_is_loaded)
        return;

    if (references::user_cam_mode.read() == 1 &&
        retail::sub_96EA30((i32)input_manager, 66, 11, 0, 0) != 0.0) { // input_mgr::get_control_state

        string_hash user_cam_id;
        user_cam_id.initialize(mash::ALLOCATED, "USER_CAM");

        auto* user_cam = (entity*)retail::sub_640AE0(&user_cam_id.source_hash_code, entity_flavor_camera, false); // entity_handle_manager::find_entity

        const vector3d &facing   = user_cam->my_abs_po->get_z_facing();
        const vector3d &position = user_cam->my_abs_po->get_position();

        vector3d ahead((f32)((f64)position.x + (f64)(f32)((f64)facing.x * 4.0)),
                       (f32)((f64)position.y + (f64)(f32)((f64)facing.y * 4.0)),
                       (f32)((f64)position.z + (f64)(f32)((f64)facing.z * 4.0)));

        references::g_world_ptr.read()->malor_point(ahead, false, true);

        references::g_world_ptr.read()->hero_ptr->physical_ifc()->cancel_all_velocity(references::unk_011113e0.get(), true);
        references::g_world_ptr.read()->hero_ptr->player_ifc()->unk_140();

        references::user_cam_hero_moved.write(1);

        debug_stop_physics = 0;
    } else if (references::user_cam_hero_moved.read()) {
        debug_stop_physics = 1;

        references::user_cam_hero_moved.write(0);
    }

    bool alternate_controls = references::user_cam_controls.read();

    f32 control_0 = (f32)retail::sub_96EA30((i32)input_manager, alternate_controls ? 46 : 43, 11, 0, 0); // input_mgr::get_control_state
    f32 control_1 = (f32)retail::sub_96EA30((i32)input_manager, alternate_controls ? 47 : 44, 11, 0, 0);
    f32 control_2 = (f32)retail::sub_96EA30((i32)input_manager, alternate_controls ? 50 : 49, 11, 0, 0);

    bool held_0 = control_0 > 0.0001f;
    bool held_1 = control_1 > 0.0001f;

    bool pressed_0 = held_0 && 0.0001f > references::last_camera_control_0.read();
    bool pressed_1 = held_1 && 0.0001f > references::last_camera_control_1.read();

    bool switched    = false;
    bool keep_matrix = true;
    i32  mode;

    references::camera_changed.write(0);

    if (held_0 && held_1 && (pressed_0 || pressed_1)) {
        if (references::user_cam_mode.read() == 1) {
            switched = true;
            mode     = 0;

            references::user_cam_controls.write(0);
            references::user_cam_mode    .write(0);
        } else {
            if (references::user_cam_mode.read() == 2)
                keep_matrix = false;

            switched = true;
            mode     = 1;

            references::user_cam_controls.write(1);
            references::user_cam_mode    .write(1);
        }
    } else if (control_2 > 0.0001f &&
               0.0001f > references::last_camera_control_2.read()) {

        mode = references::user_cam_mode.read();

        if (mode == 1) {
            switched    = true;
            mode        = 2;
            keep_matrix = false;

            references::user_cam_controls.write(0);
            references::user_cam_mode    .write(2);
        }
    } else
        mode = references::user_cam_mode.read();

    references::last_camera_control_0.write(control_0);
    references::last_camera_control_1.write(control_1);
    references::last_camera_control_2.write(control_2);

    if (switched) {
        references::camera_changed.write(1);

        // every mode but the chase cam's picks the user cam, so the edit cam's case is never reached
        i32 choice = 1;

        if (!mode) {
            choice             = 0;
            debug_stop_physics = 0;
        }

        const char* message;

        switch (choice) {
            case 1: {
                string_hash user_cam_id;
                user_cam_id.initialize(mash::ALLOCATED, "USER_CAM");

                auto* user_cam = (camera_handle)retail::sub_640AE0(&user_cam_id.source_hash_code, entity_flavor_camera, false); // entity_handle_manager::find_entity

                if (keep_matrix)
                    retail::sub_626BE0((i32)user_cam, (u64*)current_game_camera->my_abs_po); // entity_base::set_rel_po

                retail::sub_97AF20((u32*)this, (u32*)user_cam, true); // game::set_current_camera

                message = "New Camera: User Cam";

                break;
            }

            case 2: {
                string_hash user_cam_id;
                user_cam_id.initialize(mash::ALLOCATED, "USER_CAM");

                auto* user_cam = (camera_handle)retail::sub_640AE0(&user_cam_id.source_hash_code, entity_flavor_camera, false); // entity_handle_manager::find_entity

                retail::sub_626CA0((u32*)user_cam, (u32*)&current_view_camera->my_rel_po->get_position()); // entity_base::set_rel_position
                retail::sub_97AF20((u32*)this, (u32*)user_cam, true);                                      // game::set_current_camera

                message = "New Camera: Edit Cam";

                break;
            }

            default:
                retail::sub_97AF20((u32*)this, (u32*)current_game_camera, true); // game::set_current_camera

                message = "New Camera: Chase Cam";

                break;
        }

        ((void (__thiscall*)(message_board*, mash::string, f32, u32))retail::sub_601CE0)(mb, mash::string(message), 2.0f, 0xFFFFFFFF); // a debug message, empty on pc
    }

    if (retail::sub_96EBF0((i32)input_manager, 93, 11, 0, 0) == 1.0) { // input_mgr::get_control_delta
        geometry_manager::references::scene_analyzer_enabled.write(!geometry_manager::references::scene_analyzer_enabled.read());

        string_hash scene_analyzer_cam_id;
        scene_analyzer_cam_id.initialize(mash::ALLOCATED, "SCENE_ANALYZER_CAM");

        auto* scene_analyzer_cam = (entity*)retail::sub_640AE0(&scene_analyzer_cam_id.source_hash_code, entity_flavor_camera, false); // entity_handle_manager::find_entity

        retail::sub_626CA0((u32*)scene_analyzer_cam, (u32*)&current_game_camera->my_rel_po->get_position()); // entity_base::set_rel_position
    }

    if (retail::sub_96EBF0((i32)input_manager, 62, 11, 0, 0) == 1.0 ||
        retail::sub_96EBF0((i32)input_manager, 63, 11, 0, 0) == 1.0) {

        string_hash user_cam_id;
        user_cam_id.initialize(mash::ALLOCATED, "USER_CAM");

        auto* user_cam = (entity*)retail::sub_640AE0(&user_cam_id.source_hash_code, 28, false); // entity_handle_manager::find_entity

        string_hash chase_cam_id;
        chase_cam_id.initialize(mash::ALLOCATED, "CHASE_CAM");

        auto* chase_cam = (entity*)retail::sub_640AE0(&chase_cam_id.source_hash_code, 28, false);

        retail::sub_626BE0((i32)user_cam, (u64*)chase_cam->my_abs_po); // entity_base::set_rel_po

        references::camera_changed.write(1);
    }

    if (*time_inc != 0.0f && (!debug_stop_physics || debug_single_step)) {
        camera_handle seat   = nullptr;
        bool          seated = true;

        if (!unk_078 && the_world->camera_mgr.animated_camera_enabled)
            seat = the_world->camera_mgr.animated_camera;
        else if (the_world->camera_mgr.marky_camera_enabled)
            seat = the_world->camera_mgr.marky_camera;
        else
            seated = false;

        if (seated) {
            current_game_camera = seat;

            retail::sub_97AF20((u32*)this, (u32*)seat, true); // game::set_current_camera

            current_view_camera->frame_advance(*time_inc);
        } else {
            camera_handle view = current_view_camera;

            if (view != the_world->get_chase_cam_ptr())
                view->frame_advance(*time_inc);

            if (!(current_view_camera->flags & entity_flag_externally_controlled)) {
                camera_handle game_camera = current_game_camera;

                if (game_camera != the_world->get_chase_cam_ptr()) {
                    if (current_view_camera == game_camera)
                        retail::sub_97AF20((u32*)this, (u32*)the_world->get_chase_cam_ptr(), false); // game::set_current_camera

                    current_game_camera = the_world->get_chase_cam_ptr();
                }
            }

            if (!references::g_world_ptr.read()->hero_ptr && the_world->get_chase_cam_ptr())
                the_world->get_chase_cam_ptr()->frame_advance(*time_inc);

            if (camera_handle chase = the_world->get_chase_cam_ptr())
                chase->frame_advance(*time_inc);
        }
    }

    if (current_view_camera != the_world->get_chase_cam_ptr()) {
        the_world->get_chase_cam_ptr()->unk_160();
        the_world->get_chase_cam_ptr()->unk_162(false);
    }

    retail::sub_797950((f32*)current_view_camera, *time_inc); // eases the view camera's fov

    current_view_camera->adjust_geometry_pipe(false);

    if (geometry_manager::references::scene_analyzer_enabled.read()) {
        string_hash scene_analyzer_cam_id;
        scene_analyzer_cam_id.initialize(mash::ALLOCATED, "SCENE_ANALYZER_CAM");

        auto* scene_analyzer_cam = (camera_handle)retail::sub_640AE0(&scene_analyzer_cam_id.source_hash_code, entity_flavor_camera, false); // entity_handle_manager::find_entity

        scene_analyzer_cam->adjust_geometry_pipe(true);
    }

    if (base_camera) {
        string_hash station_cam_id;
        station_cam_id.initialize(mash::ALLOCATED, "STATION_CAM");

        auto* station_cam = (entity*)retail::sub_640AE0(&station_cam_id.source_hash_code, entity_flavor_camera, false); // entity_handle_manager::find_entity

        const vector3d &tracked = base_camera->my_abs_po->get_position();

        f32 dx = (f32)((f64)tracked.x - (f64)base_cam_position.x);
        f32 dy = (f32)((f64)tracked.y - (f64)base_cam_position.y);
        f32 dz = (f32)((f64)tracked.z - (f64)base_cam_position.z);

        base_cam_position.x = (f32)((f64)base_cam_position.x + (f64)dx);
        base_cam_position.y = (f32)((f64)base_cam_position.y + (f64)dy);
        base_cam_position.z = (f32)((f64)base_cam_position.z + (f64)dz);

        const vector3d &station_position = station_cam->my_abs_po->get_position();

        vector3d moved((f32)((f64)station_position.x + (f64)dx),
                       (f32)((f64)station_position.y + (f64)dy),
                       (f32)((f64)station_position.z + (f64)dz));

        retail::sub_626CA0((u32*)station_cam, (u32*)&moved); // entity_base::set_rel_position
    }
}

// sub_97B1A0
void game::load_complete() {
    po transform;
    transform.identity();

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
        transform.get_matrix().w = vector4d(*(vector3d*)((u8*)spawn + 0xD0), 1.0f);
    } else if (the_game->use_default_hero_start_position) {
        string_hash hero_start;
        hero_start.initialize(mash::ALLOCATED, "HERO_START");

        transform = *((entity*)retail::sub_6414A0((u32*)&hero_start))->my_abs_po;
    } else
        transform.get_matrix().w = vector4d(the_game->level.hero_start_position, 1.0f);

    // anything not standing upright is rebuilt on the default axes
    const vector3d &up = treyarch::references::unk_00f4d1ec.get();

    const vector3d &y_facing = transform.get_y_facing();

    if (y_facing.x != up.x || y_facing.y != up.y || y_facing.z != up.z) {
        po upright;
        upright.set_po(treyarch::references::unk_00f4d1e0.get(),
                       up,
                       treyarch::references::unk_00f4d1f8.get(),
                       transform.get_position());

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
        
        chuck::script_library::slc_manager::setup();

        auto* common = amalga::resource_manager::get_partition_pointer(amalga::resource_partition_common);

        references::unk_01111758.write(common);
        references::unk_01111754.write(common->pack_slots[0]);

        common->streamer.load(level.descriptor->level_name.c_str(), 0);

        if (start_only)
            return;
    }

    references::unk_01111758.read()->streamer.flush((amalga::resource_pack_streamer::flush_callback)retail::sub_771950);

    {
        auto* slot = amalga::resource_manager::get_best_context(amalga::resource_partition_common);

        engine_recursive_lock &lock = amalga::resource_manager::references::unk_010300a8.get();

        lock.acquire();
        amalga::resource_manager::references::resource_context_stack_mutex.read()->acquire();

        amalga::resource_manager::push_resource_context(slot);

        amalga::resource_manager::references::resource_context_stack_mutex.read()->release();
        lock.release();
    }

    retail::sub_968670((i32**)&references::g_world_ptr.read()->camera_mgr);

    // the milestone asserts these class value refs exist before any script links; retail drops the results
    references::slc_entity           .read()->find_instance(mash::string("MARKY_CAM"));
    references::slc_entity           .read()->find_instance(mash::string("HERO_START"));
    references::slc_entity           .read()->find_instance(mash::string("ANIMATED_CAM"));
    references::slc_script_controller.read()->find_instance(mash::string("CONTROLLER_1"));

    chuck::vm::script_manager &scripts = *chuck::vm::script_manager::inst();

    scripts.init_game_var();
    mission_manager::inst()->setup_game_var_refs();

    {
        mash::string busy_teaching("busy_teaching");

        references::g_world_ptr.read()->gv_busy_teaching =
            (f32*)scripts.get_game_var_address(busy_teaching, nullptr);
    }

    if (scripts.is_loadable("init_gv")) {
        string_hash init_gv;
        init_gv.initialize(mash::ALLOCATED, "init_gv");

        string_hash init_sv;
        init_sv.initialize(mash::ALLOCATED, "init_sv");

        scripts.load(init_gv,
                     chuck::vm::script_manager_load_flag_set_stall_for_breakpoints_flag,
                     references::unk_01111754.read(),
                     string_hash());

        scripts.load(init_sv,
                     chuck::vm::script_manager_load_flag_set_stall_for_breakpoints_flag,
                     references::unk_01111754.read(),
                     string_hash());

        scripts.run_single_exec(init_gv, string_hash(), 0.0f, false);
        scripts.run_single_exec(init_sv, string_hash(), 0.0f, false);

        scripts.un_load(init_gv, false, string_hash());
        scripts.un_load(init_sv, false, string_hash());

        // retail follows with an empty call (nullsub_1)
    }

    amalga::resource_pack_slot* common_slot = references::unk_01111754.read();

    {
        resource_key sin_key;
        retail::sub_739B10((u32*)&sin_key, level.name.c_str(), 2);
        retail::sub_73DDE0((i32)common_slot->pack_directory->get_resource(sin_key, nullptr, nullptr));
    }

    const char* level_name = level.descriptor->level_name.c_str();

    {
        string_hash level_hash;
        level_hash.initialize(mash::ALLOCATED, level_name);

        resource_key key;
        key.hash = string_hash();
        key.set(level_hash, 7);

        speedtrees = (speedtree_collection*)amalga::resource_manager::get_resource(key, nullptr, nullptr);

        if (speedtrees)
            retail::sub_7D9CA0((u32*)speedtrees);
    }

    mission_manager::inst()->setup_global_data(level_name, references::unk_01111754.read());
    retail::sub_7F7760((i32)quest_manager::inst(), (i32)level_name, (i32)references::unk_01111754.read());
    retail::sub_90FCB0((i32*)references::unk_010fa26c.read(), (i32)level_name, (i32)references::unk_01111754.read());
    retail::sub_7EB4D0((i32*)references::environment_progression_state.read(), (i32)level_name, (i32)references::unk_01111754.read());

    if (references::region_spawns_enabled.read() && region_spawn_manager::inst())
        region_spawn_manager::inst()->load(level_name, references::unk_01111754.read());

    {
        string_hash vehicles_hash;
        vehicles_hash.initialize(mash::ALLOCATED, "vehicles");

        resource_key key;
        key.hash = string_hash();
        key.set(vehicles_hash, 28);

        references::vehicle_model_depot.write((vehicle_model_depot*)references::unk_01111754.read()->pack_directory->get_resource(key, nullptr, nullptr));
    }

    retail::sub_7B7570();
    retail::sub_5A0F70();
    retail::sub_592FF0();

    retail::sub_7ACDC0((u32*)the_world->the_terrain, (i32 (*)())&game::load_complete);
    retail::sub_77BAB0((u32*)references::g_world_ptr.read()->the_terrain);

    {
        string_hash global_script;
        global_script.initialize(mash::ALLOCATED, references::g_world_ptr.read()->script_mgr.global_script_filename.c_str());

        auto* slot = amalga::resource_manager::get_best_context(amalga::resource_partition_common);

        scripts.load(global_script, chuck::vm::script_manager_load_flag_master, slot, string_hash());
    }

    retail::sub_95CC60((i32*)&the_world->render_mgr);

    {
        mash::string hero_name((const char*)retail::sub_77CBC0((u32*)data));

        matrix4x4 start_transform;
        auto* hero_transform = (po*)retail::sub_95C730((u32*)the_world, (i32)&start_transform); // the HERO_START transform

        the_world->add_player(hero_name, *hero_transform);
    }

    if (i_quit)
        return;

    current_game_camera = the_world->get_chase_cam_ptr();

    auto* chase = the_world->get_chase_cam_ptr();

    if (chase) {
        current_view_camera = chase;

        if (chase->is_a_game_camera())
            chase->invalidate();

        if (!current_game_camera && chase->is_a_game_camera())
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

// sub_7371B0
void game::swap_hero_costume(const mash::string &new_hero_packfile, bool keep_position) {
    mash::string hero_name(new_hero_packfile);

    po transform;
    transform.identity();

    po start_transform;

    const po* source = keep_position ?
        references::g_world_ptr.read()->hero_ptr->my_abs_po :
        (po*)retail::sub_95C730((u32*)references::g_world_ptr.read(), (i32)&start_transform); // the HERO_START transform

    transform = *source;

    std::strncmp(hero_name.c_str(), "ch_peter", 0xFFFF);

    retail::sub_968320((i32)references::g_world_ptr.read(), 1);

    references::g_world_ptr.read()->add_player(hero_name, transform);
}
