#include <cstdlib>

#include "retail.hh"
#include "treyarch/amalga/resource_manager.hh"
#include "treyarch/amalga/resource_pack_slot.hh"
#include "treyarch/app/app.hh"
#include "treyarch/chuck/vm/script_executable.hh"
#include "treyarch/chuck/vm/script_instance.hh"
#include "treyarch/chuck/vm/script_manager.hh"
#include "treyarch/game/cutscene/cutscene_player.hh"
#include "treyarch/game/cutscene/toa_cutscene.hh"
#include "treyarch/game/event/event_manager.hh"
#include "treyarch/game/frontend/frontend_manager.hh"
#include "treyarch/game/frontend/igo/igo_3d_loading_screen.hh"
#include "treyarch/game/frontend/igo/igo_3d_pauseless_dialog_widget.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/glass_house_manager.hh"
#include "treyarch/game/mission/mission_manager.hh"
#include "treyarch/game/quest_manager.hh"
#include "treyarch/game/region_pack_manager.hh"
#include "treyarch/game/region_spawn_manager.hh"
#include "treyarch/game/spawn_table_manager.hh"
#include "treyarch/game/summon_state.hh"
#include "treyarch/game/wds/ai/ai_core.hh"
#include "treyarch/game/wds/camera/camera.hh"
#include "treyarch/game/wds/entity/actor.hh"
#include "treyarch/game/wds/references.hh"
#include "treyarch/game/wds/world_dynamics_system.hh"
#include "treyarch/game/zombie_manager.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/math/references.hh"
#include "treyarch/shared/math/types/matrix4x4.hh"
#include "treyarch/shared/memory/heap.hh"
#include "treyarch/soap/online.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace references {
    util::memory_reference<string_hash> zoom_map_poi_selected                   { 0x0102C3E4 };
    util::memory_reference<string_hash> zoom_map_poi_unselected                 { 0x0102CC60 };
    util::memory_reference<string_hash> last_mission_long_time_ago              { 0x0102C368 };
    util::memory_reference<string_hash> last_successful_mission_long_time_ago   { 0x0102C5E8 };
    util::memory_reference<string_hash> last_selected_zoom_map_destination_long { 0x0102CC10 };
    util::memory_reference<string_hash> unk_0102c498                            { 0x0102C498 };
    util::memory_reference<string_hash> unk_0102cc20                            { 0x0102CC20 };

    util::memory_reference<u8> unk_00be7455 { 0x00BE7455 };
    util::memory_reference<u8> unk_0102fed8 { 0x0102FED8 };
    util::memory_reference<u8> unk_00e67164 { 0x00E67164 };

    util::memory_reference<f32> unk_00fc7a38 { 0x00FC7A38 };
}} // treyarch::references

using namespace treyarch;

// sub_9847F0
void mission_manager::process_state_initial_startup(f32 time_inc) {
    if (startup_timer > 0.0f) {
        startup_timer = (f32)((f64)startup_timer - (f64)time_inc);

        return;
    }

    flags         |= mission_manager_flag_unk_02000000;
    startup_timer  = 1.0f;
    state          = mission_manager_state_idle;

    update_eligible_missions();

    while (flags & mission_manager_flag_unk_00200000)
        update_eligible_missions();

    if (zoom_map_poi_selected_callback_id == references::unk_00bb6d68.read())
        zoom_map_poi_selected_callback_id = event_manager::add_callback(references::zoom_map_poi_selected.read(),
                                                                        arch_base_vhandle(),
                                                                        on_zoom_map_poi_selected,
                                                                        nullptr,
                                                                        false);

    if (zoom_map_poi_unselected_callback_id == references::unk_00bb6d68.read())
        zoom_map_poi_unselected_callback_id = event_manager::add_callback(references::zoom_map_poi_unselected.read(),
                                                                          arch_base_vhandle(),
                                                                          on_zoom_map_poi_unselected,
                                                                          nullptr,
                                                                          false);

    if (!selected_poi_icon) {
        void* allocation = memory::heap::allocate(0x40);
        selected_poi_icon = allocation ? retail::sub_70C4A0((u8*)allocation, 6) : nullptr;
    }
}

