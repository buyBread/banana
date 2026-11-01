#pragma once

#include "game/wds/ai/states/combat_state.hh"
#include "game/wds/entity/interface/generic_interface.hh"
#include "game/wds/entity/interface/player_reflexes.hh"
#include "game/wds/entity/interface/plr_combat_attack.hh"
#include "game/wds/entity/pendulum.hh"
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
    class polytube;
    class script_object_instance;

    // SM3 eSwingerHand
    enum e_swinger_hand : i32 {
        swinger_none  = -1,
        swinger_right = 0,
        swinger_left  = 1,
        max_swingers  = 2,
        swinger_both  = max_swingers
    };

    // SM3 eSwingerStatus
    enum e_swinger_status : i32 {
        swinger_inactive,
        swinger_animating,
        swinger_launching,
        swinger_attached
    };

    // SM3 ePostProcessAction
    enum e_post_process_action : i32 {
        pp_swing_anchor_search = 0x01,
        pp_swing_avoid_ground  = 0x02
    };

    class swing_leftover;
    class swing_params;

    // SM3 swing_anchor_data; retail adds a point before goal_constraint
    struct swing_anchor_data {
        arch_base_vhandle anchor;
        vector3d          pos;
        vector3d          unk_10;
        f32               goal_constraint;
    };

    // SM3 swing_data, as far as it's read; retail adds a point before goal_constraint
    class swing_data {

    public:
        e_swinger_status status;
        pendulum         attach_pendulum;
        vector3d         unk_54;
        f32              goal_constraint;
    };

    ASSERT_SIZEOF  (swing_anchor_data,                  0x20);
    ASSERT_OFFSETOF(swing_anchor_data, pos,             0x04);
    ASSERT_OFFSETOF(swing_anchor_data, unk_10,          0x10);
    ASSERT_OFFSETOF(swing_anchor_data, goal_constraint, 0x1C);

    ASSERT_OFFSETOF(swing_data, attach_pendulum, 0x04);
    ASSERT_OFFSETOF(swing_data, unk_54,          0x54);
    ASSERT_OFFSETOF(swing_data, goal_constraint, 0x60);

    /* slots 19+ are player_interface's own. SM3's order holds around frame_advance
       (map_controls 89, update_interface 91, frame_advance 92), but WoS inserted slots: SM3 has two getters at 19/20
       and a single post_process where retail has 93 and 94, so those are named from their retail use */
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
        virtual i32               unk_022(f32 unk);
        virtual bool              unk_023(i32 unk);
        virtual void              unk_024(actor* owner);               // set_owner's last step; the rvb builds the hero's master script object
        virtual void*             unk_025(f32 delta_t);
        virtual bool              unk_026();
        virtual bool              unk_027(i32 unk);
        virtual bool              unk_028(i32 unk);
        virtual bool              unk_029();
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
        virtual void*             unk_040(void* unk);
        virtual arch_base_vhandle unk_041(i32 index);                  // a null handle in player_interface
        virtual i32               unk_042();
        virtual void              unk_043(i32 unk);
        virtual bool              unk_044(i32 unk);
        virtual bool              unk_045();
        virtual void              set_zlock_entity(arch_base_vhandle entity);
        virtual void              unk_047(u32 unk);
        virtual arch_base_vhandle unk_048();                           // rvb: the zlock entity
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
        virtual i32               unk_076();
        virtual i32               unk_077();
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
        virtual void              unk_094(f32 delta_t);
        virtual vector3d*         unk_095();                           // its own +0x2F0; the als takes it as a player's velocity
        virtual bool              unk_096(i32 unk);
        virtual bool              unk_097();
        virtual bool              unk_098(i32 unk);
        virtual i32               unk_099();
        virtual i32               unk_100();
        virtual i32               unk_101();
        virtual bool              unk_102();                           // rvb_is_crawling asks 102..106 and 138 after crawling
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
        virtual bool              unk_113();
        virtual bool              unk_114(i32 unk);
        virtual bool              unk_115();
        virtual bool              unk_116(i32 unk);
        virtual bool              interior_transition();
        virtual i32               unk_118();
        virtual bool              unk_119(i32 unk);
        virtual bool              crawling() const;
        virtual i32               unk_121();
        virtual i32               unk_122();
        virtual i32               unk_123();
        virtual bool              unk_124();
        virtual bool              unk_125();
        virtual bool              unk_126();
        virtual bool              swing_any_hand();
        virtual bool              swing_both_hand();
        virtual bool              unk_129(bool unk);                   // rvb: swaps its two +0x638 records
        virtual bool              unk_130();
        virtual bool              unk_131();
        virtual bool              swing_any_hand_locked();
        virtual bool              swing_both_hand_locked();
        virtual bool              unk_134(i32 unk);
        virtual bool              unk_135(i32 unk);
        virtual i32               unk_136();
        virtual i32               unk_137();
        virtual bool              unk_138();
        virtual i32               unk_139();
        virtual void              unk_140();                           // the snap and translate natives call it after a move
        virtual i32               unk_141();
        virtual bool              unk_142(bool unk);
        virtual bool              unk_143();
        virtual void              unk_144();
        virtual void              unk_145(f32 unk);
        virtual void              unk_146(f32 unk);
        virtual void              unk_147(f32 unk);
        virtual f32               unk_148();                           // the als's active_jump_charge
        virtual f32               unk_149();                           // the als's jump_charge
        virtual bool              unk_150();                           // the als's blacksuit_swing_window
        virtual void              unk_151(i32 unk);
        virtual bool              unk_152();                           // the als's trick_window
        virtual bool              unk_153(i32 unk);
        virtual i32               unk_154();                           // the als's max_trick_combo
        virtual i32               unk_155(i32 unk);
        virtual bool              unk_156();
        virtual i32               unk_157();
        virtual i32               unk_158(i32 unk);
        virtual bool              unk_159(i32 unk);
        virtual void*             unk_160(i32 unk);
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
        virtual bool              unk_194(f32 unk);
        virtual i32               unk_195();
        virtual i32               unk_196(i32 unk);
        virtual i32               unk_197();
        virtual void              unk_198();
        virtual i32               unk_199(i32 unk);
        virtual void              unk_200(i32 unk);
        virtual i32               unk_201(f32 unk);
        virtual i32               unk_202();
        virtual void              unk_203();
        virtual i32               unk_204(i32 unk);
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
        virtual arch_base_vhandle unk_244();
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
        virtual arch_base_vhandle unk_258();
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
        virtual bool              unk_305();                           // black mode, judging by the cut scenes and polytubes
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
        virtual void              unk_330(f32 unk, i32 unk_2);
        virtual void              unk_331(f32 unk, i32 unk_2);
        virtual void              unk_332(f32 unk, i32 unk_2);
        virtual i32               unk_333(f32 unk, i32 unk_2);
        virtual void              unk_334(f32 unk, i32 unk_2);
        virtual void              unk_335(f32 unk, i32 unk_2);
        virtual void              unk_336(f32 unk, i32 unk_2);
        virtual i32               unk_337();
        virtual i32               unk_338();
        virtual i32               unk_339();
        virtual bool              unk_340();
        virtual bool              unk_341();
        virtual i32               unk_342();
        virtual i32               unk_343();
        virtual void              unk_344(f32 unk);
        virtual void              unk_345(i32 unk);
        virtual void              unk_346(f32 unk);
        virtual void              unk_347(f32 unk);
        virtual void              unk_348(f32 unk);
        virtual void              unk_349(i32 unk);
        virtual void              unk_350(i32 unk);
        virtual void              unk_351(i32 unk);
        virtual void              unk_352(i32 unk);
        virtual void              unk_353(i32 unk);
        virtual void              unk_354(i32 unk);
        virtual void              unk_355(i32 unk);
        virtual void              unk_356(i32 unk);
        virtual void              unk_357(i32 unk);
        virtual void              unk_358(i32 unk);
        virtual void              unk_359(i32 unk);
        virtual void              unk_360(i32 unk);
        virtual void              unk_361(i32 unk);
        virtual void              unk_362(i32 unk, i32 unk_2, i32 unk_3, i32 unk_4);
        virtual void*             unk_363(void* unk, void* unk_2, void* unk_3, void* unk_4, void* unk_5, void* unk_6);
        virtual void*             unk_364(void* unk, void* unk_2, void* unk_3, void* unk_4, void* unk_5, void* unk_6, void* unk_7);
        virtual i32               unk_365(void* unk, void* unk_2);
        virtual void*             unk_366(void* unk, void* unk_2, void* unk_3, void* unk_4);
        virtual i32               unk_367(void* unk, void* unk_2);
        virtual void              unk_368(f32 unk, swing_data* swinger);
        virtual void              unk_369(i32 unk, i32 unk_2);
        virtual bool              unk_370(i32 unk, i32 unk_2, i32 unk_3);
        virtual i32               unk_371(i32 unk, i32 unk_2);
        virtual entity*           get_last_anchor() const;
        virtual i32               unk_373();
        virtual f32               unk_374();                           // the als's swing_time
        virtual f32               unk_375();
        virtual i32               unk_376();
        virtual bool              unk_377(vector3d* point);            // rvb: the anchor of whichever of its two hands (+0x638) holds one
        virtual bool              unk_378();
        virtual i32               unk_379();
        virtual i32               unk_380(i32 unk);
        virtual f32               unk_381(void* unk, i32 unk_2, f32 unk_3);
        virtual bool              unk_382(i32 unk);
        virtual i32               unk_383();
        virtual bool              unk_384();
        virtual i32               unk_385();
        virtual i32               unk_386(f32 unk, i32 unk_2);
        virtual void*             unk_387();                           // rvb: the pole it swings on (+0x8CC)
        virtual bool              unk_388();                           // rvb: that pole leans more than 45 degrees off vertical
        virtual bool              unk_389(void* unk, f32 unk_2);
        virtual bool              unk_390();
        virtual bool              unk_391(i32 unk, i32 unk_2);
        virtual bool              unk_392(i32 unk);
        virtual bool              unk_393(f32 unk);
        virtual i32               unk_394();
        virtual i32               unk_395();
        virtual i32               unk_396(i32 unk);
        virtual i32               unk_397();
        virtual i32               unk_398(i32 unk);
        virtual swing_data*       unk_399(i32 index);                  // rvb: its swinger at the index
        virtual void              unk_400(polytube* tube, f32 radius); // dresses the polytube as the hero's web
        virtual i32               unk_401();                           // the als's swing_hand
        virtual i32               unk_402(i32 unk, i32 unk_2);
        virtual f32               unk_403();                           // the als's swing_level_rating
        virtual f32               unk_404(f32 unk);
        virtual f32               unk_405(f32 unk);
        virtual f32               unk_406(f32 unk);
        virtual f32               unk_407(f32 unk);
        virtual void              unk_408(i32 unk, i32 unk_2);
        virtual void              unk_409(i32 unk, i32 unk_2, i32 unk_3);
        virtual void              unk_410(i32 unk, i32 unk_2, i32 unk_3, i32 unk_4);
        virtual i32               unk_411(i32 unk, i32 unk_2);
        virtual void              unk_412(i32 unk, i32 unk_2, i32 unk_3, i32 unk_4, i32 unk_5, i32 unk_6);
        virtual i32               unk_413(i32 unk, i32 unk_2, void* unk_3);
        virtual f32               unk_414();
        virtual f32               unk_415();
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
        virtual f32               unk_432();                           // the als's swing_entry_speed
        virtual f32               unk_433();                           // the als's swing_exit_speed
        virtual f32               unk_434();
        virtual f32               unk_435();
        virtual f32               unk_436();
        virtual f32               unk_437();
        virtual f32               unk_438();
        virtual void              unk_439(i32 unk);
        virtual f32               unk_440();
        virtual f32               unk_441();
        virtual f32               unk_442();
        virtual f32               unk_443();
        virtual f32               unk_444();
        virtual f32               unk_445();
        virtual f32               unk_446();
        virtual f32               unk_447();
        virtual f32               unk_448();
        virtual f32               unk_449();
        virtual f32               unk_450();
        virtual f32               unk_451();
        virtual f32               unk_452();
        virtual i32               unk_453(i32 unk, i32 unk_2, i32 unk_3);
        virtual f32               unk_454();
        virtual i32               unk_455(i32 unk);
        virtual bool              unk_456();
        virtual bool              unk_457(i32 unk);
        virtual void              unk_458(void* unk, void* unk_2, f32 unk_3);
        virtual i32               unk_459();
        virtual i32               unk_460(f32 unk);
        virtual bool              unk_461();
        virtual bool              unk_462();
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
        virtual bool              unk_478(i32 unk);
        virtual bool              unk_479();
        virtual bool              unk_480(bool unk);
        virtual bool              unk_481();
        virtual bool              unk_482(bool unk);
        virtual arch_base_vhandle unk_483(i32 index);                  // a null handle in player_interface
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
        virtual bool              unk_515();
        virtual i32               unk_516(i32 unk, void* unk_2, bool unk_3);
        virtual i32               unk_517();
        virtual bool              unk_518(i32 unk);
        virtual bool              unk_519();
        virtual bool              unk_520();
        virtual void              unk_521();
        virtual i32               unk_522();
        virtual void              unk_523(bool unk, bool unk_2);
        virtual bool              unk_524();
        virtual i32               unk_525(i32 unk, i32 unk_2);
        virtual i32               unk_526();
        virtual void              unk_527(f32 unk);
        virtual line_info*        unk_528();                           // rvb: a line_info of its own (+0xBA8)
        virtual void*             unk_529(i32 unk);
        virtual bool              unk_530(f32 unk);
        virtual bool              unk_531(i32 unk);
        virtual bool              unk_532(const line_info* unk);       // rvb: null takes that line_info
        virtual i32               unk_533(i32 unk);
        virtual void              unk_534(i32 unk, f32 unk_2);
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

        game_button* get_command_button(i32 cmd) {
            return &command_buttons[cmd];
        }

        void clear_buttons();
        void clear_controller(bool unk);
        void override_button(i32 ctrl, f32 value, f32 t);
        void override_command(i32 cmd, f32 value, f32 t);
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
