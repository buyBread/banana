#pragma once

#include "game/wds/ai/states/combat_state.hh"
#include "game/wds/entity/interface/generic_interface.hh"
#include "game/wds/entity/interface/player_reflexes.hh"
#include "game/wds/entity/interface/plr_combat_attack.hh"
#include "game/input/game_axis.hh"
#include "game/input/game_button.hh"
#include "game/timing/simple_timer.hh"
#include "game/arch_base_vhandle.hh"
#include "game/boolx.hh"
#include "game/delta_variable.hh"
#include "game/line_info.hh"
#include "game/smoothilator.hh"
#include "treyarch/mash/array_replacement.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "game/math/algebra/vector3d.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"
#include "vc8/list.hh"
#include "vc8/vector.hh"

namespace treyarch {
    class ai_core;
    class ai_target;
    class entity;
    class entity_base;
    class player_combo_counter;
    class player_jump_info;
    class player_web_shooters;
    class polytube;
    class script_object_instance;
    class swing_data;
    class swing_params;
    struct swing_anchor_data;

    enum e_swinger_hand : i32;

    // SM3 ePostProcessAction
    enum e_post_process_action : i32 {
        pp_swing_anchor_search = 0x01,
        pp_swing_avoid_ground  = 0x02
    };

    // SM3 ePlayerButtonCommand, command_buttons' index; the combat block is 28 + e_combat_move
    enum e_player_button_command : i32 {
        plr_cmd_jump,
        plr_cmd_double_jump,
        plr_cmd_dual_swing,
        plr_cmd_swing,
        plr_cmd_zip,
        plr_cmd_crawl_attach,
        plr_cmd_sprint,
        plr_cmd_swing_boost,
        plr_cmd_zip_boost,
        plr_cmd_wall_sprint_attach,
        plr_cmd_pole_swing_attach,
        plr_cmd_yoyo,
        plr_cmd_wallplant,
        plr_cmd_evade,
        plr_cmd_air_evade,
        plr_cmd_block,
        plr_cmd_black,
        plr_cmd_parkour,
        plr_cmd_prop_throw,
        plr_cmd_trick_mode,
        plr_cmd_recenter_camera,
        plr_cmd_engage_freelook,
        plr_cmd_camera_lockon,
        plr_cmd_focus_target,
        plr_cmd_combat_branch_x,
        plr_cmd_combat_branch_y,
        plr_cmd_combat_branch_a,
        plr_cmd_combat_branch_b,
        plr_cmd_combat_light_melee,
        plr_cmd_combat_heavy_melee,
        plr_cmd_combat_special_attack,
        plr_cmd_combat_jump_attack,
        plr_cmd_combat_bounce_attack,
        plr_cmd_combat_counter_attack,
        plr_cmd_combat_dodge,
        plr_cmd_combat_block,
        plr_cmd_combat_shaba_save_civ,
        plr_cmd_combat_light_ranged,
        plr_cmd_combat_heavy_ranged,
        plr_cmd_combat_grapple,
        plr_cmd_combat_throw_manip,
        plr_cmd_combat_grab_civ,
        plr_cmd_combat_boss_finisher,
        plr_cmd_combat_car_finisher,
        plr_cmd_combat_boost_power_attack,
        plr_cmd_combat_boost_speed_attack,
        plr_cmd_combat_boost_air_attack,
        plr_cmd_combat_boost_web_attack,
        plr_cmd_combat_manual_dodge,
        plr_cmd_combat_script_move,
        plr_cmd_combat_momentum_light,
        plr_cmd_combat_momentum_heavy,
        plr_cmd_combat_momentum_grapple,
        plr_cmd_combat_stealth_kill,
        plr_cmd_combat_swing_light,
        plr_cmd_combat_swing_heavy,
        plr_cmd_combat_swing_grapple,
        plr_cmd_combat_light_wall_attack,
        plr_cmd_combat_heavy_wall_attack,
        plr_cmd_combat_enemy_swing_light,
        plr_cmd_combat_enemy_swing_heavy,
        plr_cmd_combat_enemy_swing_grapple,
        plr_cmd_combat_pole_swing_light,
        plr_cmd_combat_pole_swing_heavy,
        plr_cmd_combat_pole_swing_grapple,
        plr_cmd_combat_grab_obj,
        plr_cmd_trick_a,
        plr_cmd_trick_b,
        plr_cmd_trick_c,
        plr_cmd_trick_d,
        plr_cmd_web_shot,
        plr_cmd_interact,
        plr_cmd_dodge,
        plr_cmd_blacksuit,
        plr_cmd_redsuit,
        plr_cmd_ranged_interact,
        plr_cmd_recover,
        plr_cmd_reflexes,
        plr_cmd_senses,
        plr_cmd_focus_mode,
        plr_cmd_reel_web,
        plr_cmd_strength_test_interact,
        plr_cmd_strength_test_left,
        plr_cmd_strength_test_right,
        plr_cmd_strength_test_main,
        plr_cmd_strength_test_quit,
        plr_cmd_cinematic_camera_r,
        plr_cmd_cinematic_camera_l,
        plr_cmd_pause_game,
        plr_num_cmd
    };