// sub_9860E0
void mission_manager::process_state_idle(f32 time_inc) {
    state_timer         = (f32)((f64)time_inc + (f64)state_timer);
    idle_event_cooldown = (f32)((f64)idle_event_cooldown - (f64)time_inc);

    if (0.0f > idle_event_cooldown)
        idle_event_cooldown = 0.0f;

    mission_finished_screen_has_appeared = 0;

    if (flags & mission_manager_flag_unk_02000000) {
        flags &= ~mission_manager_flag_unk_02000000;

        if (references::region_spawns_enabled.read() && region_spawn_manager::inst() &&
            spawn_table_manager::inst()) {

            retail::sub_9547F0((i32)spawn_table_manager::inst()->unk_04c);
            retail::sub_94F010((i32)region_pack_manager::inst());
        }
    }

    if (flags & mission_manager_flag_need_to_malor) {
        retail::sub_707E90((u8*)references::frontend.get().igo->loading_screen, 0.0f, 0, 0, 0, 0, 45.0f);

        flags       &= ~mission_manager_flag_need_to_malor;
        state_timer  = 0.0f;
        state        = mission_manager_state_maloring_player_wait_for_blackscreen;

        return;
    }

    if (flags & mission_manager_flag_unk_20000000) {
        state_timer  = 0.0f;
        flags       &= ~mission_manager_flag_unk_20000000;
        state        = mission_manager_state_initial_startup;

        return;
    }

    if ((!((i32)state_timer % 30) || (flags & mission_manager_flag_unk_00200000)) &&
        !(flags & mission_manager_flag_mission_ready)) {

        update_eligible_missions();
        state_timer = 0.0f;
    }

    retail::sub_730F10((u32*)references::glass_house_manager.read(), 1); // set_glass_house_level

    if (!(flags & mission_manager_flag_mission_ready) && !auto_launch_instances->size() && !unk_295 &&
        real_time - last_mission_time > last_mission_interval) {

        event_manager::raise_event(references::last_mission_long_time_ago.read(), arch_base_vhandle());

        last_mission_time   = real_time;
        idle_event_cooldown = 10.0f;
    }

    if (!(flags & mission_manager_flag_mission_ready) && !auto_launch_instances->size() && !unk_295 &&
        real_time - last_successful_mission_time > last_successful_mission_interval) {

        event_manager::raise_event(references::last_successful_mission_long_time_ago.read(), arch_base_vhandle());

        last_successful_mission_time = real_time;
        idle_event_cooldown          = 10.0f;
    }

    if (!(flags & mission_manager_flag_mission_ready) && !auto_launch_instances->size() && !unk_295 &&
        real_time - last_zoom_map_selection_time > last_zoom_map_selection_interval) {

        event_manager::raise_event(references::last_selected_zoom_map_destination_long.read(), arch_base_vhandle());

        last_zoom_map_selection_time = real_time;
        idle_event_cooldown          = 10.0f;
    }

    if (idle_event_cooldown > 0.0f && !auto_launch_instances->size()) {
        if (!(flags & mission_manager_flag_mission_ready)) {
            update_mission_icons();

            return;
        }
    } else if (!(flags & mission_manager_flag_mission_ready)) {
        if (auto_launch_instances->size()) {
            const chuck::vm::script_instance* instance = auto_launch_instances->begin()->value;

            retail::sub_5A85E0((u32*)auto_launch_instances); // pop_front
            launch_auto_launch_instance(instance);
        } else {
            update_mission_icons();
            check_mission_triggers();
        }
    }

    if (flags & mission_manager_flag_mission_ready) {
        current_mission = next_mission;

        state = mission_manager_state_start_loading;

        if (flags & mission_manager_flag_using_icon) {
            u32 header_flags = ((mission_header_instance_base*)current_mission.mission_header_instance->data.buffer)->flags;

            if ((!(header_flags & mission_header_flag_unk_00004000) && !(header_flags & mission_header_flag_unk_02000000)) ||
                (flags & mission_manager_flag_unk_08000000))
                state = mission_manager_state_blackscreen_on;
        }

        if (flags & mission_manager_flag_mission_ready)
            return;
    }

    if (unk_295) {
        unpause_and_notify_frontend();
        unk_295 = 0;
    }
}

