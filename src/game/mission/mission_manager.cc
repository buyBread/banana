#include <new>

#include "retail.hh"
#include "game/app.hh"
#include "treyarch/chuck/vm/script_instance.hh"
#include "treyarch/chuck/vm/script_manager.hh"
#include "treyarch/chuck/vm/script_executable.hh"
#include "game/event/event_manager.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/game.hh"
#include "game/mission/mission_manager.hh"
#include "game/quest_manager.hh"
#include "game/script/script_access.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/entity/interface/damage_interface.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mm/mm.hh"
#include "treyarch/soap/online.hh"
#include "banana/util/memory_reference.hh"

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
    icon_entity_pool                    = nullptr;
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

    void* allocation = mm::allocate(sizeof(*saved_checkpoint_registry));
    saved_checkpoint_registry = allocation ? new (allocation) script_registry() : nullptr;

    allocation = mm::allocate(sizeof(*working_checkpoint_registry));
    working_checkpoint_registry = allocation ? new (allocation) script_registry() : nullptr;

    allocation = mm::allocate(sizeof(*mission_icons));
    mission_icons = allocation ? new (allocation) vc8::vector<mission_icon_info>() : nullptr;
    mission_icons->reserve(20);

    allocation = mm::allocate(sizeof(*mission_triggers));
    mission_triggers = allocation ? new (allocation) vc8::vector<mission_icon_info>() : nullptr;
    mission_triggers->reserve(20);

    allocation = mm::allocate(sizeof(*mission_ped_triggers));
    mission_ped_triggers = allocation ? new (allocation) vc8::vector<mission_icon_info>() : nullptr;
    mission_ped_triggers->reserve(20);

    allocation = mm::allocate(sizeof(*eligible_mission_instances));
    eligible_mission_instances = allocation ? new (allocation) vc8::vector<const chuck::vm::script_instance*>() : nullptr;
    eligible_mission_instances->reserve(40);

    allocation = mm::allocate(sizeof(*auto_launch_instances));
    auto_launch_instances = allocation ? new (allocation) vc8::list<const chuck::vm::script_instance*>() : nullptr;

    unk_0d8[0] = 0;
    unk_0d8[1] = 0;
    unk_0d8[2] = 0;

    hero_reference_position = references::unk_011117a0.read();

    allocation = mm::allocate(0x40);
    icon_entity_pool = allocation ? retail::sub_804870((u32*)allocation) : nullptr;

    allocation = mm::allocate(sizeof(*poi_locations));
    poi_locations = allocation ? new (allocation) vc8::vector<mission_poi_location_t>() : nullptr;
    selected_poi_location_key = -1;
}

// sub_428AA0
void mission_manager::create_inst() {
    void* allocation = mm::allocate(sizeof(mission_manager));

    mission_manager::instance() = allocation ? new (allocation) mission_manager() : nullptr;
}