    /* slots 19+ are player_interface's own. SM3's order holds around frame_advance
       (map_controls 89, update_interface 91, frame_advance 92, post_process 94), but WoS inserted slots:
       SM3 has two getters at 19/20 and no slot 93, which is named from its retail use */
    class player_interface : public actor_interface {

    public:
        bool                    unk_000c;      // slots 311 / 313 answer them
        bool                    unk_000d;
        u8                      pad_00e[0x02];
        u8                      reserved_010[0x230];
        delta_variable<i32>     mode;
        i32                     mode_last_frame;
        i32                     desired_mode;
        bool                    mode_locked;   // the spiderman_lock_mode native sets it
        bool                    unk_0251;      // the rvb's frame_advance skips slot 203 on a mode change while set
        u8                      reserved_0252[0x01];
        /* cleared by game::freeze_hero on the first freeze and set again on the last unfreeze and by unload_current_level;
           world_dynamics_system::frame_advance runs frame_advance while set and frame_advance_disabled otherwise */
        bool                    enabled;
        i32                     control_config;
        game_axis*              control_axis[4];
        player_axis_control*    player_axis[3];
        u8                      reserved_274[0x14];
        line_info               surface_info;  // als::crawl_transition moves to its hit_pos without attach_point
        i32                     current_jump_type;
        boolx                   allow_double_jump;
        bool                    unk_02e9;
        bool                    unk_02ea;
        bool                    unk_02eb;
        bool                    unk_02ec;
        u8                      pad_2ed[0x03];
        smoothilator<vector3d>  velocity_recorder_smoothilator;
        vector3d                last_position;
        vector3d                unk_030c;
        vc8::list<u32>*         unk_0318;      // nothing inserts into it
        plr_combat_attack*      attacks[combat_num_moves];
        e_combat_move           next_attack;
        plr_trick_move*         tricks[combat_and_tricks_num_moves - combat_max_moves - 1];
        e_combat_move           next_trick;
        i32                     unk_03cc;      // the als's max_trick_combo
        arch_base_vhandle       switch_ent;
        player_reflexes         reflexes;
        player_spider_senses    spider_senses;
        u8                      reserved_41c[0x10];
        player_combo_counter*   unk_042c;
        player_combo_counter*   combo_counter_display;
        simple_timer            combo_counter_delay;
        boolx                   transition;
        bool                    unk_0439;
        bool                    unk_043a;
        u8                      pad_43b[0x01];
        i32                     switch_can_be_flicked_function_index;
        f32                     ranged_max_switch_range;
        f32                     calced_jump_height;
        f32                     calced_jump_distance;
        vector3d                initial_jump_force_applied;
        vector3d                jump_start;
        vector3d                unk_0464;      // the wallplant normal
        arch_base_vhandle       unk_0470;
        arch_base_vhandle       unk_0474;
        mash::string            preset_dodge_anim;
        i32                     preset_dodge_type;
        mash::string            preset_block_anim;
        i32                     preset_block_type;
        arch_base_vhandle       combat_vehicle;
        f32                     unk_049c;
        i32                     post_process_flags;
        bool                    unk_04a4;
        bool                    unk_04a5;      // the rvb's check_for_block tests it
        bool                    unk_04a6;
        u8                      pad_4a7[0x01];
        arch_base_vhandle       unk_04a8;
        f32                     unk_04ac;      // PLR_MIN_TARGET_RANGE
        arch_base_vhandle       unk_4b0;       // an actor the als's target_ai rules read as the hero's target
        u8                      reserved_4b4[0x04];
        f32                     unk_4b8;       // counted down by the frame advance; the blackscreen sets it to its fade time
        script_object_instance* master_object; // the rvb's slot 24 builds it
        u8                      reserved_4c0[0x40];
        bool                    override_mode;
        u8                      reserved_501[0x1B];
        mash::array_replacement
            <game_button, 25>   buttons;
        mash::array_replacement
            <game_button, 89>   command_buttons;
        u8                      reserved_524[0x0C];

        explicit player_interface(mash::generating_vtable_version so_do_nothing) : actor_interface(so_do_nothing),
                                                                                    surface_info(so_do_nothing),
                                                                                    velocity_recorder_smoothilator(so_do_nothing),
                                                                                    combo_counter_delay(so_do_nothing),
                                                                                    preset_dodge_anim(so_do_nothing),
                                                                                    preset_block_anim(so_do_nothing) {}

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool         get_ifc_num(const string_hash &name, f32* value, bool document) override;
        bool         set_ifc_num(const string_hash &name, f32 value, bool document) override;
        bool         get_ifc_vec(const string_hash &name, vector3d* value, bool document) override;
        e_entity_ifc get_ifc_id() const override;

        void set_owner(actor* owner) override;