// sub_984960
void mission_manager::process_state_start_loading() {
    update_mission_icons();

    if (references::summon_state.read()) {
        retail::sub_90FC00(references::summon_state.read());

        if (retail::sub_920420((u32*)region_pack_manager::inst()))
            return;
    }

    if (references::region_spawns_enabled.read() && region_spawn_manager::inst()) {
        retail::sub_951C40((i32)region_pack_manager::inst(), 1);

        if (!retail::sub_90F610((i32)region_pack_manager::inst()))
            return;

        if (retail::sub_90F5E0((i32)region_pack_manager::inst()))
            return;

        retail::sub_9585E0((i32)spawn_table_manager::inst());
        zombie_manager::inst()->suspended = 0;
    }

    if (references::unk_00be7455.read() && references::summon_state.read())
        retail::sub_983D00((i32)references::summon_state.read());

    if (act_transition_state)
        return;

    i32 mission_act = get_mission_act(current_mission);

    if (mission_act != -1 && mission_act != act) {
        if (act_transition_state)
            return;

        act_transition_state = 1;
        act_force_reload     = 0;
        requested_act        = mission_act;

        begin_act_transition();

        return;
    }

    u32* unk_object = (u32*)amalga::resource_manager::references::unk_010f7760.read();

    {
        mash::string name(current_mission.name.c_str());
        retail::sub_83F640((const char**)unk_object, (i32)&name, 3);
    }

    {
        mash::string first (current_mission.name.c_str());
        mash::string second(current_mission.name.c_str());

        retail::sub_87D610((i32)unk_object, (i32*)&second, (i32)&first, 3);
    }

    if (flags & mission_manager_flag_mission_ready)
        event_manager::raise_event(references::unk_0102cc20.read(), arch_base_vhandle());

    retail::sub_7F09B0((u32*)quest_manager::inst(), (i32)&current_mission);

    i32 presence = soap::online::inst()->unk_02c;

    if (flags & mission_manager_flag_mission_ready) {
        i32 context = retail::sub_7342C0(1, current_mission.mission_index);

        if (context == -1) {
            retail::sub_9EB5D0((u32*)soap::online::inst(), 1, 3); // online::set_context

            presence = 0;
        } else {
            retail::sub_9EB5D0((u32*)soap::online::inst(), 2, current_act);
            retail::sub_9EB5D0((u32*)soap::online::inst(), 5, context);

            presence = 2;
        }
    }

    soap::online::inst()->method_024(presence);

    if (references::unk_0102fed8.read())
        flags |= mission_manager_flag_unk_00020000;
    else
        flags &= ~mission_manager_flag_unk_00020000;

    if (references::unk_00e67164.read())
        flags |= mission_manager_flag_unk_00010000;
    else
        flags &= ~mission_manager_flag_unk_00010000;

    unk_214              = (f32)((f64)references::unk_00fc7a38.read() * 8.0);
    saved_game_time_rate = game_time_rate;

    if (flags & mission_manager_flag_mission_ready) {
        if (((mission_header_instance_base*)next_mission.mission_header_instance->data.buffer)->flags & mission_header_flag_unk_04000000)
            flags |= mission_manager_flag_unk_00400000;
        else
            flags &= ~mission_manager_flag_unk_00400000;

        clear_selected_poi(true);
    }

    state = mission_manager_state_loading;
}