// sub_97E060
void mission_manager::setup_game_var_refs() {
    if (!real_time_var) {
        real_time_var  = (f32*)chuck::vm::script_manager::inst()->get_game_var_addr("gv_real_world_timer");
        *real_time_var = 0.0f;
    }

    if (!game_time_var)
        game_time_var = (f32*)chuck::vm::script_manager::inst()->get_game_var_addr("gv_game_clock_timer");

    if (!game_days_var)
        game_days_var = (f32*)chuck::vm::script_manager::inst()->get_game_var_addr("gv_game_days");

    if (!game_day_of_the_week_var)
        game_day_of_the_week_var = (f32*)chuck::vm::script_manager::inst()->get_game_var_addr("gv_game_day_of_the_week");

    if (!mission_vars)
        mission_vars = (f32*)chuck::vm::script_manager::inst()->get_game_var_addr("gv_missions");

    if (!upgrade_vars)
        upgrade_vars = (f32*)chuck::vm::script_manager::inst()->get_game_var_addr("gv_upgrades");
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
bool mission_manager::get_mission_caption_index(arch_base_vhandle, i32* index) {
    *index = -1;

    return false;
}

// sub_71FF20
i32 mission_manager::get_mission_index(arch_base_vhandle) const {
    return -1;
}

// sub_71FF10
bool mission_manager::is_mission_eligible(arch_base_vhandle) const {
    return false;
}

// sub_983E10
void mission_manager::set_mission_meta_text(u32, arch_base_vhandle, i32) {}

// sub_97F030
void mission_manager::hide_blackscreen_after_unload(bool hide) {
    auto* header = (mission_header_instance_base*)current_mission.mission_header_instance->data.buffer;

    if (hide)
        header->flags &= ~mission_header_flag_keep_blackscreen_after_unload;
    else
        header->flags |= mission_header_flag_keep_blackscreen_after_unload;
}

// sub_97E330
vector3d mission_manager::get_mission_key_position() const {
    if (state == mission_manager_state_running_mission)
        return current_mission.key_position;

    return vector3d(0.0f, 0.0f, 0.0f);
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

// sub_982220
mission_icon_info* mission_manager::find_mission_icon(const chuck::vm::script_instance* instance) const {
    for (mission_icon_info* icon = mission_icons->begin(); icon != mission_icons->end(); ++icon) {
        if (icon->instance == instance)
            return icon;
    }

    return nullptr;
}

// sub_9855F0
void mission_manager::terminate_script(bool was_completed_successfully, bool suppress_dialogs) {
    if (state != mission_manager_state_running_mission)
        return;

    if (flags & mission_manager_flag_need_to_unload || flags & mission_manager_flag_terminating)
        return;

    i32 mission_index = current_mission.mission_index;

    flags |= mission_manager_flag_terminating;

    event_manager::raise_event(references::terminate_script_called.read(), arch_base_vhandle());

    ui_frontend* igo = references::frontend.get().igo;

    if (igo) {
        if (igo->objective_text)
            retail::sub_6E7A00((u32*)igo->objective_text);

        if (igo->hint_text)
            retail::sub_6E7A00((u32*)igo->hint_text);

        if (igo->enemy_health_manager)
            retail::sub_68D020((u8*)igo->enemy_health_manager, 0);

        retail::sub_6E7000((u32*)igo, 0.0f, 0.0f); // letterbox
        retail::sub_689500((u32**)igo, 0);
    }

    string_hash stop_voice_bus;
    stop_voice_bus.initialize(mash::ALLOCATED, "stop_voice_bus");

    retail::sub_900D60(stop_voice_bus.source_hash_code, 0x10000000, 0); // post the sound event on the global game object

    if (was_completed_successfully && !suppress_dialogs) {
        saved_checkpoint_registry->clear();
        working_checkpoint_registry->clear();

        retail::sub_76E110((u32*)retail::sub_734220(), current_mission.mission_index);
        retail::sub_75F100(retail::sub_7685B0());
        retail::sub_9218C0((u32*)references::upgrade_manager.read()); // reverts the temporary upgrades

        if (state == mission_manager_state_running_mission) {
            entity* hero = references::g_world_ptr.read()->hero_ptr;

            if (hero && hero->damage_ifc())
                hero->damage_ifc()->set_hit_points(references::g_world_ptr.read()->hero_ptr->damage_ifc()->get_max_hit_points());

            last_successful_mission_time = real_time;

            mission_vars[current_mission.mission_index] = (f32)((f64)mission_vars[current_mission.mission_index] + 1.0);

            auto* header = (mission_header_instance_base*)current_mission.mission_header_instance->data.buffer;

            flags |= mission_manager_flag_show_succeeded_dialog;

            if (!(flags & mission_manager_flag_auto_launched)) {
                if (flags & mission_manager_flag_using_icon) {
                    flags &= ~mission_manager_flag_using_icon;

                    if (header->flags & mission_header_flag_unk_00000004) {
                        retail::sub_982590((u32*)this);

                        header->flags &= ~mission_header_flag_eligible;
                    } else {
                        igo_3d_zoom_map* zoom_map = nullptr;

                        if (references::frontend.get().igo)
                            zoom_map = references::frontend.get().igo->zoom_map;

                        mission_icon_info* icon = find_mission_icon(current_mission.mission_header_instance);

                        if (zoom_map && icon && icon->poi_index != -1 && retail::sub_690AB0((u32*)zoom_map, icon->poi_index)) {
                            retail::sub_6DFAC0((u32*)zoom_map, icon->poi_index);

                            icon->poi_index = -1;
                        }
                    }
                } else if (flags & mission_manager_flag_using_trigger) {
                    flags &= ~mission_manager_flag_using_trigger;

                    if (header->flags & mission_header_flag_unk_00000080)
                        retail::sub_982720((u32*)this);
                }
            }

            // enables the child mission
            if (header->child_inst)
                retail::sub_984750((u32*)this, 1);

            update_eligible_missions();
        }

        retail::sub_97FB10((u32*)this); // the all-missions achievement

        if (mission_index == 302)
            unk_295 = 1;

        if (!(flags & mission_manager_flag_show_succeeded_dialog) && !mission_finished_screen_has_appeared)
            retail::sub_7FEF20((u32*)quest_manager::inst(), (i32)&current_mission);
    } else if (!references::game.read()->game_paused && !suppress_dialogs && state == mission_manager_state_running_mission)
        flags |= mission_manager_flag_show_failed_dialog;
    else {
        if (!suppress_dialogs)
            retail::sub_7FFF00((i32*)quest_manager::inst(), (i32)&current_mission);

        retail::sub_76E110((u32*)retail::sub_734220(), current_mission.mission_index);
        retail::sub_75F100(retail::sub_7685B0());
        retail::sub_9218C0((u32*)references::upgrade_manager.read()); // reverts the temporary upgrades
    }

    retail::sub_9EB5D0((u32*)soap::online::inst(), 1, 4); // online::set_context
    soap::online::inst()->method_024(0);

    if (state == mission_manager_state_running_mission) {
        flags                        |= mission_manager_flag_need_to_unload;
        last_zoom_map_selection_time  = real_time;
        last_mission_time             = real_time;

        ++current_mission.exec->suspend_count;
    }

    if (!(flags & mission_manager_flag_show_failed_dialog))
        flags &= ~mission_manager_flag_terminating;
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
