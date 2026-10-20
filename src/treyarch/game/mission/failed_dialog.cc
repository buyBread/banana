#include "retail.hh"
#include "treyarch/chuck/vm/script_executable.hh"
#include "treyarch/chuck/vm/script_object.hh"
#include "treyarch/game/frontend/frontend_manager.hh"
#include "treyarch/game/frontend/ui_frontend.hh"
#include "treyarch/game/mission/mission_manager.hh"
#include "treyarch/game/quest_manager.hh"
#include "treyarch/game/script/script_access.hh"
#include "treyarch/game/wds/entity/entity.hh"
#include "treyarch/game/wds/references.hh"
#include "treyarch/game/wds/world_dynamics_system.hh"
#include "treyarch/shared/hash/string_hash.hh"

using namespace treyarch;

// sub_97F930
void mission_manager::play_failed_music() {
    string_hash music = references::mission_failed_stinger.read();

    i32 function_index = script::find_instance_function(references::mission_manager_get_failed_music.read(),
                                                        current_mission.exec->get_global_script_object()->global_instance);

    if (function_index != -1) {
        // the mission's exec may be suspended; lift it for the call
        i32 suspensions = 0;

        while (current_mission.exec->suspend_count > 0) {
            --current_mission.exec->suspend_count;
            ++suspensions;
        }

        chuck::vm::vm_thread* thread = script::create_thread(function_index,
                                                             current_mission.exec->get_global_script_object()->global_instance,
                                                             0);

        if (thread) {
            string_hash result;

            if (script::run_thread_with_return(thread, &result))
                music = result;
        }

        for (; suspensions > 0; --suspensions)
            ++current_mission.exec->suspend_count;
    }

    retail::sub_900D60(music.source_hash_code, 0x10000000, 0); // post the sound event on the global game object
}

// sub_981F20
void mission_manager::quit_failed_mission() {
    retail::sub_7FFF00((i32*)quest_manager::inst(), (i32)&current_mission);
    retail::sub_76E110((u32*)retail::sub_734220(), current_mission.mission_index);
    retail::sub_75F100(retail::sub_7685B0());

    flags &= ~mission_manager_flag_unk_00400000;

    {
        f32 detail_count = 0.0f;

        script_arg count;
        count.type  = 1;
        count.value = &detail_count;

        string_hash clean_up_random_details;
        clean_up_random_details.initialize(mash::ALLOCATED, "clean_up_random_details(num)");

        script::call_function(nullptr,
                              clean_up_random_details,
                              count,
                              script_arg(),
                              script_arg(),
                              script_arg(),
                              script_arg(),
                              script_arg(),
                              script_arg(),
                              script_arg());
    }

    retail::sub_707E90((u8*)references::frontend.get().igo->loading_screen, 0.0f, 0, 0, 0, 0, 45.0f);

    {
        i32 max_hit_points = references::g_world_ptr.read()->hero_ptr->damage_ifc()->get_max_hit_points();

        references::g_world_ptr.read()->hero_ptr->damage_ifc()->set_hit_points(max_hit_points);
    }

    bool fall_back = true;

    if (current_mission.exec) {
        // the mission's exec may be suspended; lift it for the call
        i32 suspensions = 0;

        while (current_mission.exec->suspend_count > 0) {
            --current_mission.exec->suspend_count;
            ++suspensions;
        }

        i32 function_index = script::find_instance_function(references::mission_manager_get_restart_position.read(),
                                                            current_mission.exec->get_global_script_object()->global_instance);

        if (function_index != -1) {
            chuck::vm::vm_thread* thread = script::create_thread(function_index,
                                                                 current_mission.exec->get_global_script_object()->global_instance,
                                                                 0);

            vector3 position;

            if (thread && script::run_thread_with_return(thread, &position)) {
                player_restart_position = position;

                const vector3 &z_axis = references::g_world_ptr.read()->hero_ptr->my_abs_po->matrix.z_row();

                player_restart_xz_facing   = z_axis;
                player_restart_xz_facing.y = 0.0f;

                retail::sub_402050((f32*)&player_restart_xz_facing);

                flags     |= mission_manager_flag_need_to_malor;
                fall_back  = false;
            }
        }

        for (; suspensions > 0; --suspensions)
            ++current_mission.exec->suspend_count;

        if (!fall_back)
            return;
    }

    if (flags & mission_manager_flag_using_icon) {
        if (!(flags & mission_manager_flag_need_to_malor) &&
            (state == mission_manager_state_running_mission ||
             state == mission_manager_state_running_mission_failed_dialog)) {

            player_restart_position  = current_mission.key_position;
            player_restart_xz_facing = vector3(0.0f, 0.0f, 1.0f);

            flags |= mission_manager_flag_need_to_malor;
        }
    } else if (!(flags & mission_manager_flag_need_to_malor)) {
        using restart_from_registry = bool (__thiscall*)(mission_manager*, vector3*, vector3*);

        if (((restart_from_registry)retail::sub_980A50)(this, &player_restart_position, &player_restart_xz_facing))
            flags |= mission_manager_flag_need_to_malor;
    }
}

// sub_985EE0
void mission_manager::retry_failed_mission() {
    {
        f32 detail_count = 0.0f;

        script_arg count;
        count.type  = 1;
        count.value = &detail_count;

        string_hash clean_up_random_details;
        clean_up_random_details.initialize(mash::ALLOCATED, "clean_up_random_details(num)");

        script::call_function(nullptr,
                              clean_up_random_details,
                              count,
                              script_arg(),
                              script_arg(),
                              script_arg(),
                              script_arg(),
                              script_arg(),
                              script_arg(),
                              script_arg());
    }

    retail::sub_707E90((u8*)references::frontend.get().igo->loading_screen, 0.0f, 0, 0, 0, 0, 45.0f);

    {
        i32 max_hit_points = references::g_world_ptr.read()->hero_ptr->damage_ifc()->get_max_hit_points();

        references::g_world_ptr.read()->hero_ptr->damage_ifc()->set_hit_points(max_hit_points);
    }

    if (references::g_world_ptr.read()->hero_ptr->physical_ifc()->is_flagged(physical_interface_flag_biped_running))
        references::g_world_ptr.read()->hero_ptr->physical_ifc()->stop_biped_physics();

    references::g_world_ptr.read()->hero_ptr->physical_ifc()->cancel_all_velocity(references::unk_011117a0.get());

    if (player_reflexes* reflexes = references::g_world_ptr.read()->hero_ptr->player_ifc()->get_reflexes())
        reflexes->disable(false);

    if (player_spider_senses* senses = references::g_world_ptr.read()->hero_ptr->player_ifc()->get_spider_senses())
        senses->disable(true, false);

    retail::sub_985A10((i32)this); // restart
}