// sub_984C90
void mission_manager::process_state_loading() {
    update_mission_icons();

    if (retail::sub_824870((u32*)amalga::resource_manager::references::unk_010f7760.read(), 3))
        return;

    entity* hero = references::g_world_ptr.read()->hero_ptr;

    if (hero && ((actor*)hero)->get_ai_core()) {
        auto* node = (u32*)((actor*)hero)->get_ai_core()->get_info_node(info_node_type_std_carry);

        if (node && retail::sub_425020(node))
            retail::sub_4BA230(node, 0, 1);
    }

    if (references::region_spawns_enabled.read() && (u32*)region_pack_manager::inst()) {
        retail::sub_951C40((i32)region_pack_manager::inst(), 1);

        if (retail::sub_90F5E0((i32)region_pack_manager::inst()))
            return;

        retail::sub_9585E0((i32)spawn_table_manager::inst());
        zombie_manager::inst()->suspended = 0;
    }

    if (references::unk_00be7455.read() && references::summon_state.read())
        retail::sub_983D00((i32)references::summon_state.read());

    {
        auto* slot = (amalga::resource_pack_slot*)
                     ((i32 (__cdecl*)(mash::string))retail::sub_90AAF0)
                     (current_mission.name);

        amalga::push_resource_context_stack_object context(slot);

        string_hash exec_name;
        exec_name.initialize(mash::ALLOCATED, current_mission.name.c_str());

        chuck::vm::script_manager &scripts = *chuck::vm::script_manager::inst();

        scripts.load(exec_name, 0, slot, string_hash());
        current_mission.exec = scripts.find_executable(exec_name, string_hash());

        while (current_mission.exec->suspend_count > 0)
            --current_mission.exec->suspend_count;
    }

    if (flags & mission_manager_flag_mission_ready) {
        flags &= ~mission_manager_flag_mission_ready;
        state  = mission_manager_state_running_mission;
    }

    save_player_restart(hero->my_abs_po->get_position(), hero->my_abs_po->matrix.z_row());
}

// sub_97EF50
void mission_manager::process_state_running_mission() {
    if (flags & mission_manager_flag_unk_00002000) {
        flags &= ~mission_manager_flag_unk_00002000;
        state  = mission_manager_state_start_mission_failed_dialog;

        return;
    }

    if (flags & mission_manager_flag_unk_00800000) {
        flags &= ~mission_manager_flag_unk_00800000;
        state  = mission_manager_state_start_mission_succeeded_dialog;

        return;
    }

    if (!(flags & mission_manager_flag_need_to_unload))
        return;

    state = mission_manager_state_start_unloading;

    if (!(flags & mission_manager_flag_mission_ready) && (flags & mission_manager_flag_unk_00400000))
        state = mission_manager_state_unk_5;

    flags &= ~mission_manager_flag_need_to_unload;

    event_manager::raise_event(references::unk_0102c498.read(), arch_base_vhandle());

    unk_0fc[0] = real_time;
    unk_0fc[3] = unk_0fc[1] + std::rand() % (i32)(unk_0fc[2] - unk_0fc[1]);
}

// sub_980C10
void mission_manager::process_state_unk_5() {
    game_time_rate = saved_game_time_rate;

    retail::sub_730F10((u32*)references::glass_house_manager.read(), 1); // set_glass_house_level

    if ((flags & mission_manager_flag_need_to_malor) || (flags & mission_manager_flag_mission_ready)) {
        state = mission_manager_state_start_unloading;

        return;
    }

    retail::sub_808FA0((i32*)references::cutscene_player.read(), 0);

    unk_240 = string_hash();

    play_open_city_music();

    entity* hero = references::g_world_ptr.read()->hero_ptr;

    leave_radius_squared = 40000.0f;

    if (!hero) {
        hero_reference_position = references::unk_011117a0.read();
        hero_region             = nullptr;
        state                   = mission_manager_state_start_unloading;

        return;
    }

    hero_reference_position = hero->my_abs_po->get_position();
    hero_region             = hero->get_primary_region();

    if ((f64)hero_reference_position.y > 25.0)
        leave_radius_squared = 122500.0f;

    flags |= mission_manager_flag_unk_00004000 | mission_manager_flag_unk_00008000;
    state  = mission_manager_state_unk_6;
}