        virtual bool              is_enabled() const;
        virtual void              set_enabled(bool value);
        virtual f32               unk_021();
        virtual void              set_rpg_progress(f32 progress);
        virtual bool              unk_023(i32 unk);
        virtual void              unk_024(actor* owner);               // set_owner's last step; the rvb builds the hero's master script object
        virtual void*             unk_025(f32 delta_t);
        virtual bool              unk_026();
        virtual bool              unk_027(i32 unk);
        virtual bool              unk_028(i32 unk);
        virtual bool              unk_029() const;
        virtual f32               unk_030();
        virtual i32               unk_031(i32 unk);
        virtual i32               unk_032(i32 unk);
        virtual void              unk_033(i32 unk);
        virtual i32               unk_034(i32 unk);
        virtual i32               unk_035(i32 unk);
        virtual void*             unk_036(void* unk);
        virtual void              set_zip_override_anchor_num(f32 num);
        virtual f32               unk_038();
        virtual i32               unk_039(i32 unk);
        virtual arch_base_vhandle unk_040() const;
        virtual arch_base_vhandle unk_041(i32 index);                  // a null handle in player_interface
        virtual i32               unk_042();
        virtual void              unk_043(i32 unk);
        virtual bool              unk_044(i32 unk);
        virtual bool              unk_045();
        virtual void              set_zlock_entity(arch_base_vhandle entity);
        virtual void              unk_047(u32 unk);
        virtual arch_base_vhandle unk_048() const;                     // rvb: the zlock entity
        virtual bool              unk_049(i32 unk);
        virtual i32               unk_050(arch_base_vhandle target);
        virtual i32               unk_051(i32 unk, i32 unk_2, i32 unk_3);
        virtual void*             unk_052(void* unk);
        virtual void*             unk_053(void* unk, void* unk_2);
        virtual void              unk_054(arch_base_vhandle target, u32 kind);
        virtual arch_base_vhandle unk_055();                           // rvb: the timed target
        virtual f32               unk_056();
        virtual i32               unk_057();
        virtual void              unk_058(i32 unk);
        virtual void*             unk_059(void* unk);
        virtual i32               unk_060(arch_base_vhandle target);   // the hero's combat frame hands it its first target
        virtual arch_base_vhandle unk_061();                           // rvb: its +0x728; a null handle in player_interface
        virtual void*             unk_062(void* unk);
        virtual i32               unk_063(i32 unk);
        virtual i32               unk_064(i32 unk);
        virtual void*             unk_065(void* unk);
        virtual void              unk_066(i32 unk, i32 unk_2, i32 unk_3);
        virtual i32               unk_067(i32 unk);
        virtual bool              unk_068(i32 unk);
        virtual bool              unk_069();
        virtual i32               unk_070(i32 unk);
        virtual void*             unk_071(void* unk);
        virtual bool              unk_072();
        virtual bool              unk_073();
        virtual bool              unk_074();
        virtual void              set_desired_mode(i32 m);
        virtual i32               unk_076() const;
        virtual i32               unk_077() const;
        virtual void              set_swing_override_anchor(arch_base_vhandle anchor);
        virtual void              set_zip_override_anchor(arch_base_vhandle anchor);
        virtual arch_base_vhandle unk_080();
        virtual void              unk_081(f32 unk);
        virtual i32               unk_082(f32 unk);
        virtual i32               unk_083();
        virtual f32               get_movement_speed() const;
        virtual f32               unk_085();
        virtual void              unk_086(i32 unk);
        virtual bool              unk_087();
        virtual bool              unk_088(i32 unk);
        virtual bool              map_controls(i32 config, bool force);
        virtual void              render_phase();
        virtual void              update_interface();
        virtual void              frame_advance(f32 delta_t);
        virtual void              frame_advance_disabled(f32 delta_t); // clears input and swing state, ignores delta_t
        virtual void              post_process(f32 delta_t);
        virtual vector3d*         unk_095();                           // its own +0x2F0; the als takes it as a player's velocity
        virtual bool              unk_096(i32 unk);
        virtual bool              unk_097();
        virtual bool              unk_098(i32 unk);
        virtual bool              is_walking() const;
        virtual i32               unk_100();
        virtual i32               unk_101();
        virtual bool              is_wallsprinting() const;            // rvb_is_crawling asks 102..106 and 138 after crawling
        virtual bool              is_crawl_jumping();
        virtual bool              unk_104();
        virtual bool              unk_105();
        virtual bool              unk_106();
        virtual i32               unk_107();
        virtual i32               unk_108(i32 unk);
        virtual bool              is_transition() const;
        virtual bool              unk_110() const;
        virtual bool              unk_111() const;
        virtual void              set_transition(bool t, bool unk);    // unk picks which of unk_0439 / unk_043a goes up with t
        virtual bool              exterior_transition();
        virtual bool              unk_114(i32 unk);
        virtual bool              exterior_transition_180();
        virtual bool              unk_116(i32 unk);
        virtual bool              interior_transition();
        virtual i32               unk_118();
        virtual bool              unk_119(i32 unk);
        virtual bool              crawling() const;
        virtual i32               unk_121();
        virtual i32               unk_122();
        virtual i32               unk_123();
        virtual bool              unk_124();
        virtual bool              swing_r_hand() const;
        virtual bool              swing_l_hand() const;
        virtual bool              swing_any_hand();
        virtual bool              swing_both_hand();
        virtual void              swap_swing_hands(bool refire);
        virtual bool              swing_r_hand_locked() const;
        virtual bool              swing_l_hand_locked() const;
        virtual bool              swing_any_hand_locked();
        virtual bool              swing_both_hand_locked();
        virtual void              lock_swing_r_hand(bool l);
        virtual void              lock_swing_l_hand(bool l);
        virtual i32               unk_136();
        virtual bool              on_ground() const;
        virtual bool              on_wall() const;
        virtual bool              on_ceiling() const;
        virtual void              unk_140();                           // the snap and translate natives call it after a move
        virtual i32               unk_141();
        virtual bool              unk_142(bool unk);
        virtual bool              unk_143();
        virtual void              unk_144();
        virtual void              inc_jump_charge(f32 delta_t);
        virtual void              set_abs_jump_charge(f32 fraction);
        virtual void              clear_jump_charge(bool store_for_als);
        virtual f32               get_jump_charge() const;             // the als's active_jump_charge
        virtual f32               get_als_jump_charge() const;
        virtual bool              unk_150();                           // the als's blacksuit_swing_window
        virtual void              unk_151(i32 unk);
        virtual bool              unk_152();                           // the als's trick_window
        virtual bool              unk_153(i32 unk);
        virtual i32               unk_154();                           // the als's max_trick_combo
        virtual i32               unk_155(i32 unk);
        virtual bool              unk_156();
        virtual i32               predict_jump_type();
        virtual i32               unk_158(i32 unk);
        virtual bool              unk_159(i32 unk);
        virtual player_jump_info* get_jump_info(i32 jump);
        virtual i32               unk_161();
        virtual i32               unk_162(i32 unk);
        virtual void              unk_163();
        virtual void              unk_164();
        virtual i32               unk_165();
        virtual i32               unk_166();
        virtual bool              unk_167(i32 unk);
        virtual bool              unk_168();
        virtual bool              unk_169(i32 unk);
        virtual bool              unk_170();
        virtual bool              unk_171(i32 unk);
        virtual bool              unk_172();
        virtual bool              unk_173(i32 unk);
        virtual bool              unk_174();
        virtual bool              unk_175(i32 unk);
        virtual bool              unk_176();
        virtual bool              unk_177(i32 unk);
        virtual bool              unk_178();
        virtual bool              unk_179(i32 unk);
        virtual bool              unk_180();
        virtual i32               unk_181(i32 unk);
        virtual bool              unk_182();
        virtual bool              unk_183(bool unk);
        virtual bool              unk_184();
        virtual bool              unk_185(bool unk);
        virtual bool              unk_186();
        virtual void              unk_187(i32 unk);
        virtual bool              unk_188();
        virtual bool              unk_189(bool unk);
        virtual bool              unk_190();
        virtual bool              unk_191(bool unk);
        virtual bool              unk_192();
        virtual bool              unk_193(bool unk);
        virtual bool              handle_jump_charge(f32 delta_t);
        virtual i32               unk_195();
        virtual vector3d          unk_196();
        virtual i32               unk_197();
        virtual void              unk_198();
        virtual i32               unk_199(i32 unk);
        virtual void              unk_200(i32 unk);
        virtual i32               unk_201(f32 unk);
        virtual i32               unk_202();
        virtual void              unk_203();
        virtual vector3d          unk_204();
        virtual void*             unk_205(void* unk);
        virtual i32               unk_206(f32 unk, entity* unk_2, i32 unk_3);
        virtual void              unk_207();
        virtual void              unk_208(i32 unk);
        virtual void              unk_209(i32 unk);
        virtual f32               unk_210();

