#include "retail.hh"
#include "game/amalga/resource_manager.hh"
#include "game/app.hh"
#include "game/container/fixed_vector.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/game.hh"
#include "game/game_meter_manager.hh"
#include "game/math/references.hh"
#include "game/mission/mission_manager.hh"
#include "game/sm_mash.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/path.hh"
#include "game/wds/camera/camera.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/interface/generic_interface.hh"
#include "game/wds/entity/interface/physical_interface.hh"
#include "game/wds/entity/interface/player_interface.hh"
#include "game/wds/entity/interface/standing_interface.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"
#include "treyarch/mm/mm.hh"

using namespace treyarch;

// sub_97A860
world_dynamics_system::world_dynamics_system() : last_hero_ai_disable_count(0),
                                                 saved_hero_suspended(false),
                                                 saved_hero_invulnerable(false),
                                                 saved_hero_enabled(true) {

    unk_01c          = 0;
    gv_busy_teaching = nullptr;
    unk_139          = false;
    hero_ptr         = nullptr;
    chase_cam_ptr    = nullptr;
    unk_13c          = -1;
    unk_134          = 4;
    the_terrain      = nullptr;
    last_snm_handle  = 0;

    sm_generate_vtable();

    retail::sub_A721A0(); // the ise library's own registration list
    retail::sub_600E20();
}

// sub_97A400
world_dynamics_system::~world_dynamics_system() {
    unk_128.clear();

    // a scene anim's destructor takes its own entry out of this map; the walk steps on from the erased node
    for (scene_anim_map_t::node* where = scene_anim_map.begin(); where != scene_anim_map.end(); where = where->next())
        delete where->value.second;

    scene_anim_map.clear();

    retail::sub_7FEA00((i32)&ent_mgr); // wds_entity_manager, which its destructor runs again

    hero_ptr = nullptr;

    if (the_terrain) {
        retail::sub_7AC480((i32**)the_terrain); // ~terrain
        mm::free(the_terrain);
    }

    hero_ptr      = nullptr;
    chase_cam_ptr = nullptr;

    references::unk_01113240.write(nullptr);

    retail::sub_824F10(); // clears the master script's global object and instance
    retail::sub_646780(); // empties the list of grenades out of their cache
}

// sub_967E00
void world_dynamics_system::add_player(const mash::string &filename, const po &transform) {
    if (!filename.empty())
        hero_name = filename;

    if (!hero_ptr && !filename.empty())
        retail::sub_76F860((u32*)references::game.read(), filename.c_str()); // game::load_hero_packfile

    if (references::game.read()->i_quit)
        return;

    mash::string hero_instance("HERO");

    {
        amalga::push_resource_context_stack_object context(amalga::resource_manager::get_best_context(amalga::resource_partition_common));

        mash::string directory("");

        string_hash instance_name;
        instance_name.initialize(mash::ALLOCATED, hero_instance.c_str());

        string_hash template_name;
        template_name.initialize(mash::ALLOCATED, hero_name.c_str());

        hero_ptr = (entity*)retail::sub_804460((u32*)&references::g_world_ptr.read()->ent_mgr,
                                               (i32)template_name.source_hash_code,
                                               &instance_name.source_hash_code,
                                               (u64*)&transform,
                                               (i32)&directory,
                                               1,
                                               0,
                                               0); // built from its template
    }

    retail::sub_601C70((u32*)hero_ptr, true);

    const vector3d &position = hero_ptr->my_rel_po->get_position();
    const vector3d &lift     = references::unk_00f4ce5c.get();

    vector3d lifted((f32)((f64)position.x + (f64)lift.x),
                    (f32)((f64)position.y + (f64)lift.y),
                    (f32)((f64)position.z + (f64)lift.z));

    retail::sub_626CA0((u32*)hero_ptr, (u32*)&lifted); // entity_base::set_rel_position

    {
        mash::string chase_cam_name("CHASE_CAM");

        void* memory = mm::allocate(0x390);

        if (memory) {
            string_hash camera_id;
            camera_id.initialize(mash::ALLOCATED, chase_cam_name.c_str());

            chase_cam_ptr = (camera*)retail::sub_9C4B90((i32)memory, (i32*)&camera_id, (i32)hero_ptr); // spiderman_camera constructor
        } else
            chase_cam_ptr = nullptr;

        retail::sub_803E80((u32**)&references::g_world_ptr.read()->ent_mgr, 0, (i32*)chase_cam_ptr);

        references::unk_01113240.write(chase_cam_ptr);
    }

    retail::sub_97AF20((u32*)references::game.read(), (u32*)chase_cam_ptr, true); // game::set_current_camera

    hero_ptr->set_visible(true, false);
    ((actor*)hero_ptr)->invalidate_frame_delta();
    retail::sub_640D20((u8*)hero_ptr);
    hero_ptr->compute_sector(the_terrain, false, nullptr);

    retail::sub_991A30((i32*)hero_ptr->player_ifc());

    *(entity**)(references::unk_010f9bd8.read() + 0x04) = hero_ptr;

    game_meter_manager* meters = references::game_meter_manager.read();

    retail::sub_933B40((u32*)meters, (i32)hero_ptr); // stores the hero and hands it to every meter

    if (hero_ptr->player_ifc()->unk_077() == 1) {
        retail::sub_93F540((u32*)meters, (i32)references::redsuit_behaviors.read().source_hash_code);
        retail::sub_93F540((u32*)meters, (i32)references::blacksuit_behaviors.read().source_hash_code);

        string_hash behaviors = hero_ptr->player_ifc()->is_in_rvb_black_mode() ?
            references::blacksuit_behaviors.read() :
            references::redsuit_behaviors.read();

        retail::sub_93F600((u32*)meters, (i32)behaviors.source_hash_code);
    }

    references::frontend.get().igo->register_player_callbacks(hero_ptr);
    references::frontend.get().igo->entity_tracker_mgr->set_hero_tracker(hero_ptr->my_handle);

    if (hero_ptr->has_sound_ifc()) {
        string_hash event;
        event.initialize(mash::ALLOCATED, "start_hero");

        retail::sub_65FA00((i32)hero_ptr->my_ifc_storage->get_ifc(entity_ifc_sound), (i32)event.source_hash_code, false); // sound_interface: post event
    }

    if (saved_hero_suspended) {
        hero_ptr->suspend(true);
        --last_hero_ai_disable_count;
    } else
        hero_ptr->unsuspend(true);

    if (saved_hero_invulnerable)
        hero_ptr->ext_flags |= entity_ext_flag_invulnerable;
    else
        hero_ptr->ext_flags &= ~entity_ext_flag_invulnerable;

    hero_ptr->player_ifc()->set_enabled(saved_hero_enabled);

    for (; last_hero_ai_disable_count > 0; --last_hero_ai_disable_count) {
        if (((actor*)hero_ptr)->get_ai_core())
            ((actor*)hero_ptr)->get_ai_core()->push_disable(0);
    }

    entity* hero = hero_ptr;

    if (hero->has_ifc(entity_ifc_rpg)) {
        generic_interface* rpg = hero->my_ifc_storage->get_ifc(entity_ifc_rpg);

        ((void (__thiscall*)(generic_interface*))(*(void***)rpg)[19])(rpg); // slot 19
    }

    retail::sub_9846F0((u32*)mission_manager::inst());
}