// sub_984EE0
void mission_manager::process_state_unk_6() {
    retail::sub_8FE010(0.0f);

    if ((flags & mission_manager_flag_mission_ready) || (flags & mission_manager_flag_need_to_malor) ||
        (auto_launch_instances && auto_launch_instances->size())) {

        state = mission_manager_state_start_unloading;

        return;
    }

    entity* hero = references::g_world_ptr.read()->hero_ptr;

    if (!hero)
        state = mission_manager_state_start_unloading;
    else {
        const vector3 &position = hero->my_abs_po->get_position();

        f32 dx = (f32)((f64)position.x - (f64)hero_reference_position.x);
        f32 dy = (f32)((f64)position.y - (f64)hero_reference_position.y);
        f32 dz = (f32)((f64)position.z - (f64)hero_reference_position.z);

        f32 distance_squared = (f32)((f64)dx * (f64)dx + (f64)dy * (f64)dy + (f64)dz * (f64)dz);

        if (distance_squared > leave_radius_squared)
            state = mission_manager_state_start_unloading;
    }

    update_mission_icons();
}

// sub_985000
void mission_manager::process_state_start_unloading() {
    retail::sub_6179B0();
    retail::sub_808FA0((i32*)references::cutscene_player.read(), 0);

    if (current_mission.exec) {
        while (current_mission.exec->suspend_count > 0)
            --current_mission.exec->suspend_count;

        current_mission.exec = nullptr;
    }

    if (!(flags & mission_manager_flag_mission_ready)) {
        unk_240 = string_hash();

        play_open_city_music();
    }

    {
        auto* slot = (amalga::resource_pack_slot*)
                     ((i32 (__cdecl*)(mash::string))retail::sub_90AAF0)
                     (current_mission.name);

        amalga::push_resource_context_stack_object context(slot);

        string_hash exec_name;
        exec_name.initialize(mash::ALLOCATED, current_mission.name.c_str());

        chuck::vm::script_manager &scripts = *chuck::vm::script_manager::inst();

        if (scripts.is_loaded(exec_name, string_hash()))
            scripts.un_load(exec_name, true, string_hash());
    }

    update_eligible_missions();

    app::inst()->get_game()->unk_078 = 0;

    if (references::frontend.get().igo && references::frontend.get().igo->face_button_system)
        retail::sub_6DE2E0((u32*)references::frontend.get().igo->face_button_system);

    retail::sub_808070((i32)toa_cutscene::get());
    retail::sub_730F10((u32*)references::glass_house_manager.read(), 1); // set_glass_house_level

    retail::sub_73E3C0((flags & mission_manager_flag_unk_00020000) != 0);
    flags &= ~mission_manager_flag_unk_00020000;

    if (!(flags & mission_manager_flag_unk_00010000))
        retail::sub_598C50(0);

    retail::sub_59C2E0((flags & mission_manager_flag_unk_00010000) != 0);
    flags &= ~mission_manager_flag_unk_00010000;

    retail::sub_588CC0(unk_214);
    retail::sub_583EA0(0);

    game_time_rate = saved_game_time_rate;

    retail::sub_7A4CE0(references::g_world_ptr.read()->the_terrain);

    for (; saved_script_hero_frozen_count > 0; --saved_script_hero_frozen_count)
        references::game.read()->freeze_hero(false);

    entity* hero = references::g_world_ptr.read()->hero_ptr;

    for (; saved_script_hero_suspend_count > 0; --saved_script_hero_suspend_count)
        ((void (__thiscall*)(entity*, i32))hero->vtable[0x1D4 / 4])(hero, 1);

    if (((actor*)hero)->get_ai_core()) {
        for (; saved_script_hero_ai_disable_count > 0; --saved_script_hero_ai_disable_count)
            retail::sub_4DF4B0((i32*)((actor*)hero)->get_ai_core(), 1);
    }

    for (; saved_script_hero_invulnerable_count > 0; --saved_script_hero_invulnerable_count)
        *(u32*)((u8*)hero + 0x18) &= ~0x4000;

    saved_script_hero_frozen_count       = 0;
    saved_script_hero_ai_disable_count   = 0;
    saved_script_hero_suspend_count      = 0;
    saved_script_hero_invulnerable_count = 0;

    if (!(flags & mission_manager_flag_mission_ready)) {
        flags &= ~mission_manager_flag_unk_00040000;

        if (hero) {
            hero_reference_position = hero->my_abs_po->get_position();
            hero_region             = hero->get_primary_region();

            flags |= mission_manager_flag_unk_00004000 | mission_manager_flag_unk_00008000;
        } else {
            hero_reference_position = references::unk_011117a0.read();
            hero_region             = nullptr;
        }
    }

    flags &= ~mission_manager_flag_unk_00400000;
    state  = mission_manager_state_unloading;
}