        virtual void                     force_attack(e_combat_move move, const ai_target* targ, i32 script_move);
        virtual void                     force_attack(e_combat_move move, const vc8::vector<ai_target>* targs, i32 script_move);
        virtual const plr_combat_attack* get_attack(e_combat_move att) const;
        virtual bool                     attack_possible(e_combat_move att) const;
        virtual i32                      unk_215();
        virtual e_combat_move            get_next_attack_enum() const;
        virtual const plr_combat_attack* get_next_attack() const;
        virtual void                     init_attacks();
        virtual void                     clear_attacks(bool all);
        virtual bool                     should_execute_combat(f32 delta_t, f32 tolerance);
        virtual void                     update_attacks(f32 delta_t);
        virtual void                     unk_222(i32 unk, i32 unk_2, i32 unk_3);
        virtual void                     unk_223(entity_base* ent, i32 unk_2); // takes the entity off the owner's hostiles
        virtual const plr_trick_move*    get_trick(e_combat_move trk) const;
        virtual e_combat_move            get_next_trick_enum() const;
        virtual const plr_trick_move*    get_next_trick() const;
        virtual i32                      get_trick_array_index(e_combat_move trk) const;
        virtual void                     init_tricks();
        virtual void                     clear_tricks();
        virtual void                     update_tricks(f32 delta_t);
        virtual bool                     check_for_dodge(      entity_base*               attacker,
                                                               i32                        chain_size,
                                                         const combo_move_data*           combo_move,
                                                               weapon_interface_extender* weapon,
                                                               entity_base*               weapon_entity,
                                                               i32                        usage_type);
        virtual bool                     should_execute_dodge(f32 delta_t);
        virtual const char*              get_dodge_anim() const;
        virtual void                     override_dodge_anim(const char* a);
        virtual i32                      get_dodge_type() const;
        virtual void                     override_dodge_type(i32 t);
        virtual bool                     check_for_block(      entity_base*               attacker,
                                                               i32                        chain_size,
                                                         const combo_move_data*           combo_move,
                                                               hit_react_record*          react,
                                                               weapon_interface_extender* weapon,
                                                               entity_base*               weapon_entity,
                                                               i32                        usage_type);
        virtual bool                     should_execute_block(f32 delta_t);
        virtual const char*              get_block_anim() const;
        virtual void                     override_block_anim(const char* a);
        virtual i32                      get_block_type() const;
        virtual void                     override_block_type(i32 t);