// sub_967860
void world_dynamics_system::malor_point(const vector3d &xyz, bool translate_cam, bool locking) {
    if (locking) {
        container::fixed_vector<region*, 15> regions;

        retail::sub_789E20((i32*)the_terrain, (i32)&xyz, (i32)&regions); // the regions under the point
    }

    physical_interface* physical = hero_ptr->physical_ifc();
    ai_core*            core     = ((actor*)hero_ptr)->get_ai_core();

    bool restart_biped_physics = false;

    hero_ptr->player_ifc()->unk_369(0, 0);

    if (physical->is_flagged(physical_interface_flag_biped_running)) {
        physical->stop_biped_physics(false);
        restart_biped_physics = true;
    }

    if (core) {
        core->stop_movement();
        core->push_disable(1);
    }

    po new_po = *hero_ptr->my_abs_po;

    const vector3d &old_position = new_po.get_position();

    vector3d delta((f32)((f64)xyz.x - (f64)old_position.x),
                   (f32)((f64)xyz.y - (f64)old_position.y),
                   (f32)((f64)xyz.z - (f64)old_position.z));

    new_po.set_position(xyz);

    if (hero_ptr->has_standing_ifc()) {
        hero_ptr->standing_ifc()->set_control_parent(nullptr);
        hero_ptr->standing_ifc()->clear_effectively_standing();
    }

    if (hero_ptr->my_parent)
        fast_po_mul(new_po, new_po, hero_ptr->my_parent->my_abs_po->inverse());

    retail::sub_626BE0((i32)hero_ptr, (u64*)&new_po); // entity_base::set_rel_po

    ((actor*)hero_ptr)->save_last_collision_free_state();
    ((actor*)hero_ptr)->invalidate_frame_delta();

    physical->cancel_all_velocity(references::zerovec.get(), true);

    references::g_world_ptr.read()->hero_ptr->player_ifc()->unk_140();
    references::g_world_ptr.read()->hero_ptr->player_ifc()->unk_141();

    if (translate_cam) {
        const vector3d &camera_position = chase_cam_ptr->my_rel_po->get_position();

        vector3d moved((f32)((f64)camera_position.x + (f64)delta.x),
                       (f32)((f64)camera_position.y + (f64)delta.y),
                       (f32)((f64)camera_position.z + (f64)delta.z));

        retail::sub_626CA0((u32*)chase_cam_ptr, (u32*)&moved); // entity_base::set_rel_position
    } else {
        vector3d forward = math::unitize(new_po.get_z_facing());

        vector3d ahead((f32)((f64)forward.x * 2.0),
                       (f32)((f64)forward.y * 2.0),
                       (f32)((f64)forward.z * 2.0));

        vector3d up = math::unitize(new_po.get_y_facing());

        vector3d offset((f32)((f64)(f32)((f64)up.x * 2.0) - (f64)ahead.x),
                        (f32)((f64)(f32)((f64)up.y * 2.0) - (f64)ahead.y),
                        (f32)((f64)(f32)((f64)up.z * 2.0) - (f64)ahead.z));

        vector3d behind = hero_ptr->my_abs_po->slow_xform(offset);

        retail::sub_626CA0((u32*)chase_cam_ptr, (u32*)&behind); // entity_base::set_rel_position
    }

    chase_cam_ptr->invalidate();

    if (restart_biped_physics)
        retail::sub_67FB10((u32*)physical); // physical_interface::start_biped_physics

    core->pop_disable(1);
}