// sub_9853B0
void mission_manager::process_state_unloading() {
    update_mission_icons();

    flags |= mission_manager_flag_unk_02000000;

    retail::sub_93D770();

    if (references::region_spawns_enabled.read() && region_spawn_manager::inst() &&
        (u32*)region_pack_manager::inst() && spawn_table_manager::inst()) {

        region_spawn_manager::inst()->unk_008 = 0;

        retail::sub_9585E0((i32)spawn_table_manager::inst());
        zombie_manager::inst()->suspended = 0;
    }

    if (references::unk_00be7455.read() && references::summon_state.read()) {
        retail::sub_90FC00(references::summon_state.read());

        if (retail::sub_920420((u32*)region_pack_manager::inst()))
            return;

        retail::sub_983D00((i32)references::summon_state.read());
    }

    u32* unk_object = (u32*)amalga::resource_manager::references::unk_010f7760.read();

    if (retail::sub_8248E0(unk_object))
        return;

    if (retail::sub_68BF20((i32)references::frontend.get().igo->conversation_menu_system))
        retail::sub_6F9250((i32)references::frontend.get().igo->conversation_menu_system);

    if (retail::sub_879F10(unk_object, 3))
        return;

    if (references::region_spawns_enabled.read() && region_spawn_manager::inst() &&
        (u32*)region_pack_manager::inst() && spawn_table_manager::inst()) {

        retail::sub_951C40((i32)region_pack_manager::inst(), 0);

        if (flags & mission_manager_flag_mission_ready)
            retail::sub_951C40((i32)region_pack_manager::inst(), 1);

        retail::sub_92E640((u32*)region_spawn_manager::inst());
        retail::sub_9547F0((i32)spawn_table_manager::inst()->unk_04c);
    }

    if (references::unk_00be7455.read() && references::summon_state.read())
        retail::sub_951330((u32*)references::summon_state.read());

    {
        mash::string name(current_mission.name.c_str());
        retail::sub_824810(unk_object, (i32)&name, 3);
    }

    if (!(flags & mission_manager_flag_mission_ready) && !(flags & mission_manager_flag_need_to_malor)) {
        bool end_fade = true;

        const chuck::vm::script_instance* header = current_mission.mission_header_instance;

        if (header && !current_mission.gen_dis_exec_name.size()) {
            string_hash parent_name;

            if (*(string_hash*)retail::sub_5FD120((u32*)header->parent, (u32*)&parent_name) == references::mission_header_instance_base_name.read() &&
                (((mission_header_instance_base*)header->data.buffer)->flags & mission_header_flag_unk_01000000))

                end_fade = false;
        }

        game_data* data = references::game.read()->data;

        if ((!data || !retail::sub_77C9F0((u32*)data)) && end_fade)
            retail::sub_6A9D60((u8*)references::frontend.get().igo->loading_screen, 0.0f, 1);
    }

    state = mission_manager_state_idle;
}

