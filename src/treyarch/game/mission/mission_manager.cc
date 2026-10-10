#include <new>

#include "retail.hh"
#include "treyarch/app/app.hh"
#include "treyarch/chuck/vm/script_manager.hh"
#include "treyarch/game/frontend/frontend_manager.hh"
#include "treyarch/game/frontend/ui_frontend.hh"
#include "treyarch/game/mission/mission_manager.hh"
#include "treyarch/game/script/script_access.hh"
#include "treyarch/shared/memory/heap.hh"
#include "util/memory_reference.hh"

using namespace treyarch;

// sub_97FFA0
mission_info::mission_info() {}

// sub_97E2C0
mission_info &mission_info::operator=(const mission_info &other) {
    name                    = other.name;
    mission_index           = other.mission_index;
    icon                    = other.icon;
    ped                     = other.ped;
    key_position            = other.key_position;
    mission_header_instance = other.mission_header_instance;
    gen_dis_exec_name       = other.gen_dis_exec_name;
    exec                    = other.exec;
    retry_count             = other.retry_count;
    unk_03c                 = other.unk_03c;

    return *this;
}

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
    selected_poi.poi_index              = -1;
    selected_poi.instance               = nullptr;
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
    mission_icons->reserve(20);

    allocation = memory::heap::allocate(sizeof(*mission_triggers));
    mission_triggers = allocation ? new (allocation) dinkumware::vector<mission_icon_info>() : nullptr;
    mission_triggers->reserve(20);

    allocation = memory::heap::allocate(sizeof(*mission_ped_triggers));
    mission_ped_triggers = allocation ? new (allocation) dinkumware::vector<mission_icon_info>() : nullptr;
    mission_ped_triggers->reserve(20);

    allocation = memory::heap::allocate(sizeof(*eligible_mission_instances));
    eligible_mission_instances = allocation ? new (allocation) dinkumware::vector<const chuck::vm::script_instance*>() : nullptr;
    eligible_mission_instances->reserve(40);

    allocation = memory::heap::allocate(sizeof(*auto_launch_instances));
    auto_launch_instances = allocation ? new (allocation) dinkumware::list<const chuck::vm::script_instance*>() : nullptr;

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

    mission_manager::instance() = allocation ? new (allocation) mission_manager() : nullptr;
}

// sub_97E1E0
bool mission_manager::is_idle() const {
    return state == mission_manager_state_idle;
}

// sub_97E1F0
bool mission_manager::is_mission_running() const {
    return state == mission_manager_state_running_mission;
}

// sub_97E200
bool mission_manager::is_mission_ready() const {
    return flags & mission_manager_flag_mission_ready;
}

// sub_97E220
bool mission_manager::needs_to_malor() const {
    return flags & mission_manager_flag_need_to_malor;
}

// sub_97E230
bool mission_manager::is_maloring() const {
    return state == mission_manager_state_maloring_player_wait_for_blackscreen ||
           state == mission_manager_state_maloring_player_wait_for_district;
}

// sub_97E260
e_mission_manager_state mission_manager::get_state() const {
    return state;
}

// sub_97E270
chuck::vm::script_executable* mission_manager::get_running_mission_exec() const {
    return state == mission_manager_state_running_mission ? current_mission.exec : nullptr;
}

// sub_97E320
bool mission_manager::get_mission_caption_index(u32, i32* index) {
    *index = -1;

    return false;
}

// sub_97E330
vector3 mission_manager::get_mission_key_position() const {
    if (state == mission_manager_state_running_mission)
        return current_mission.key_position;

    return vector3(0.0f, 0.0f, 0.0f);
}

// sub_97E370
i32 mission_manager::get_running_script_index() const {
    return state == mission_manager_state_running_mission ? current_mission.mission_index : 0;
}

// sub_97E390
const char* mission_manager::get_running_mission_name() const {
    return state == mission_manager_state_running_mission ? current_mission.name.c_str() : nullptr;
}

// sub_97E3B0
i32 mission_manager::get_mission_script_retry_count() const {
    return state == mission_manager_state_running_mission ? current_mission.retry_count : 0;
}

// sub_97E3E0
i32 mission_manager::get_current_act() const {
    return current_act;
}

// sub_97E3F0
i32 mission_manager::get_current_act_internal() const {
    return act;
}

// sub_97E400
i32 mission_manager::get_requested_act() const {
    return requested_act;
}

// sub_97E410
bool mission_manager::is_act_transitioning() const {
    return act_transition_state != 0;
}

// sub_9803F0
bool mission_manager::has_auto_launch_instances() const {
    return auto_launch_instances && auto_launch_instances->size();
}

// sub_980540
i32 mission_manager::get_total_times_script_has_been_successfully_completed() const {
    if (state == mission_manager_state_running_mission)
        return (i32)mission_vars[current_mission.mission_index];

    return -1;
}

// sub_97FA30
void mission_manager::play_open_city_music() {
    f32* music_in_mission = (f32*)chuck::vm::script_manager::inst()->get_shared_var_addr("g_music_in_mission");

    if (!(*music_in_mission > 0.0f))
        return;

    string_hash function_name;
    function_name.initialize(mash::ALLOCATED, "music_enable_city_music()");

    script::start_thread(script::create_thread(function_name, references::master_global_instance.read(), 0), true);
}

// sub_97E290
void mission_manager::unpause_and_notify_frontend() {
    game* the_game = references::game.read();

    if (the_game->game_paused)
        retail::sub_97BB70((i32)the_game); // game::unpause_action

    void* widget = references::frontend.get().igo->unknown_widget_174;

    ((void (__thiscall*)(void*))(*(void***)widget)[0xD8 / 4])(widget); // slot 54
}
