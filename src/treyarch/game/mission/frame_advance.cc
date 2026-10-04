#include "retail.hh"
#include "treyarch/app/app.hh"
#include "treyarch/chuck/vm/script_manager.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/mission/mission_manager.hh"
#include "treyarch/game/wds/ai/ai_core.hh"
#include "treyarch/game/wds/entity/actor.hh"
#include "treyarch/game/wds/references.hh"
#include "treyarch/game/wds/world_dynamics_system.hh"
#include "treyarch/shared/dinkumware/list.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mutex.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace references {
    util::memory_reference<string_hash> unk_0102c2b8 { 0x0102C2B8 };
    util::memory_reference<string_hash> unk_0102c75c { 0x0102C75C };
}} // treyarch::references

using namespace treyarch;

// sub_986520
void mission_manager::frame_advance(f32 time_inc) {
    advance_act_transition();

    switch (sky_swap_state) {
        case 0:
            check_sky_name();

            break;

        case 1:
            if (retail::sub_902810()) // is_bank_loader_idle
                sky_swap_state = 2;

            break;

        case 2:
            load_sky_pack();

            break;

        case 3:
            apply_sky_pack();

            break;
    }

    game* the_game = references::game.read();

    if (the_game->game_paused || !game_time_var || the_game->get_cur_state() != 9) {
        frame_clock.reset();

        return;
    }

    real_time_timer = (f32)((f64)real_time_timer + (f64)time_inc);

    bool real_time_ticked = false;

    if (real_time_timer >= 1.0f) {
        real_time_ticked = true;

        ++real_time;
        *real_time_var = (f32)real_time;

        real_time_timer = (f32)((f64)real_time_timer - 1.0);
    }

    frame_advance_game_time(time_inc);

    if ((flags & mission_manager_flag_unk_10000000) && !retail::sub_77C9E0(references::game.read()->data)) {
        retail::sub_79CAB0((u32*)references::game.read()->data, (i32)this);

        flags &= ~mission_manager_flag_unk_10000000;
    }

    if (real_time_ticked) {
        update_hero_proximity_event();
        update_hero_tracking_flags();
        run_district_scripts();
    }

    switch (state) {
        case mission_manager_state_initial_startup:
            process_state_initial_startup(time_inc);

            break;

        case mission_manager_state_idle:
            process_state_idle(time_inc);

            break;

        case mission_manager_state_start_loading:
            process_state_start_loading();

            break;

        case mission_manager_state_loading:
            process_state_loading();

            break;

        case mission_manager_state_running_mission:
            process_state_running_mission();

            break;

        case mission_manager_state_unk_5:
            process_state_unk_5();

            break;

        case mission_manager_state_unk_6:
            process_state_unk_6();

            break;

        case mission_manager_state_start_unloading:
            process_state_start_unloading();

            break;

        case mission_manager_state_unloading:
            process_state_unloading();

            break;

        case mission_manager_state_maloring_player_wait_for_blackscreen:
            process_state_maloring_player_wait_for_blackscreen(time_inc);

            break;

        case mission_manager_state_maloring_player_wait_for_district:
            process_state_maloring_player_wait_for_district();

            break;

        case mission_manager_state_start_mission_failed_dialog:
            process_state_start_mission_failed_dialog();

            break;

        case mission_manager_state_running_mission_failed_dialog:
            process_state_running_mission_failed_dialog();

            break;

        case mission_manager_state_start_mission_succeeded_dialog:
            state = mission_manager_state_running_mission_succeeded_dialog;

            break;

        case mission_manager_state_running_mission_succeeded_dialog:
            process_state_running_mission_succeeded_dialog();

            break;

        case mission_manager_state_blackscreen_on:
            process_state_blackscreen_on();

            break;

        case mission_manager_state_wait_for_blackscreen_on:
            process_state_wait_for_blackscreen_on(time_inc);

            break;
    }

    frame_clock.reset();
}

// sub_981D50
void mission_manager::update_hero_proximity_event() {
    entity* hero = references::g_world_ptr.read()->hero_ptr;

    auto* owner = (u8*)((actor*)hero)->get_ai_core()->get_info_node(info_node_type_combat_target);

    engine_recursive_lock &lock = *(engine_recursive_lock*)(owner + 0x98);

    lock.acquire();

    vector3 hero_position = hero->my_abs_po->get_position();
    bool    hero_nearby   = false;

    auto* nearby = *(dinkumware::list<u32*>**)(owner + 0x94);

    for (auto* node = nearby->begin(); node != nearby->end(); node = node->next) {
        const vector3 &position = ((entity*)retail::sub_4C3A70(node->value))->my_abs_po->get_position();

        f32 dx = (f32)((f64)position.x - (f64)hero_position.x);
        f32 dy = (f32)((f64)position.y - (f64)hero_position.y);
        f32 dz = (f32)((f64)position.z - (f64)hero_position.z);

        if ((f32)((f64)dx * (f64)dx + (f64)dy * (f64)dy + (f64)dz * (f64)dz) <= 10000.0f) {
            hero_nearby = true;

            break;
        }
    }

    if (!unk_200) {
        if (hero_nearby) {
            unk_200 = 1;

            retail::sub_601750((u32*)hero, references::unk_0102c2b8.read().source_hash_code);
        }
    } else if (!hero_nearby) {
        unk_200 = 0;

        retail::sub_601750((u32*)hero, references::unk_0102c75c.read().source_hash_code);
    }

    lock.release();
}

// sub_97F7B0
void mission_manager::update_hero_tracking_flags() {
    u32 current_flags = flags;

    if (!(current_flags & mission_manager_flag_unk_00004000) && !(current_flags & mission_manager_flag_unk_00008000))
        return;

    entity* hero = references::g_world_ptr.read()->hero_ptr;

    if (!hero)
        return;

    if (current_flags & mission_manager_flag_unk_00004000) {
        bool in_district = false;

        if (hero_region) {
            for (i32 index = 0; index < district_count; ++index) {
                if (district_containers[index].region == hero_region) {
                    in_district = true;

                    break;
                }
            }
        }

        if (!in_district) {
            flags       = current_flags & ~mission_manager_flag_unk_00004000;
            hero_region = nullptr;
        }
    }

    if (!(flags & mission_manager_flag_unk_00008000))
        return;

    const vector3 &position = hero->my_abs_po->get_position();

    f32 dx = (f32)((f64)position.x - (f64)hero_reference_position.x);
    f32 dy = (f32)((f64)position.y - (f64)hero_reference_position.y);
    f32 dz = (f32)((f64)position.z - (f64)hero_reference_position.z);

    if ((f32)((f64)dx * (f64)dx + (f64)dy * (f64)dy + (f64)dz * (f64)dz) > 90000.0f) {
        flags                   &= ~mission_manager_flag_unk_00008000;
        hero_reference_position  = references::unk_011117a0.read();
    }
}

// sub_97F550
void mission_manager::run_district_scripts() {
    for (i32 index = 0; index < district_count; ++index) {
        mission_district_info &district = district_containers[index];

        if (!district.gen_dis_exec || district.gen_dis_script_frame_count <= 0)
            continue;

        if (--district.gen_dis_script_frame_count)
            continue;

        while (retail::sub_A1FA30((u32*)district.gen_dis_exec))
            retail::sub_A1B960((i32)&chuck::vm::script_manager::get(), (i32*)&district.gen_dis_exec_name, 0, 0.0f, 0);

        retail::sub_97F4F0((char*)this, index);
    }
}