// sub_97F060
void mission_manager::process_state_maloring_player_wait_for_blackscreen(f32) {
    retail::sub_789BA0((u32*)references::g_world_ptr.read()->the_terrain);

    malor_region = (region*)retail::sub_7A7400((i32*)references::g_world_ptr.read()->the_terrain,
                                               (i32)&player_restart_position,
                                               1);

    retail::sub_967860((u32*)references::g_world_ptr.read(), (i32)&player_restart_position, 0, 1);

    const vector3 &up     = math::references::unk_00f4d3ac.get();
    const vector3 &facing = player_restart_xz_facing;

    vector3 side;
    side.x = (f32)((f64)facing.z * (f64)up.y - (f64)facing.y * (f64)up.z);
    side.y = (f32)((f64)up.z * (f64)facing.x - (f64)facing.z * (f64)up.x);
    side.z = (f32)((f64)facing.y * (f64)up.x - (f64)up.y * (f64)facing.x);

    entity* hero = references::g_world_ptr.read()->hero_ptr;

    matrix4x4 transform;
    retail::sub_5FC820((u64*)&transform, (i32)&side, (i32)&up, (u32*)&facing, (u32*)&hero->my_abs_po->get_position());
    retail::sub_627180((i32)hero, (f32*)&transform);

    vector3 position = references::g_world_ptr.read()->hero_ptr->my_abs_po->get_position();
    position.y = (f32)((f64)position.y + 2.0);

    camera* view = (camera*)retail::sub_95C430((u32*)references::g_world_ptr.read());

    if (view && ((bool (__thiscall*)(camera*))view->vtable[0x7C / 4])(view)) {
        retail::sub_9C4880((u32*)view, 11);

        const vector3 &z_axis = references::g_world_ptr.read()->hero_ptr->my_abs_po->matrix.z_row();

        vector3 behind;
        behind.x = (f32)((f64)position.x - (f64)(f32)((f64)z_axis.x * 2.0));
        behind.y = (f32)((f64)position.y - (f64)(f32)((f64)z_axis.y * 2.0));
        behind.z = (f32)((f64)position.z - (f64)(f32)((f64)z_axis.z * 2.0));

        ((void (__thiscall*)(camera*, vector3))retail::sub_9C4980)(view, behind);
        retail::sub_9C4970((u32*)view, 1);
    }

    malor_wait_frames = 0;
    state             = mission_manager_state_maloring_player_wait_for_district;
}

// sub_980D40
void mission_manager::process_state_maloring_player_wait_for_district() {
    entity* hero = references::g_world_ptr.read()->hero_ptr;

    if (!(hero->unk_014 & 1) && hero->get_primary_region() && retail::sub_77BF80())
        ++malor_wait_frames;
    else
        malor_wait_frames = 0;

    if ((i32)malor_wait_frames < 30)
        return;

    malor_wait_frames = 0;

    retail::sub_967860((u32*)references::g_world_ptr.read(), (i32)&player_restart_position, 0, 1);

    vector3 position = references::g_world_ptr.read()->hero_ptr->my_abs_po->get_position();
    position.y = (f32)((f64)position.y + 2.0);

    const vector3 &up     = math::references::unk_00f4d3ac.get();
    const vector3 &facing = player_restart_xz_facing;

    vector3 side;
    side.x = (f32)((f64)facing.z * (f64)up.y - (f64)facing.y * (f64)up.z);
    side.y = (f32)((f64)up.z * (f64)facing.x - (f64)facing.z * (f64)up.x);
    side.z = (f32)((f64)facing.y * (f64)up.x - (f64)up.y * (f64)facing.x);

    hero = references::g_world_ptr.read()->hero_ptr;

    matrix4x4 transform;
    retail::sub_5FC820((u64*)&transform, (i32)&side, (i32)&up, (u32*)&facing, (u32*)&position);
    retail::sub_627180((i32)hero, (f32*)&transform);

    camera* view = (camera*)retail::sub_95C430((u32*)references::g_world_ptr.read());

    if (view && ((bool (__thiscall*)(camera*))view->vtable[0x7C / 4])(view)) {
        retail::sub_9C4880((u32*)view, 11);

        const vector3 &z_axis = references::g_world_ptr.read()->hero_ptr->my_abs_po->matrix.z_row();

        vector3 behind;
        behind.x = (f32)((f64)position.x - (f64)(f32)((f64)z_axis.x * 2.0));
        behind.y = (f32)((f64)position.y - (f64)(f32)((f64)z_axis.y * 2.0));
        behind.z = (f32)((f64)position.z - (f64)(f32)((f64)z_axis.z * 2.0));

        ((void (__thiscall*)(camera*, vector3))retail::sub_9C4980)(view, behind);
        retail::sub_9C4970((u32*)view, 1);
        retail::sub_9C6500((i32*)view);
    }

    malor_region = nullptr;
    state        = mission_manager_state_idle;

    if (region_spawn_manager::inst() && spawn_table_manager::inst()) {
        retail::sub_92E640((u32*)region_spawn_manager::inst());
        retail::sub_9547F0((i32)spawn_table_manager::inst()->unk_04c);
    }
}

