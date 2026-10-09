#pragma once

#include "treyarch/shared/arch_base_vhandle.hh"
#include "treyarch/shared/dinkumware/list.hh"
#include "treyarch/shared/dinkumware/vector.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/math/types/vector3.hh"
#include "treyarch/shared/singleton.hh"
#include "treyarch/shared/timing/hires_clock.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    namespace chuck { namespace vm {
        class script_executable;
        class script_instance;
    }} // chuck::vm

    class event;
    struct region;

    enum e_mission_manager_state : u32 {
        mission_manager_state_initial_startup,
        mission_manager_state_idle,
        mission_manager_state_start_loading,
        mission_manager_state_loading,
        mission_manager_state_running_mission,
        mission_manager_state_unk_5, // no milestone
        mission_manager_state_unk_6, // no milestone
        mission_manager_state_start_unloading,
        mission_manager_state_unloading,
        mission_manager_state_maloring_player_wait_for_blackscreen,
        mission_manager_state_maloring_player_wait_for_district,
        mission_manager_state_start_mission_failed_dialog,
        mission_manager_state_running_mission_failed_dialog,
        mission_manager_state_start_mission_succeeded_dialog,
        mission_manager_state_running_mission_succeeded_dialog,
        mission_manager_state_blackscreen_on,
        mission_manager_state_wait_for_blackscreen_on
    };

    enum e_mission_manager_flags : u32 {
        mission_manager_flag_mission_ready  = 0x00000001,
        mission_manager_flag_need_to_unload = 0x00000004,
        mission_manager_flag_using_trigger  = 0x00000008,
        mission_manager_flag_using_icon     = 0x00000010,
        mission_manager_flag_need_to_malor  = 0x00000020,

        // no milestone names past this point
        mission_manager_flag_unk_00001000 = 0x00001000,
        mission_manager_flag_unk_00002000 = 0x00002000,
        mission_manager_flag_unk_00004000 = 0x00004000,
        mission_manager_flag_unk_00008000 = 0x00008000,
        mission_manager_flag_unk_00010000 = 0x00010000,
        mission_manager_flag_unk_00020000 = 0x00020000,
        mission_manager_flag_unk_00040000 = 0x00040000,
        mission_manager_flag_unk_00200000 = 0x00200000,
        mission_manager_flag_unk_00400000 = 0x00400000,
        mission_manager_flag_unk_00800000 = 0x00800000,
        mission_manager_flag_unk_02000000 = 0x02000000,
        mission_manager_flag_unk_04000000 = 0x04000000,
        mission_manager_flag_unk_08000000 = 0x08000000,
        mission_manager_flag_unk_10000000 = 0x10000000,
        mission_manager_flag_unk_20000000 = 0x20000000
    };

    enum e_mission_header_flags : u32 {
        mission_header_flag_eligible_required = 0x00000001,
        mission_header_flag_eligible          = 0x00000002,
        mission_header_flag_uses_trigger      = 0x00000010,

        // no sm3 names past this point
        mission_header_flag_unk_00004000 = 0x00004000,
        mission_header_flag_unk_00008000 = 0x00008000,
        mission_header_flag_unk_00010000 = 0x00010000,
        mission_header_flag_unk_00020000 = 0x00020000, // set once the instance sits in mission_icons or mission_triggers
        mission_header_flag_unk_00800000 = 0x00800000,
        mission_header_flag_unk_01000000 = 0x01000000,
        mission_header_flag_unk_02000000 = 0x02000000,
        mission_header_flag_unk_04000000 = 0x04000000
    };

    // the script data of a mission header instance
    struct mission_header_instance_base {
        vector3                     key_position;
        u8                          reserved_00c[0x10];
        f32                         unk_01c;
        u8                          reserved_020[0x14];
        chuck::vm::script_instance* child_inst;
        u32                         child_eligibility_id;
        u32                         flags;
        f32                         eligibility_array[10];
        f32                         trigger_radius;
        u8                          reserved_06c[0x08];
        i32                         mission_icon_index;
    };

    struct mission_info {
        mash::string                      name;
        i32                               mission_index           = 0;
        u32                               icon                    = 0;
        u32                               ped                     = 0;
        vector3                           key_position;
        const chuck::vm::script_instance* mission_header_instance = nullptr;
        mash::string                      gen_dis_exec_name;
        chuck::vm::script_executable*     exec                    = nullptr;
        i32                               retry_count             = 0;
        u32                               unk_03c                 = 0;

        mission_info();

        mission_info &operator=(const mission_info &other);
    };

    // SM3 mission_district_info without its leading m_question_mark_icons
    struct mission_district_info {
        struct region*                region                     = nullptr;
        void*                         pack_slot                  = nullptr;
        i32                           gen_dis_script_frame_count = 0;
        chuck::vm::script_executable* gen_dis_exec               = nullptr;
        string_hash                   gen_dis_exec_name;

        mission_district_info();
    };

    struct mission_icon_info {
        i32                               poi_index; // the zoom map's, -1 while it has none
        const chuck::vm::script_instance* instance;
        u32                               unk_008;
    };

    struct mission_poi_location_t {
        i32     key;
        vector3 position;
    };

    // RTTI eligible_mission_visitor, a script_instance_visitor
    struct eligible_mission_visitor {
        void*   vtable;
        u32     unk_004;
        vector3 hero_position;
        i32     unk_014;
    };

    class mission_manager : public singleton_instance<mission_manager, 0x01111760> {

    public:
        chuck::vm::script_executable* gen_global_exec;
        string_hash                   gen_global_exec_name;
        mission_district_info         district_containers[8];
        i32                           district_count;
        f32                           startup_timer;
        u32                           real_time;
        f32*                          real_time_var;
        f32                           real_time_timer;
        u32                           game_time;
        f32*                          game_time_var;
        f32                           game_time_timer;
        u32                           game_time_rate;
        u32                           saved_game_time_rate;
        f32*                          game_days_var;
        f32*                          game_day_of_the_week_var;
        u32                           unk_0d8[3];
        u32                           last_mission_time;
        u32                           last_mission_interval;
        u32                           last_successful_mission_time;
        u32                           last_successful_mission_interval;
        u32                           last_zoom_map_selection_time;
        u32                           last_zoom_map_selection_interval;
        u32                           unk_0fc[4];
        f32                           unk_10c;
        f32                           unk_110;
        u32                           reserved_114;
        e_mission_manager_state       state;
        mission_info                  current_mission;
        mission_info                  next_mission;
        u32                           flags;
        dinkumware::vector
            <mission_icon_info>*      mission_icons;
        dinkumware::vector
            <mission_icon_info>*      mission_triggers;
        dinkumware::vector
            <mission_icon_info>*      mission_ped_triggers;
        f32*                          mission_vars;
        f32*                          upgrade_vars;
        u32                           reserved_1b4;
        u32                           unk_1b8;
        u32                           unk_1bc;
        void*                         saved_checkpoint_registry;
        void*                         working_checkpoint_registry;
        f32                           state_timer;
        f32                           idle_event_cooldown;
        vector3                       player_restart_position;
        vector3                       player_restart_xz_facing;
        vector3                       hero_reference_position;
        region*                       hero_region;
        region*                       malor_region;
        u32                           malor_wait_frames;
        u8                            unk_200;
        u8                            reserved_201[0x03];
        dinkumware::vector
            <const chuck::vm::
             script_instance*>*       eligible_mission_instances;
        dinkumware::list
            <const chuck::vm::
             script_instance*>*       auto_launch_instances;
        u32                           unk_20c;
        u32                           unk_210;
        f32                           unk_214;
        hires_clock_t                 frame_clock;
        u32                           zoom_map_poi_selected_callback_id;
        u32                           zoom_map_poi_unselected_callback_id;
        void*                         selected_poi_icon;
        mission_icon_info             selected_poi;
        void*                         unk_238;
        u8                            unk_23c;
        u8                            reserved_23d[0x03];
        string_hash                   unk_240;
        f32                           unk_244;
        i32                           saved_script_hero_frozen_count;
        i32                           saved_script_hero_ai_disable_count;
        i32                           saved_script_hero_suspend_count;
        i32                           saved_script_hero_invulnerable_count;
        i32                           act;
        i32                           current_act;
        i32                           requested_act;
        i32                           unk_264;
        i32                           act_transition_state;
        i32                           act_transition_countdown;
        u8                            act_force_reload;
        u8                            reserved_271[0x03];
        u32                           act_transition_frames;
        mash::string                  sky_name;
        mash::string                  wanted_sky_name;
        i32                           sky_swap_state;
        u8                            sky_force_reload;
        u8                            unk_295;
        u8                            reserved_296[0x02];
        f32                           leave_radius_squared;
        dinkumware::vector
            <mission_poi_location_t>* poi_locations;
        i32                           selected_poi_location_key;
        vector3                       selected_poi_location_position;
        u32                           checkpoint;
        string_hash                   saved_mission_hash;
        i32                           saved_checkpoint;
        u8                            is_quitable;
        u8                            reserved_2bd[0x03];
        i32                           success_screen_type;
        i32                           success_screen_parameter;
        u8                            mission_finished_screen_has_appeared;
        u8                            first_act_request;
        u8                            reserved_2ca[0x02];
        u32                           script_globals_initialized;

        mission_manager();

        static void create_inst();

        bool                          is_idle() const;
        bool                          is_mission_running() const;
        bool                          is_mission_ready() const;
        bool                          needs_to_malor() const;
        bool                          is_maloring() const;
        e_mission_manager_state       get_state() const;
        chuck::vm::script_executable* get_running_mission_exec() const;
        vector3                       get_mission_key_position() const;
        i32                           get_running_script_index() const;
        const char*                   get_running_mission_name() const;
        i32                           get_mission_script_retry_count() const;
        i32                           get_current_act() const;
        i32                           get_current_act_internal() const;
        i32                           get_requested_act() const;
        bool                          is_act_transitioning() const;
        bool                          has_auto_launch_instances() const;
        i32                           get_total_times_script_has_been_successfully_completed() const;

        static bool get_mission_caption_index(u32 icon, i32* index);

        i32  game_time_get_military_time_hour() const;
        u32  game_time_get_seconds_since_midnight() const;
        f32  game_time_get_full_time() const;
        bool is_daytime() const;
        void frame_advance_game_time(f32 time_inc);

        void frame_advance(f32 time_inc);

        void advance_act_transition();
        void begin_act_transition();
        void load_act_pack();

        i32 get_mission_act(const mission_info &info) const;

        mash::string get_sky_name() const;

        void check_sky_name();
        void load_sky_pack();
        void apply_sky_pack();

        void update_hero_proximity_event();
        void update_hero_tracking_flags();
        void run_district_scripts();

        void update_eligible_missions();
        void add_mission_icon(const chuck::vm::script_instance* instance);
        void update_mission_icons();
        void check_mission_triggers();
        void launch_auto_launch_instance(const chuck::vm::script_instance* instance);

        void select_poi(const mission_icon_info &poi, bool);
        void clear_selected_poi(bool clear_zoom_map);

        static void on_zoom_map_poi_selected  (event* raised_event, arch_base_vhandle recipient, void* parameters);
        static void on_zoom_map_poi_unselected(event* raised_event, arch_base_vhandle recipient, void* parameters);

        void save_player_restart(const vector3 &position, vector3 xz_facing);

        static void play_open_city_music();
        static void unpause_and_notify_frontend();

        void process_state_initial_startup(f32 time_inc);
        void process_state_idle(f32 time_inc);
        void process_state_start_loading();
        void process_state_loading();
        void process_state_running_mission();
        void process_state_unk_5();
        void process_state_unk_6();
        void process_state_start_unloading();
        void process_state_unloading();
        void process_state_maloring_player_wait_for_blackscreen(f32 time_inc);
        void process_state_maloring_player_wait_for_district();
        void process_state_start_mission_failed_dialog();
        void process_state_running_mission_failed_dialog();
        void process_state_running_mission_succeeded_dialog();
        void process_state_blackscreen_on();
        void process_state_wait_for_blackscreen_on(f32 time_inc);
    };

    namespace references {
        // seeds mission_manager::current_act
        inline util::memory_reference<i32> global_act { 0x0111186C };

        // .rdata 0; the unset value of the zoom-map callback ids
        inline util::memory_reference<u32> unk_00bb6d68 { 0x00BB6D68 };

        // a zero vector; reset target for the hero reference position
        inline util::memory_reference<vector3> unk_011117a0 { 0x011117A0 };

        inline util::memory_reference<string_hash> registry_mission_manager          { 0x011117B8 };
        inline util::memory_reference<string_hash> registry_player_restart_position  { 0x011117F4 };
        inline util::memory_reference<string_hash> registry_player_restart_xzfacing  { 0x0111187C };
        inline util::memory_reference<string_hash> mission_header_instance_base_name { 0x011117C4 };

        inline util::memory_reference<void*> eligible_mission_visitor_vtable { 0x00BEA26C };
    } // references

    ASSERT_OFFSETOF(mission_header_instance_base, unk_01c,              0x1C);
    ASSERT_OFFSETOF(mission_header_instance_base, child_inst,           0x34);
    ASSERT_OFFSETOF(mission_header_instance_base, child_eligibility_id, 0x38);
    ASSERT_OFFSETOF(mission_header_instance_base, flags,                0x3C);
    ASSERT_OFFSETOF(mission_header_instance_base, trigger_radius,       0x68);
    ASSERT_OFFSETOF(mission_header_instance_base, mission_icon_index,   0x74);

    ASSERT_SIZEOF  (mission_info,                          0x40);
    ASSERT_OFFSETOF(mission_info, mission_index,           0x0C);
    ASSERT_OFFSETOF(mission_info, key_position,            0x18);
    ASSERT_OFFSETOF(mission_info, mission_header_instance, 0x24);
    ASSERT_OFFSETOF(mission_info, gen_dis_exec_name,       0x28);
    ASSERT_OFFSETOF(mission_info, exec,                    0x34);
    ASSERT_OFFSETOF(mission_info, retry_count,             0x38);

    ASSERT_SIZEOF  (mission_district_info,                             0x14);
    ASSERT_OFFSETOF(mission_district_info, gen_dis_script_frame_count, 0x08);
    ASSERT_OFFSETOF(mission_district_info, gen_dis_exec_name,          0x10);

    ASSERT_SIZEOF  (mission_icon_info,           0x0C);
    ASSERT_OFFSETOF(mission_icon_info, instance, 0x04);

    ASSERT_SIZEOF(mission_poi_location_t, 0x10);

    ASSERT_SIZEOF  (eligible_mission_visitor,                0x18);
    ASSERT_OFFSETOF(eligible_mission_visitor, hero_position, 0x08);
    ASSERT_OFFSETOF(eligible_mission_visitor, unk_014,       0x14);

    ASSERT_SIZEOF  (mission_manager,                                    0x2D0);
    ASSERT_OFFSETOF(mission_manager, district_containers,               0x008);
    ASSERT_OFFSETOF(mission_manager, district_count,                    0x0A8);
    ASSERT_OFFSETOF(mission_manager, startup_timer,                     0x0AC);
    ASSERT_OFFSETOF(mission_manager, real_time,                         0x0B0);
    ASSERT_OFFSETOF(mission_manager, game_time,                         0x0BC);
    ASSERT_OFFSETOF(mission_manager, game_time_rate,                    0x0C8);
    ASSERT_OFFSETOF(mission_manager, game_day_of_the_week_var,          0x0D4);
    ASSERT_OFFSETOF(mission_manager, last_mission_time,                 0x0E4);
    ASSERT_OFFSETOF(mission_manager, last_zoom_map_selection_time,      0x0F4);
    ASSERT_OFFSETOF(mission_manager, unk_10c,                           0x10C);
    ASSERT_OFFSETOF(mission_manager, state,                             0x118);
    ASSERT_OFFSETOF(mission_manager, current_mission,                   0x11C);
    ASSERT_OFFSETOF(mission_manager, next_mission,                      0x15C);
    ASSERT_OFFSETOF(mission_manager, flags,                             0x19C);
    ASSERT_OFFSETOF(mission_manager, mission_icons,                     0x1A0);
    ASSERT_OFFSETOF(mission_manager, mission_vars,                      0x1AC);
    ASSERT_OFFSETOF(mission_manager, saved_checkpoint_registry,         0x1C0);
    ASSERT_OFFSETOF(mission_manager, state_timer,                       0x1C8);
    ASSERT_OFFSETOF(mission_manager, player_restart_position,           0x1D0);
    ASSERT_OFFSETOF(mission_manager, hero_reference_position,           0x1E8);
    ASSERT_OFFSETOF(mission_manager, malor_region,                      0x1F8);
    ASSERT_OFFSETOF(mission_manager, unk_200,                           0x200);
    ASSERT_OFFSETOF(mission_manager, eligible_mission_instances,        0x204);
    ASSERT_OFFSETOF(mission_manager, unk_20c,                           0x20C);
    ASSERT_OFFSETOF(mission_manager, frame_clock,                       0x218);
    ASSERT_OFFSETOF(mission_manager, zoom_map_poi_selected_callback_id, 0x220);
    ASSERT_OFFSETOF(mission_manager, selected_poi_icon,                 0x228);
    ASSERT_OFFSETOF(mission_manager, selected_poi,                      0x22C);
    ASSERT_OFFSETOF(mission_manager, unk_238,                           0x238);
    ASSERT_OFFSETOF(mission_manager, saved_script_hero_frozen_count,    0x248);
    ASSERT_OFFSETOF(mission_manager, act,                               0x258);
    ASSERT_OFFSETOF(mission_manager, act_transition_state,              0x268);
    ASSERT_OFFSETOF(mission_manager, act_transition_frames,             0x274);
    ASSERT_OFFSETOF(mission_manager, sky_name,                          0x278);
    ASSERT_OFFSETOF(mission_manager, wanted_sky_name,                   0x284);
    ASSERT_OFFSETOF(mission_manager, sky_swap_state,                    0x290);
    ASSERT_OFFSETOF(mission_manager, leave_radius_squared,              0x298);
    ASSERT_OFFSETOF(mission_manager, poi_locations,                     0x29C);
    ASSERT_OFFSETOF(mission_manager, selected_poi_location_key,         0x2A0);
    ASSERT_OFFSETOF(mission_manager, selected_poi_location_position,    0x2A4);
    ASSERT_OFFSETOF(mission_manager, checkpoint,                        0x2B0);
    ASSERT_OFFSETOF(mission_manager, is_quitable,                       0x2BC);
    ASSERT_OFFSETOF(mission_manager, success_screen_type,               0x2C0);
    ASSERT_OFFSETOF(mission_manager, first_act_request,                 0x2C9);
    ASSERT_OFFSETOF(mission_manager, script_globals_initialized,        0x2CC);
} // treyarch
