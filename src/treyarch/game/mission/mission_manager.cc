#include <new>

#include "retail.hh"
#include "treyarch/game/mission/mission_manager.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;

// sub_97FFA0
mission_info::mission_info() {}

// sub_97DFF0
mission_district_info::mission_district_info() {}

// sub_982F80
mission_manager::mission_manager() {
    gen_global_exec = nullptr;

    startup_timer            = 1.0f;
    real_time_timer          = 0.0f;
    game_time_timer          = 0.0f;
    game_time_rate           = 10;
    saved_game_time_rate     = 10;
    unk_10c                  = 40.0f;
    district_count           = 0;
    real_time                = 0;
    real_time_var            = nullptr;
    game_time                = 0;
    game_time_var            = nullptr;
    game_days_var            = nullptr;
    game_day_of_the_week_var = nullptr;

    last_mission_time                = 0;
    last_mission_interval            = 300;
    last_successful_mission_time     = 0;
    last_successful_mission_interval = 300;
    last_zoom_map_selection_time     = 0;
    last_zoom_map_selection_interval = 300;
    unk_0fc[0]                       = 0;
    unk_0fc[1]                       = 20;
    unk_0fc[2]                       = 40;
    unk_0fc[3]                       = 20;
    unk_110                          = 200.0f;
    state                            = mission_manager_state_initial_startup;

    flags                       = 0;
    mission_icons               = nullptr;
    mission_triggers            = nullptr;
    mission_ped_triggers        = nullptr;
    mission_vars                = nullptr;
    upgrade_vars                = nullptr;
    unk_1b8                     = 0;
    unk_1bc                     = 0;
    saved_checkpoint_registry   = nullptr;
    working_checkpoint_registry = nullptr;
    state_timer                 = 0.0f;
    idle_event_cooldown         = 0.0f;
    hero_region                 = nullptr;
    malor_region                = nullptr;
    malor_wait_frames           = 0;
    unk_200                     = 0;
    eligible_mission_instances  = nullptr;
    auto_launch_instances       = nullptr;
    unk_20c                     = 0;
    unk_210                     = 5;
    unk_214                     = 0.0f;

    zoom_map_poi_selected_callback_id   = references::unk_00bb6d68.read();
    zoom_map_poi_unselected_callback_id = references::unk_00bb6d68.read();
    selected_poi_icon                   = nullptr;
    selected_poi.icon                   = (u32)-1;
    selected_poi.poi_index              = 0;
    unk_238                             = nullptr;
    unk_23c                             = 0;
    unk_244                             = 0.0f;

    saved_script_hero_frozen_count       = 0;
    saved_script_hero_ai_disable_count   = 0;
    saved_script_hero_suspend_count      = 0;
    saved_script_hero_invulnerable_count = 0;

    act                  = references::global_act.read();
    current_act          = references::global_act.read();
    unk_264              = references::global_act.read();
    act_transition_state = 0;
    act_force_reload     = 0;

    sky_swap_state                       = 0;
    unk_295                              = 0;
    leave_radius_squared                 = 0.0f;
    poi_locations                        = nullptr;
    checkpoint                           = 0;
    is_quitable                          = 0;
    success_screen_type                  = 0;
    success_screen_parameter             = 0;
    mission_finished_screen_has_appeared = 0;
    first_act_request                    = 1;

    script_globals_initialized = 0;

    void* allocation = memory::heap::allocate(0x0C);
    saved_checkpoint_registry = allocation ? retail::sub_804D60((u32*)allocation) : nullptr;

    allocation = memory::heap::allocate(0x0C);
    working_checkpoint_registry = allocation ? retail::sub_804D60((u32*)allocation) : nullptr;

    allocation = memory::heap::allocate(sizeof(*mission_icons));
    mission_icons = allocation ? new (allocation) dinkumware::vector<mission_icon_info>() : nullptr;
    retail::sub_982750((u32*)mission_icons, 0x14);

    allocation = memory::heap::allocate(sizeof(*mission_triggers));
    mission_triggers = allocation ? new (allocation) dinkumware::vector<mission_icon_info>() : nullptr;
    retail::sub_982750((u32*)mission_triggers, 0x14);

    allocation = memory::heap::allocate(sizeof(*mission_ped_triggers));
    mission_ped_triggers = allocation ? new (allocation) dinkumware::vector<mission_icon_info>() : nullptr;
    retail::sub_982750((u32*)mission_ped_triggers, 0x14);

    allocation = memory::heap::allocate(sizeof(*eligible_mission_instances));
    eligible_mission_instances = allocation ? new (allocation) dinkumware::vector<void*>() : nullptr;
    retail::sub_982DD0((u32*)eligible_mission_instances, 0x28);

    allocation = memory::heap::allocate(sizeof(*auto_launch_instances));
    auto_launch_instances = allocation ? new (allocation) dinkumware::list<void*>() : nullptr;

    unk_0d8[0] = 0;
    unk_0d8[1] = 0;
    unk_0d8[2] = 0;

    hero_reference_position = references::unk_011117a0.read();

    allocation = memory::heap::allocate(0x40);
    unk_238 = allocation ? retail::sub_804870((u32*)allocation) : nullptr;

    allocation = memory::heap::allocate(sizeof(*poi_locations));
    poi_locations = allocation ? new (allocation) dinkumware::vector<mission_poi_location_t>() : nullptr;
    selected_poi_location_key = -1;
}

// sub_428AA0
void mission_manager::create_inst() {
    void* allocation = memory::heap::allocate(sizeof(mission_manager));

    references::mission_manager.write(allocation ? new (allocation) mission_manager() : nullptr);
}

// sub_97E1E0
bool mission_manager::is_idle() const {
    return state == mission_manager_state_idle;
}

// sub_97E1F0
bool mission_manager::is_mission_running() const {
    return state == mission_manager_state_running_mission;
}