// sub_980FE0
void mission_manager::process_state_start_mission_failed_dialog() {
    retail::sub_97F930((u32*)this);
    retail::sub_6A9D60((u8*)references::frontend.get().igo->loading_screen, 0.0f, 0);

    entity*            hero   = references::g_world_ptr.read()->hero_ptr;
    generic_interface* damage = hero->my_ifc_storage->get_ifc(entity_ifc_damage);

    u32 unk_value = *(u32*)((u8*)damage + 0xB4);

    auto* dialog = (igo_3d_pauseless_dialog_widget*)references::frontend.get().igo->pauseless_dialog;

    dialog->method_0e8();

    if (!unk_value) {
        dialog->method_0d8(3234);
        dialog->method_0e4()(dialog, mash::string(""));
    } else {
        dialog->method_0d8(311);

        // constructs the name into the given slot; a by-value return cast would swap the slot and this
        alignas(mash::string) u8 name[sizeof(mash::string)];
        ((mash::string* (__thiscall*)(void*, void*))retail::sub_687300)(retail::sub_7685B0(), name);

        dialog->method_0e4()(dialog, *(mash::string*)name);

        ((mash::string*)name)->~string();
    }

    dialog->unk_22c = 0;

    if (is_quitable) {
        retail::sub_6A2260((u32*)dialog, 347);
        retail::sub_68E920((i32)dialog, 1);
        retail::sub_6A2280((u32*)dialog, 344);
        retail::sub_68E950((i32)dialog, 1);
    } else {
        retail::sub_6A2280((u32*)dialog, 347);
        retail::sub_68E950((i32)dialog, 1);
        retail::sub_68E920((i32)dialog, 0);
    }

    retail::sub_6DE490((u8*)dialog, 1);

    state = mission_manager_state_running_mission_failed_dialog;
}

// sub_9864B0
void mission_manager::process_state_running_mission_failed_dialog() {
    if (references::frontend.get().igo->pauseless_dialog->is_visible())
        return;

    i32 selection = ((igo_3d_pauseless_dialog_widget*)references::frontend.get().igo->pauseless_dialog)->method_120();

    if (is_quitable && selection != 1)
        retail::sub_981F20((i32)this);
    else
        retail::sub_985EE0(this);

    flags &= ~mission_manager_flag_unk_00001000;
    state  = mission_manager_state_running_mission;
}

// sub_97F2B0
void mission_manager::process_state_running_mission_succeeded_dialog() {
    if (!mission_finished_screen_has_appeared)
        retail::sub_7FEF20((u32*)quest_manager::inst(), (i32)&current_mission);

    flags &= ~mission_manager_flag_unk_00001000;
    state  = mission_manager_state_running_mission;
}

// sub_981110
void mission_manager::process_state_blackscreen_on() {
    if (flags & mission_manager_flag_unk_08000000)
        flags &= ~mission_manager_flag_unk_08000000;
    else if (!references::frontend.get().igo->loading_screen->is_visible())
        retail::sub_707E90((u8*)references::frontend.get().igo->loading_screen, 1.5f, 0, 0, 0, 0, 45.0f);

    state_timer = 0.0f;

    clear_selected_poi(true);

    state = mission_manager_state_wait_for_blackscreen_on;
}

// sub_97F2F0
void mission_manager::process_state_wait_for_blackscreen_on(f32 time_inc) {
    state_timer = (f32)((f64)time_inc + (f64)state_timer);

    if (state_timer > 1.5f) {
        state_timer = 0.0f;
        state       = mission_manager_state_start_loading;
    }
}