        virtual arch_base_vhandle unk_243();
        virtual arch_base_vhandle unk_244() const;
        virtual i32               unk_245();
        virtual f32               unk_246();
        virtual f32               unk_247();
        virtual bool              unk_248();
        virtual void              unk_249(bool unk);
        virtual f32               unk_250();
        virtual f32               get_block_strength();
        virtual void              unk_252(f32 unk);
        virtual f32               unk_253();
        virtual void              unk_254(f32 unk);
        virtual void              unk_255(f32 delta_t);
        virtual void              unk_256();
        virtual bool              unk_257();
        virtual arch_base_vhandle unk_258() const;
        virtual bool              unk_259(f32 delta_t);
        virtual bool              unk_260(f32 delta_t);
        virtual bool              unk_261(void* unk, void* unk_2, void* unk_3);
        virtual i32               unk_262(i32 unk, i32 unk_2);
        virtual bool              unk_263(void* unk, i32 unk_2);
        virtual bool              unk_264(f32 delta_t, void* unk_2, i32 unk_3, i32 unk_4, i32 unk_5);
        virtual bool              unk_265();
        virtual void              force_evade(vector3d v);
        virtual void              unk_267();
        virtual bool              unk_268();
        virtual vector3d          unk_269();
        virtual bool              unk_270();
        virtual bool              unk_271();
        virtual bool              unk_272();
        virtual entity*           unk_273(entity* target);             // what plr_combat_state::hit_target hits in its place
        virtual bool              should_pick_up_ai(f32 delta_t);
        virtual bool              unk_275(i32 unk);
        virtual void*             unk_276(void* unk);
        virtual bool              unk_277(ai_core* core);              // queues a pick up of the core's desired slave
        virtual bool              unk_278(f32 delta_t);
        virtual bool              unk_279(i32 unk);
        virtual void*             unk_280(void* unk);
        virtual bool              unk_281(ai_core* core);
        virtual bool              unk_282(ai_core* core);              // drops what the core's carry inode holds
        virtual bool              is_current_combat_target(arch_base_vhandle targ) const;
        virtual bool              unk_284();

        virtual player_combo_counter* unk_285();
        virtual player_combo_counter* get_combo_counter_display() const;
        virtual void                  inc_combo_counter(i32 num);
        virtual void                  set_combo_counter(i32 num);
        virtual player_combo_counter* unk_289(i32 unk);
        virtual player_combo_counter* unk_290(i32 unk);
        virtual void                  set_combo_counter_delay(f32 num, bool force);
        virtual i32                   get_combo_counter() const;
        virtual i32                   unk_293();
        virtual i32                   unk_294();

        virtual f32               unk_295();
        virtual void              unk_296(i32 unk);
        virtual f32               unk_297();
        virtual void              unk_298(i32 unk);
        virtual vector3d          get_initial_jump_force_applied() const;
        virtual void              set_initial_jump_force_applied(vector3d f);
        virtual vector3d          get_jump_start() const;
        virtual void              set_jump_start(vector3d s);
        virtual void*             unk_303(void* unk);
        virtual i32               unk_304(i32 unk);
        virtual bool              is_in_rvb_black_mode() const;
        virtual i32               unk_306();
        virtual i32               unk_307();
        virtual f32               unk_308();
        virtual void              unk_309(i32 unk);
        virtual f32               unk_310();
        virtual bool              unk_311();
        virtual bool              unk_312(bool unk);
        virtual bool              unk_313();
        virtual bool              unk_314(bool unk);
        virtual f32               unk_315();
        virtual i32               unk_316();
        virtual f32               unk_317();
        virtual f32               unk_318();
        virtual bool              unk_319(f32 delta_t, game_button* button, i32 unk_3);
        virtual void*             unk_320();
        virtual void*             unk_321(const line_info* unk);
        virtual bool              unk_322(i32 unk);
        virtual void*             unk_323(void* unk);
        virtual i32               unk_324(i32 unk);
        virtual f32               unk_325();
        virtual bool              unk_326();
        virtual bool              unk_327(i32 unk);
        virtual void*             unk_328();
        virtual void*             unk_329(void* unk);
        virtual void              unk_330(f32 duration, bool force);
        virtual void              unk_331(f32 duration, bool force);
        virtual void              unk_332(f32 duration, bool force);
        virtual void              unk_333(f32 duration, bool force);
        virtual void              unk_334(f32 duration, bool force);
        virtual void              unk_335(f32 duration, bool force);
        virtual void              unk_336(f32 duration, bool force);
        virtual i32               unk_337();
        virtual i32               unk_338();
        virtual i32               unk_339();
        virtual bool              unk_340();
        virtual bool              unk_341();
        virtual i32               unk_342();
        virtual i32               unk_343();
        virtual void              set_sweet_spot_incline_angle_degrees(f32 angle);
        virtual void              set_sweet_spot_distance(f32 dist);
        virtual void              set_sweet_cone_angle_degrees(f32 angle);
        virtual void              set_max_swing_web_length(f32 len);
        virtual void              set_min_swing_web_length(f32 len);
        virtual void              set_min_dynamic_sweet_length(f32 len);
        virtual void              set_max_dynamic_sweet_length(f32 len);
        virtual void              set_overdrive_sweet_length(f32 len);
        virtual void              unk_352(f32 len);
        virtual void              set_overdrive_velocity(f32 vel);
        virtual void              set_min_dynamic_sweet_angle(f32 angle);
        virtual void              set_max_dynamic_sweet_angle(f32 angle);
        virtual void              set_swing_ground_clearance(f32 clearance);
        virtual void              unk_357(f32 rate);
        virtual void              unk_358(f32 rate);
        virtual void              unk_359(f32 value);
        virtual void              unk_360(f32 value);
        virtual void              unk_361(f32 value);
        virtual void              unk_362(i32 unk, i32 unk_2, i32 unk_3, i32 unk_4);
        virtual void              get_sweet_spot_params(f32* incline_sin,
                                                        f32* incline_cos,
                                                        f32* sweet_cone_cos,
                                                        f32* distance,
                                                        f32* min_len_squared,
                                                        f32* max_len_squared);
        virtual void              get_dynamic_sweet_spot_params(f32* min_length,
                                                                f32* max_length,
                                                                f32* overdrive_length,
                                                                f32* overdrive_velocity,
                                                                f32* max_no_anchor_length,
                                                                f32* min_angle,
                                                                f32* max_angle);
        virtual void              get_blacksuit_swing_params(f32* swing_velocity_mod, f32* to_anchor_force);
        virtual void              get_reel_in_rate(f32* min_rate, f32* max_rate, f32* max_time, f32* max_range);
        virtual void              get_fake_anchor_params(f32* max_time, f32* pull_rate);
        virtual void              unk_368(f32 unk, swing_data* swinger);
        virtual bool              break_webs(bool add_momentum, bool add_dangler);
        virtual bool              break_web(e_swinger_hand web_to_break, bool add_momentum, bool add_dangler);
        virtual void              unk_371(e_swinger_hand hand, bool hide); // rvb: leaves a copy of the hand's web dangling
        virtual entity*           get_last_anchor() const;
        virtual void              hide_web_dangles();
        virtual f32               get_time_on_web();                   // the als's swing_time
        virtual f32               unk_375();
        virtual player_web_shooters* get_web_shooters();
        virtual bool              unk_377(vector3d* point);            // rvb: the anchor of whichever of its two hands (+0x638) holds one
        virtual bool              unk_378();
        virtual i32               unk_379();
        virtual i32               unk_380(i32 unk);
        virtual f32               weight_swing_anchor(const vector3d &point, const vector3d &normal, f32 dist_sq);
        virtual bool              unk_382(i32 unk);
        virtual i32               unk_383();
        virtual bool              unk_384();
        virtual i32               unk_385();
        virtual bool              unk_386(f32 unk, bool unk_2);
        virtual void*             unk_387();                           // rvb: the pole it swings on (+0x8CC)
        virtual bool              unk_388();                           // rvb: that pole leans more than 45 degrees off vertical
        virtual bool              unk_389(void* unk, f32 unk_2);
        virtual bool              unk_390();
        virtual bool              unk_391(i32 unk, i32 unk_2);
        virtual bool              unk_392(bool skip);
        virtual bool              unk_393(f32 unk);
        virtual swing_data*       get_active_swinger();
        virtual e_swinger_hand    get_active_swinger_hand();
        virtual e_swinger_hand    get_other_swinger_hand(e_swinger_hand cur_hand);
        virtual swing_data*       get_last_fired_swinger();
        virtual swing_data*       choose_next_swinger_to_fire(e_swinger_hand which_hand);
        virtual swing_data*       get_swinger(e_swinger_hand i);
        virtual void              unk_400(polytube* tube, f32 radius); // dresses the polytube as the hero's web
        virtual i32               get_als_swing_hand_value();          // the als's swing_hand
        virtual void              apply_awesome_delta(f32 boost_value, f32 delta_t);
        virtual f32               get_awesome_rating() const;          // the als's swing_level_rating
        virtual f32               unk_404(f32 progress);
        virtual f32               unk_405(f32 progress);
        virtual f32               unk_406(f32 progress);
        virtual f32               unk_407(f32 progress);
        virtual void              set_swinger_info(swing_data* the_swinger, const swing_anchor_data* data);
        virtual void              set_zip_info(swing_data* the_swinger, const line_info* data, bool enable_pendulum);
        virtual void              unk_410(swing_data*      first_swinger, // rvb: zips both hands at once, pendulums off
                                          swing_data*      second_swinger,
                                          const line_info* first_data,
                                          const line_info* second_data);
        virtual void              set_yoyo_info(swing_data* the_swinger, const line_info* data);
        virtual void              unk_412(swing_data*  the_swinger,    // rvb: zips onto a point in the pivot's space
                                          entity_base* pivot,
                                          vector3d     pos,
                                          bool         enable_pendulum);
        virtual i32               unk_413(i32 unk, i32 unk_2, void* unk_3);
        virtual f32               get_swing_collision_break_length() const;
        virtual f32               get_swing_collision_stick_length() const;
        virtual f32               unk_416();
        virtual i32               unk_417();
        virtual i32               unk_418();
        virtual i32               unk_419();
        virtual i32               unk_420();
        virtual i32               unk_421();
        virtual i32               unk_422();
        virtual i32               unk_423();
        virtual f32               compute_swing_angle();
        virtual f32               get_als_swing_angle();
        virtual f32               compute_swing_side_angle();
        virtual f32               get_als_swing_side_angle();
        virtual f32               compute_swing_to_anchor_angle();
        virtual f32               get_als_swing_to_anchor_angle();
        virtual f32               unk_430();
        virtual f32               unk_431();                           // slot 430 in degrees
        virtual f32               get_als_swing_entry_speed();
        virtual f32               get_als_swing_exit_speed();
        virtual f32               get_als_lateral_speed();
        virtual f32               unk_435();
        virtual f32               unk_436();
        virtual f32               unk_437();
        virtual f32               unk_438();
        virtual void              unk_439(i32 unk);
        virtual f32               get_swing_boost_cooldown_time() const;
        virtual f32               get_min_swing_boost_angle() const;
        virtual f32               get_max_swing_boost_angle() const;
        virtual f32               get_swing_boost_force() const;
        virtual f32               unk_444();
        virtual f32               get_swing_jump_boost() const;
        virtual f32               get_swing_drag_coefficient() const;
        virtual f32               get_swing_drag_coefficient_min_speed() const;
        virtual f32               get_swing_drag_coefficient_horz_scale() const;
        virtual f32               get_swing_drag_coefficient_up_scale() const;
        virtual f32               get_swing_drag_coefficient_down_scale() const;
        virtual f32               get_swing_ground_clearance() const;
        virtual f32               unk_452();
        virtual void              set_new_swing_record_keeping(f32 swing_entry_speed,
                                                               f32 to_anchor_angle,
                                                               f32 to_anchor_length);
        virtual f32               get_swing_max_dot_for_velocity_preservation() const;
        virtual void              set_swing_max_dot_for_velocity_preservation(f32 d);
        virtual bool              get_swing_avoid_ground() const;
        virtual void              set_swing_avoid_ground(bool b);
        virtual void              unk_458(void* unk, void* unk_2, f32 unk_3);
        virtual i32               unk_459();
        virtual i32               unk_460(f32 unk);
        virtual bool              should_do_strength_test() const;
        virtual bool              is_a_scripted_strength_test() const;
        virtual bool              unk_463();
        virtual i32               unk_464();
        virtual void              unk_465();
        virtual bool              unk_466();
        virtual void*             unk_467();                           // rvb: clears +0xA44's first byte and returns it, a handle at +4
        virtual void              unk_468();
        virtual bool              unk_469();
        virtual bool              unk_470();
        virtual i32               unk_471();
        virtual void              unk_472();
        virtual arch_base_vhandle get_combat_vehicle() const;
        virtual void              set_combat_vehicle(arch_base_vhandle v);
        virtual f32               unk_475();
        virtual void              unk_476(i32 unk);
        virtual void              set_post_process_flag(e_post_process_action f, bool set);
        virtual bool              get_post_process_flag(e_post_process_action f);
        virtual bool              unk_479();
        virtual bool              unk_480(bool unk);
        virtual bool              unk_481();
        virtual bool              unk_482(bool unk);
        virtual arch_base_vhandle unk_483(const plr_combat_attack* attack) const;
        virtual bool              unk_484();
        virtual bool              unk_485();
        virtual bool              unk_486(bool unk, f32 delay);        // rvb: stores both at +0xB5C and +0xB54
        virtual bool              unk_487();
        virtual bool              unk_488(bool unk, f32 delay);        // rvb: stores both at +0xB5D and +0xB58
        virtual i32               unk_489();
        virtual i32               unk_490();
        virtual f32               unk_491();
        virtual void              unk_492(bool start);
        virtual i32               unk_493();
        virtual i32               unk_494();
        virtual i32               is_in_webcast_approach_normal_hit_range();
        virtual i32               is_in_webcast_approach_critical_hit_range();
        virtual i32               is_in_approach_counter();
        virtual i32               is_in_webcast_approach_early_range();
        virtual i32               is_in_webcast_approach_counter_range();
        virtual i32               unk_500();
        virtual i32               unk_501();
        virtual f32               approach_norm_to_crit_percent();
        virtual i32               unk_503();
        virtual bool              is_webcasting_entity(arch_base_vhandle entity);
        virtual f32               unk_505();
        virtual void              unk_506(bool start);
        virtual const vector3d*   unk_507(const vector3d* v);
        virtual vector3d          unk_508() const;                     // the owner's z facing in player_interface
        virtual i32               unk_509();
        virtual void              unk_510(bool start);
        virtual bool              unk_511();
        virtual bool              unk_512(bool unk);
        virtual bool              unk_513(i32 unk);
        virtual void*             unk_514(i32 unk);
        virtual bool              is_in_wallmode();
        virtual i32               unk_516(i32 unk, void* unk_2, bool unk_3);
        virtual i32               unk_517();
        virtual bool              unk_518(i32 unk);
        virtual bool              unk_519();
        virtual bool              unk_520();
        virtual void              unk_521();
        virtual i32               unk_522();
        virtual void              lock_suit(bool lock, bool black);
        virtual bool              unk_524();
        virtual void              mode_lock_suit(bool lock, f32 time);
        virtual void              unk_526();
        virtual void              unk_527(f32 delta_t);
        virtual line_info*        unk_528();                           // rvb: a line_info of its own (+0xBA8)
        virtual void              unk_529(const line_info* line);
        virtual bool              unk_530(f32 delta_t);
        virtual bool              unk_531(const line_info* line);
        virtual bool              unk_532(const line_info* unk);       // rvb: null takes that line_info
        virtual i32               unk_533(i32 unk);
        virtual void              unk_534(arch_base_vhandle target, f32 time);
        virtual entity_base*      select_lockon_target(const vector3d &base_pos,
                                                             f32       min_range,
                                                             f32       max_range,
                                                             f32       min_dotp,
                                                       const vector3d &dir,
                                                       const vector3d &up) const;

        static e_combat_move previous_next_attack; // update_attacks keeps last update's next_attack here

        i32 get_mode() const {
            return mode.current;
        }

        i32 get_prev_mode() const {
            return mode.previous;
        }

        void enable_axis(i32 ctrl, bool v) {
            control_axis[ctrl]->enable(v);
        }

        game_button* get_button(i32 ctrl) {
            return &buttons[ctrl];
        }

        // sub_783720
        // inlined @ sub_7976E0
        // inlined @ sub_86B470
        void enable_button(i32 ctrl, bool v) {
            buttons[ctrl].enable(v);
        }

        game_button* get_command_button(e_player_button_command cmd) {
            return &command_buttons[cmd];
        }

        void clear_buttons();
        void clear_controller(bool unk);
        void override_button(i32 ctrl, f32 value, f32 t);
        void override_command(e_player_button_command cmd, f32 value, f32 t);
        void set_button(i32 ctrl, f32 value);
        void set_axis(i32 ctrl, bool ud, f32 value);

        player_reflexes* get_reflexes() {
            return &reflexes;
        }

        player_spider_senses* get_spider_senses() {
            return &spider_senses;
        }

        bool is_carrying_or_carried() const;
    };

    ASSERT_SIZEOF  (player_interface,                                       0x530);
    ASSERT_OFFSETOF(player_interface, unk_000c,                             0x00C);
    ASSERT_OFFSETOF(player_interface, mode,                                 0x240);
    ASSERT_OFFSETOF(player_interface, desired_mode,                         0x24C);
    ASSERT_OFFSETOF(player_interface, mode_locked,                          0x250);
    ASSERT_OFFSETOF(player_interface, enabled,                              0x253);
    ASSERT_OFFSETOF(player_interface, control_config,                       0x254);
    ASSERT_OFFSETOF(player_interface, control_axis,                         0x258);
    ASSERT_OFFSETOF(player_interface, player_axis,                          0x268);
    ASSERT_OFFSETOF(player_interface, surface_info,                         0x288);
    ASSERT_OFFSETOF(player_interface, current_jump_type,                    0x2E4);
    ASSERT_OFFSETOF(player_interface, velocity_recorder_smoothilator,       0x2F0);
    ASSERT_OFFSETOF(player_interface, unk_0318,                             0x318);
    ASSERT_OFFSETOF(player_interface, attacks,                              0x31C);
    ASSERT_OFFSETOF(player_interface, next_attack,                          0x3B4);
    ASSERT_OFFSETOF(player_interface, tricks,                               0x3B8);
    ASSERT_OFFSETOF(player_interface, next_trick,                           0x3C8);
    ASSERT_OFFSETOF(player_interface, switch_ent,                           0x3D0);
    ASSERT_OFFSETOF(player_interface, reflexes,                             0x3D4);
    ASSERT_OFFSETOF(player_interface, spider_senses,                        0x3FC);
    ASSERT_OFFSETOF(player_interface, unk_042c,                             0x42C);
    ASSERT_OFFSETOF(player_interface, transition,                           0x438);
    ASSERT_OFFSETOF(player_interface, switch_can_be_flicked_function_index, 0x43C);
    ASSERT_OFFSETOF(player_interface, initial_jump_force_applied,           0x44C);
    ASSERT_OFFSETOF(player_interface, preset_dodge_anim,                    0x478);
    ASSERT_OFFSETOF(player_interface, preset_block_anim,                    0x488);
    ASSERT_OFFSETOF(player_interface, combat_vehicle,                       0x498);
    ASSERT_OFFSETOF(player_interface, unk_04ac,                             0x4AC);
    ASSERT_OFFSETOF(player_interface, unk_4b0,                              0x4B0);
    ASSERT_OFFSETOF(player_interface, unk_4b8,                              0x4B8);
    ASSERT_OFFSETOF(player_interface, master_object,                        0x4BC);
    ASSERT_OFFSETOF(player_interface, override_mode,                        0x500);
    ASSERT_OFFSETOF(player_interface, buttons,                              0x51C);
    ASSERT_OFFSETOF(player_interface, command_buttons,                      0x520);
} // treyarch
