#pragma once

#include "game/wds/entity/interface/player_interface.hh"
#include "game/wds/entity/interface/swing_data.hh"
#include "game/wds/ai/ai_strength_test_info.hh"
#include "game/arch_base_vhandle.hh"
#include "game/delta_variable.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "game/math/algebra/vector3d.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    struct region;

    namespace references {
        // development_option<bool> RPGTuning/RELOAD_CURRENT_UPGRADE_SETTINGS
        inline util::memory_reference<u8> reload_current_upgrade_settings { 0x01112D0C };

        // what frame_advance last reloaded the upgrade settings with; 1 from the start
        inline util::memory_reference<i32> unk_00f4db94 { 0x00F4DB94 };

        // .data 1; frame_advance takes the velocity from slot 95 while it's set, else from the physical interface
        inline util::memory_reference<u8> unk_00f4de2c { 0x00F4DE2C };

        // set to 0 every hero frame
        inline util::memory_reference<u8> unk_01112c58 { 0x01112C58 };

        // "frame_advance(num)", the master object's function frame_advance calls
        inline util::memory_reference<string_hash> frame_advance_num { 0x01112D5C };

        // "damage", the combo move param check_for_block takes as the block value
        inline util::memory_reference<string_hash> damage { 0x00FC1A44 };

        // "wall_explode_recovery", the als base layer param slot 484 tests
        inline util::memory_reference<string_hash> wall_explode_recovery { 0x01112CAC };

        // the web power upgrades slot 21 scales damage by
        inline util::memory_reference<string_hash> web_upgrade_power_3 { 0x01112CB4 };
        inline util::memory_reference<string_hash> web_upgrade_power_2 { 0x0111311C };
        inline util::memory_reference<string_hash> web_upgrade_power_1 { 0x01112DB4 };

        // two of the {3.75, 0.0035} pairs at 0x00F4D590, rewritten every hero frame
        inline util::memory_reference<f32> unk_00f4d590 { 0x00F4D590 };
        inline util::memory_reference<f32> unk_00f4d594 { 0x00F4D594 };
        inline util::memory_reference<f32> unk_00f4d5a0 { 0x00F4D5A0 };
        inline util::memory_reference<f32> unk_00f4d5a4 { 0x00F4D5A4 };
    } // references

    // the swing tune's level-scaled values
    inline f32 linear_formula(f32 base, f32 rate, f32 x) {
        return base + rate * x;
    }

    // the hero's player interface
    class spiderman_rvb_player_interface : public player_interface {

    public:
        // a point, an entity or a point in an entity's space
        struct unk_068c_t {
            i32               kind;     // 1 the point, 2 the entity, 3 the point in the entity's space
            i32               unk_04;
            vector3d          position;
            arch_base_vhandle handle;
        };

        bool                     rvb_black_mode;                  // the als's player_in_blacksuit_mode
        bool                     suit_locked;
        bool                     suit_mode_locked;
        u8                       pad_533[0x01];
        i32                      unk_0534;                        // slot 525 copies the mode here
        bool                     unk_0538;
        bool                     unk_0539;
        u8                       pad_53a[0x02];
        f32                      unk_053c;
        f32                      unk_0540;
        u8                       reserved_0544[0x08];
        u8                       web_shooters[0x28];              // a player_web_shooters
        u32                      trick_begin_callback_id;
        u32                      trick_end_callback_id;
        f32                      unk_057c;                        // counted down, held at 0 in mode 8
        u8                       reserved_0580[0x04];
        f32                      jump_charge_time;
        f32                      jump_min_charge_time;
        f32                      jump_max_charge_time;
        f32                      unk_0590;                        // PLR_SWEET_MANUAL_DODGE_TIME
        f32                      stored_als_jump_charge;
        delta_variable<i32>      forced_jump_type;                // set to 42 every frame
        u8                       heading_sampling_window[0x48];   // a direction_sampling_window
        u8                       up_sampling_window[0x48];
        f32                      cur_lateral_velocity;            // the velocity's length in x and z
        swing_params*            swing;
        swing_data*              swingers[max_swingers];
        e_swinger_hand           active_swinger;
        e_swinger_hand           considering_swinger;
        swing_leftover*          swing_leftovers[8];
        i32                      last_swing_leftover;
        f32                      time_on_web;
        f32                      unk_0670;
        bool                     unk_0674;                        // consumed by frame_advance, which then times it in unk_0678
        u8                       pad_675[0x03];
        f32                      unk_0678;
        f32                      unk_067c;
        u8                       vertical_line_anchor_tune[0x0C]; // a vertical_line_anchor_tune_t
        unk_068c_t               unk_068c;
        unk_068c_t               unk_06a4;
        vector3d                 unk_06bc;                        // the owner's position as frame_advance left it
        unk_068c_t               unk_06c8;
        arch_base_vhandle        swing_override_anchor;
        arch_base_vhandle        zip_override_anchor;
        u32                      unk_06e8;                        // the timed target
        f32                      unk_06ec;
        arch_base_vhandle        unk_06f0;
        u8                       reserved_06f4[0x0A];
        bool                     unk_06fe;                        // the zlock control
        u8                       reserved_06ff[0x05];
        f32                      unk_0704;
        f32                      unk_0708;
        f32                      unk_070c;
        u8                       reserved_0710[0x0C];
        arch_base_vhandle        unk_071c;                        // the zlock target
        u8                       reserved_0720[0x04];
        arch_base_vhandle        unk_0724;                        // slot 40 answers it
        arch_base_vhandle        unk_0728;
        arch_base_vhandle        unk_072c[2];
        f32                      zip_override_anchor_num;
        u8                       reserved_0738[0x0C];
        vector3d                 unk_0744;
        arch_base_vhandle        unk_0750;
        arch_base_vhandle        unk_0754;
        u32                      unk_0758;
        bool                     unk_075c;
        u8                       pad_75d[0x03];
        arch_base_vhandle        unk_0760;                        // check_for_block's attacker
        arch_base_vhandle        unk_0764;                        // and weapon entity
        i32                      unk_0768;                        // the react level
        f32                      unk_076c;
        f32                      unk_0770;                        // the block value, summed while unk_077c is set
        f32                      unk_0774;
        f32                      unk_0778;
        bool                     unk_077c;
        bool                     unk_077d;
        bool                     unk_077e;
        u8                       pad_77f[0x01];
        arch_base_vhandle        unk_0780;                        // the attacker again while unk_077e is set
        u8                       reserved_0784[0x2C];
        u32                      unk_07b0;
        u32                      unk_07b4;
        u32                      unk_07b8;
        u8                       reserved_07bc[0x0C];
        u32                      unk_07c8;
        u32                      unk_07cc;
        u8                       reserved_07d0[0x10];
        f32                      unk_07e0;
        f32                      unk_07e4;
        f32                      unk_07e8;
        i32                      unk_07ec;
        i32                      unk_07f0;
        u8                       reserved_07f4[0x08];
        bool                     unk_07fc;
        u8                       pad_7fd[0x03];
        f32                      unk_0800;
        vector3d                 unk_0804;
        bool                     unk_0810[7];
        u8                       pad_817[0x01];
        vc8::vector
            <arch_base_vhandle>* unk_0818;
        u8                       reserved_081c[0x04];
        f32                      zipjump_ground_angle;
        f32                      zipjump_air_angle;
        f32                      zipjump_length;
        f32                      unk_082c;                        // PLR_ZIPJUMP_BOOST_FORCE
        f32                      unk_0830;                        // the seven timers frame_advance counts down
        f32                      unk_0834;
        f32                      unk_0838;
        f32                      unk_083c;
        f32                      unk_0840;
        f32                      unk_0844;                        // unk_0840's starting duration
        f32                      unk_0848;
        f32                      unk_084c;
        bool                     unk_0850;                        // force_evade's request
        u8                       pad_851[0x03];
        vector3d                 unk_0854;
        bool                     unk_0860;
        bool                     unk_0861;
        bool                     unk_0862;
        u8                       reserved_0863[0x0D];
        line_info                desired_zip_or_yoyo_data;
        arch_base_vhandle        unk_08cc[2];                     // the pole the hero swings on and its parent
        u8                       wallsprint_tune[0x80];           // a wallsprint_tune_t
        line_info                wall_sprint_surface_info;
        u8                       reserved_09b0[0x64];
        boolx                    _exterior_transition;
        boolx                    _exterior_transition_180;
        u8                       reserved_0a16[0x06];
        i32                      unk_0a1c;                        // set to 106 every frame
        u8                       reserved_0a20[0x04];
        strength_test_info       my_strength_test_info;
        u8                       my_slingshot_point_info[0x24];
        u8                       my_agility_test_info[0x60];
        u8                       reserved_0ac8[0x04];
        u8                       skydiving_tune[0x88];            // a skydiving_tune_t
        f32                      unk_0b54;                        // slot 486 runs when it runs out
        f32                      unk_0b58;                        // slot 488 runs when it runs out
        bool                     unk_0b5c;
        bool                     unk_0b5d;
        u8                       reserved_0b5e[0x0A];
        f32                      unk_0b68;                        // force_run_speed_pct
        u8                       reserved_0b6c[0x38];
        u32                      unk_0ba4;                        // flags; slots 306 and 307 read bits 0 and 1
        line_info                unk_0ba8;
        f32                      unk_0c04;                        // the obstacle hop's hold time
        u8                       reserved_0c08[0x04];
        i32                      unk_0c0c;
        arch_base_vhandle        unk_0c10;                        // slot 534 keeps it out of camera collision until unk_0c14 runs out
        f32                      unk_0c14;
        arch_base_vhandle        unk_0c18;
        f32                      unk_0c1c;
        u8                       reserved_0c20[0x38];
        f32                      unk_0c58;
        f32                      unk_0c5c;
        f32                      unk_0c60[4];
        vector3d                 unk_0c70[4];
        f32                      noone_attack_me_time;            // resets the combat target capacities when it runs out
        f32                      fall_timer;                      // how long the owner has been below y -8 without standing
        mash::array_replacement_base
            <swing_anchor_data>  desired_swing_anchor_data;
        u8                       reserved_0cac[0x04];

        explicit spiderman_rvb_player_interface(mash::generating_vtable_version so_do_nothing) : player_interface(so_do_nothing),
                                                                                                  desired_zip_or_yoyo_data(so_do_nothing),
                                                                                                  wall_sprint_surface_info(so_do_nothing),
                                                                                                  unk_0ba8(so_do_nothing) {}

        ~spiderman_rvb_player_interface() override;

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool get_ifc_num(const string_hash &name, f32* value, bool document) override;
        bool set_ifc_num(const string_hash &name, f32 value, bool document) override;
        bool get_ifc_vec(const string_hash &name, vector3d* value, bool document) override;

        void set_owner(actor* owner) override;

        f32               unk_021() override;
        void              set_rpg_progress(f32 progress) override;
        bool              unk_023(i32 unk) override;
        void              unk_024(actor* owner) override;
        void*             unk_025(f32 delta_t) override;
        bool              unk_026() override;
        bool              unk_027(i32 unk) override;
        bool              unk_028(i32 unk) override;
        bool              unk_029() const override;
        f32               unk_030() override;
        i32               unk_031(i32 unk) override;
        i32               unk_032(i32 unk) override;
        i32               unk_034(i32 unk) override;
        i32               unk_035(i32 unk) override;
        void*             unk_036(void* unk) override;
        void              set_zip_override_anchor_num(f32 num) override;
        f32               unk_038() override;
        i32               unk_039(i32 unk) override;
        arch_base_vhandle unk_040() const override;
        arch_base_vhandle unk_041(i32 index) override;
        i32               unk_042() override;
        void              unk_043(i32 unk) override;
        bool              unk_044(i32 unk) override;
        bool              unk_045() override;
        void              set_zlock_entity(arch_base_vhandle entity) override;
        void              unk_047(u32 unk) override;
        arch_base_vhandle unk_048() const override;
        bool              unk_049(i32 unk) override;
        i32               unk_050(arch_base_vhandle target) override;
        i32               unk_051(i32 unk, i32 unk_2, i32 unk_3) override;
        void*             unk_052(void* unk) override;
        void*             unk_053(void* unk, void* unk_2) override;
        void              unk_054(arch_base_vhandle target, u32 kind) override;
        arch_base_vhandle unk_055() override;
        f32               unk_056() override;
        i32               unk_057() override;
        i32               unk_060(arch_base_vhandle target) override;
        arch_base_vhandle unk_061() override;
        void*             unk_062(void* unk) override;
        i32               unk_063(i32 unk) override;
        i32               unk_064(i32 unk) override;
        void*             unk_065(void* unk) override;
        void              unk_066(i32 unk, i32 unk_2, i32 unk_3) override;
        i32               unk_067(i32 unk) override;
        bool              unk_068(i32 unk) override;
        bool              unk_069() override;
        i32               unk_070(i32 unk) override;
        void*             unk_071(void* unk) override;
        bool              unk_072() override;
        bool              unk_074() override;
        i32               unk_076() const override;
        i32               unk_077() const override;
        void              set_swing_override_anchor(arch_base_vhandle anchor) override;
        void              set_zip_override_anchor(arch_base_vhandle anchor) override;
        arch_base_vhandle unk_080() override;
        void              unk_081(f32 unk) override;
        i32               unk_082(f32 unk) override;
        f32               get_movement_speed() const override;
        void              unk_086(i32 unk) override;
        bool              unk_087() override;
        bool              unk_088(i32 unk) override;
        bool              map_controls(i32 config, bool force) override;
        void              render_phase() override;
        void              update_interface() override;
        void              frame_advance(f32 delta_t) override;
        void              post_process(f32 delta_t) override;
        bool              unk_096(i32 unk) override;
        bool              unk_097() override;
        bool              unk_098(i32 unk) override;
        bool              is_walking() const override;
        i32               unk_100() override;
        i32               unk_101() override;
        i32               unk_107() override;
        i32               unk_108(i32 unk) override;
        bool              exterior_transition() override;
        bool              unk_114(i32 unk) override;
        bool              exterior_transition_180() override;
        bool              unk_116(i32 unk) override;
        i32               unk_118() override;
        bool              unk_119(i32 unk) override;
        i32               unk_121() override;
        bool              swing_r_hand() const override;
        bool              swing_l_hand() const override;
        bool              swap_swing_hands(bool refire) override;
        bool              swing_r_hand_locked() const override;
        bool              swing_l_hand_locked() const override;
        void              lock_swing_r_hand(bool l) override;
        void              lock_swing_l_hand(bool l) override;
        i32               unk_136() override;
        bool              on_ground() const override;
        bool              on_ceiling() const override;
        i32               unk_141() override;
        void              inc_jump_charge(f32 delta_t) override;
        void              set_abs_jump_charge(f32 fraction) override;
        void              clear_jump_charge(bool store_for_als) override;
        f32               get_jump_charge() const override;
        f32               get_als_jump_charge() const override;
        bool              unk_152() override;
        bool              unk_153(i32 unk) override;
        bool              unk_156() override;
        i32               predict_jump_type() override;
        i32               unk_158(i32 unk) override;
        void              unk_163() override;
        void              unk_164() override;
        i32               unk_165() override;
        i32               unk_166() override;
        bool              unk_167(i32 unk) override;
        bool              unk_168() override;
        bool              unk_169(i32 unk) override;
        bool              unk_170() override;
        bool              unk_171(i32 unk) override;
        bool              unk_172() override;
        bool              unk_173(i32 unk) override;
        bool              unk_174() override;
        bool              unk_175(i32 unk) override;
        bool              unk_176() override;
        bool              unk_177(i32 unk) override;
        bool              unk_178() override;
        bool              unk_179(i32 unk) override;
        bool              unk_180() override;
        bool              unk_188() override;
        bool              unk_192() override;
        bool              unk_193(bool unk) override;
        bool              handle_jump_charge(f32 delta_t) override;
        i32               unk_195() override;
        vector3d          unk_196() override;
        i32               unk_197() override;
        void              unk_198() override;
        i32               unk_201(f32 unk) override;
        i32               unk_202() override;
        void              unk_203() override;
        vector3d          unk_204() override;
        void*             unk_205(void* unk) override;
        i32               unk_206(f32 unk, entity* unk_2, i32 unk_3) override;
        void              unk_207() override;
        void              unk_208(i32 unk) override;
        void              unk_209(i32 unk) override;
        f32               unk_210() override;
        i32               unk_215() override;
        void              init_attacks() override;
        void              update_attacks(f32 delta_t) override;
        void              init_tricks() override;
        bool              check_for_block(      entity_base*               attacker,
                                                i32                        chain_size,
                                          const combo_move_data*           combo_move,
                                                hit_react_record*          react,
                                                weapon_interface_extender* weapon,
                                                entity_base*               weapon_entity,
                                                i32                        usage_type) override;
        arch_base_vhandle unk_243() override;
        arch_base_vhandle unk_244() const override;
        i32               unk_245() override;
        f32               unk_246() override;
        f32               unk_247() override;
        bool              unk_248() override;
        void              unk_249(bool unk) override;
        f32               unk_250() override;
        f32               get_block_strength() override;
        void              unk_252(f32 unk) override;
        f32               unk_253() override;
        void              unk_254(f32 unk) override;
        void              unk_255(f32 delta_t) override;
        void              unk_256() override;
        bool              unk_257() override;
        arch_base_vhandle unk_258() const override;
        i32               unk_262(i32 unk, i32 unk_2) override;
        bool              unk_263(void* unk, i32 unk_2) override;
        bool              unk_264(f32 delta_t, void* unk_2, i32 unk_3, i32 unk_4, i32 unk_5) override;
        bool              unk_265() override;
        void              force_evade(vector3d v) override;
        void              unk_267() override;
        bool              unk_268() override;
        vector3d          unk_269() override;
        bool              unk_270() override;
        bool              unk_271() override;
        bool              unk_272() override;
        entity*           unk_273(entity* target) override;
        bool              unk_284() override;
        bool              is_in_rvb_black_mode() const override;
        i32               unk_306() override;
        i32               unk_307() override;
        f32               unk_308() override;
        f32               unk_310() override;
        f32               unk_315() override;
        bool              unk_319(f32 delta_t, game_button* button, i32 unk_3) override;
        void*             unk_320() override;
        void*             unk_321(const line_info* unk) override;
        bool              unk_322(i32 unk) override;
        void*             unk_323(void* unk) override;
        i32               unk_324(i32 unk) override;
        f32               unk_325() override;
        bool              unk_326() override;
        bool              unk_327(i32 unk) override;
        void              unk_330(f32 duration, bool force) override;
        void              unk_331(f32 duration, bool force) override;
        void              unk_332(f32 duration, bool force) override;
        void              unk_333(f32 duration, bool force) override;
        void              unk_334(f32 duration, bool force) override;
        void              unk_335(f32 duration, bool force) override;
        void              unk_336(f32 duration, bool force) override;
        i32               unk_337() override;
        i32               unk_338() override;
        i32               unk_339() override;
        bool              unk_340() override;
        bool              unk_341() override;
        i32               unk_342() override;
        i32               unk_343() override;
        void              unk_344(f32 unk) override;
        void              unk_345(i32 unk) override;
        void              unk_346(f32 unk) override;
        void              unk_347(f32 unk) override;
        void              unk_348(f32 unk) override;
        void              unk_349(i32 unk) override;
        void              unk_350(i32 unk) override;
        void              unk_351(i32 unk) override;
        void              unk_352(i32 unk) override;
        void              unk_353(i32 unk) override;
        void              unk_354(i32 unk) override;
        void              set_max_dynamic_sweet_angle(f32 angle) override;
        void              set_swing_ground_clearance(f32 clearance) override;
        void              unk_357(f32 rate) override;
        void              unk_358(f32 rate) override;
        void              unk_359(f32 value) override;
        void              unk_360(f32 value) override;
        void              unk_361(f32 value) override;
        void              get_sweet_spot_params(f32* incline_sin,
                                                f32* incline_cos,
                                                f32* sweet_cone_cos,
                                                f32* distance,
                                                f32* min_len_squared,
                                                f32* max_len_squared) override;
        void              get_dynamic_sweet_spot_params(f32* min_length,
                                                        f32* max_length,
                                                        f32* overdrive_length,
                                                        f32* overdrive_velocity,
                                                        f32* max_no_anchor_length,
                                                        f32* min_angle,
                                                        f32* max_angle) override;
        void              get_blacksuit_swing_params(f32* swing_velocity_mod, f32* to_anchor_force) override;
        void              get_reel_in_rate(f32* min_rate, f32* max_rate, f32* max_time, f32* max_range) override;
        void              get_fake_anchor_params(f32* max_time, f32* pull_rate) override;
        void              unk_368(f32 unk, swing_data* swinger) override;
        void              unk_369(i32 unk, i32 unk_2) override;
        bool              break_web(e_swinger_hand web_to_break, bool add_momentum, bool add_dangler) override;
        i32               unk_371(i32 unk, i32 unk_2) override;
        i32               unk_373() override;
        f32               unk_374() override;
        f32               unk_375() override;
        i32               unk_376() override;
        bool              unk_377(vector3d* point) override;
        bool              unk_378() override;
        i32               unk_379() override;
        i32               unk_380(i32 unk) override;
        f32               unk_381(void* unk, i32 unk_2, f32 unk_3) override;
        bool              unk_382(i32 unk) override;
        i32               unk_383() override;
        bool              unk_384() override;
        i32               unk_385() override;
        bool              unk_386(f32 unk, bool unk_2) override;
        void*             unk_387() override;
        bool              unk_388() override;
        bool              unk_389(void* unk, f32 unk_2) override;
        bool              unk_390() override;
        bool              unk_391(i32 unk, i32 unk_2) override;
        bool              unk_392(bool skip) override;
        bool              unk_393(f32 unk) override;
        i32               unk_394() override;
        i32               unk_395() override;
        i32               unk_396(i32 unk) override;
        i32               unk_398(i32 unk) override;
        swing_data*       get_swinger(e_swinger_hand i) override;
        void              unk_400(polytube* tube, f32 radius) override;
        i32               get_als_swing_hand_value() override;
        i32               unk_402(i32 unk, i32 unk_2) override;
        f32               get_awesome_rating() const override;
        f32               unk_404(f32 progress) override;
        f32               unk_405(f32 progress) override;
        f32               unk_406(f32 progress) override;
        f32               unk_407(f32 progress) override;
        void              unk_408(i32 unk, i32 unk_2) override;
        void              unk_409(i32 unk, i32 unk_2, i32 unk_3) override;
        void              unk_410(i32 unk, i32 unk_2, i32 unk_3, i32 unk_4) override;
        i32               unk_411(i32 unk, i32 unk_2) override;
        void              unk_412(i32 unk, i32 unk_2, i32 unk_3, i32 unk_4, i32 unk_5, i32 unk_6) override;
        i32               unk_413(i32 unk, i32 unk_2, void* unk_3) override;
        f32               get_swing_collision_break_length() const override;
        f32               get_swing_collision_stick_length() const override;
        f32               unk_416() override;
        i32               unk_417() override;
        i32               unk_418() override;
        i32               unk_419() override;
        i32               unk_420() override;
        i32               unk_421() override;
        i32               unk_422() override;
        i32               unk_423() override;
        f32               get_als_swing_angle() override;
        f32               get_als_swing_side_angle() override;
        f32               get_als_swing_to_anchor_angle() override;
        f32               get_als_swing_entry_speed() override;
        f32               get_als_swing_exit_speed() override;
        f32               get_als_lateral_speed() override;
        f32               unk_438() override;
        void              unk_439(i32 unk) override;
        f32               get_swing_boost_cooldown_time() const override;
        f32               get_min_swing_boost_angle() const override;
        f32               get_max_swing_boost_angle() const override;
        f32               get_swing_boost_force() const override;
        f32               unk_444() override;
        f32               get_swing_jump_boost() const override;
        f32               get_swing_drag_coefficient() const override;
        f32               get_swing_drag_coefficient_min_speed() const override;
        f32               get_swing_drag_coefficient_horz_scale() const override;
        f32               get_swing_drag_coefficient_up_scale() const override;
        f32               get_swing_drag_coefficient_down_scale() const override;
        f32               get_swing_ground_clearance() const override;
        f32               unk_452() override;
        void              set_new_swing_record_keeping(f32 swing_entry_speed,
                                                       f32 to_anchor_angle,
                                                       f32 to_anchor_length) override;
        f32               get_swing_max_dot_for_velocity_preservation() const override;
        void              set_swing_max_dot_for_velocity_preservation(f32 d) override;
        bool              get_swing_avoid_ground() const override;
        void              set_swing_avoid_ground(bool b) override;
        void              unk_458(void* unk, void* unk_2, f32 unk_3) override;
        i32               unk_459() override;
        i32               unk_460(f32 unk) override;
        bool              should_do_strength_test() const override;
        bool              is_a_scripted_strength_test() const override;
        bool              unk_463() override;
        i32               unk_464() override;
        void              unk_465() override;
        bool              unk_466() override;
        void*             unk_467() override;
        void              unk_468() override;
        bool              unk_469() override;
        bool              unk_470() override;
        i32               unk_471() override;
        void              unk_472() override;
        arch_base_vhandle unk_483(const plr_combat_attack* attack) const override;
        bool              unk_484() override;
        bool              unk_485() override;
        bool              unk_486(bool unk, f32 delay) override;
        bool              unk_487() override;
        bool              unk_488(bool unk, f32 delay) override;
        i32               unk_489() override;
        i32               unk_490() override;
        f32               unk_491() override;
        void              unk_492(bool start) override;
        i32               unk_493() override;
        i32               unk_494() override;
        i32               is_in_webcast_approach_normal_hit_range() override;
        i32               is_in_webcast_approach_critical_hit_range() override;
        i32               is_in_approach_counter() override;
        i32               is_in_webcast_approach_early_range() override;
        i32               is_in_webcast_approach_counter_range() override;
        i32               unk_500() override;
        i32               unk_501() override;
        f32               approach_norm_to_crit_percent() override;
        i32               unk_503() override;
        bool              is_webcasting_entity(arch_base_vhandle entity) override;
        void              unk_506(bool start) override;
        const vector3d*   unk_507(const vector3d* v) override;
        vector3d          unk_508() const override;
        i32               unk_509() override;
        void              unk_510(bool start) override;
        bool              unk_511() override;
        bool              unk_512(bool unk) override;
        bool              unk_513(i32 unk) override;
        void*             unk_514(i32 unk) override;
        bool              is_in_wallmode() override;
        i32               unk_517() override;
        bool              unk_518(i32 unk) override;
        bool              unk_519() override;
        bool              unk_520() override;
        i32               unk_522() override;
        void              lock_suit(bool lock, bool black) override;
        bool              unk_524() override;
        void              mode_lock_suit(bool lock, f32 time) override;
        void              unk_526() override;
        void              unk_527(f32 delta_t) override;
        line_info*        unk_528() override;
        void              unk_529(const line_info* line) override;
        bool              unk_530(f32 delta_t) override;
        bool              unk_531(const line_info* line) override;
        bool              unk_532(const line_info* unk) override;
        void              unk_534(arch_base_vhandle target, f32 time) override;

        virtual bool     unk_536(i32 unk);
        virtual bool     unk_537(void* unk);
        virtual bool     unk_538(i32 unk);
        virtual f32      unk_539();
        virtual void     unk_540(f32 delta_t);         // the cut scene player's time step while it plays
        virtual f32      unk_541(i32 unk);
        virtual vector3d unk_542(i32 index);
        virtual bool     unk_543();
        virtual i32      unk_544(f32 delta_t);
        virtual bool     unk_545();
        virtual i32      unk_546(i32 unk);
        virtual bool     unk_547();
        virtual f32      unk_548(f32 t);
        virtual f32      unk_549(f32 t);
        virtual void     unk_550(arch_base_vhandle* out, i32 count);
        virtual void*    unk_551(void* unk);
        virtual i32      unk_552(void* unk, i32 unk_2);
        virtual void     unk_553(arch_base_vhandle old_target, arch_base_vhandle new_target, arch_base_vhandle other_target);
        virtual f32      unk_554();
        virtual void     unk_555(f32 unk);
        virtual void*    unk_556(void* unk);
        virtual void*    unk_557(i32 unk);
        virtual void     unk_558(f32 unk);
        virtual void     unk_559(f32 delta_t);
        virtual void*    unk_560(f32 unk);
        virtual i32      unk_561(game_button* button, i32* state);
        virtual f32      unk_562(arch_base_vhandle target);
        virtual f32      unk_563(f32 level, f32 target_level);
        virtual i32      unk_564(f32 unk);
        virtual void     unk_565(f32 delta_t);
        virtual void     unk_566(i32 unk);
        virtual void*    unk_567(f32 unk);
        virtual void     unk_568();
        virtual void     unk_569();
        virtual bool     unk_570(i32 unk, void* unk_2, i32 unk_3, i32 unk_4, f32 unk_5);
        virtual i32      unk_571(i32 unk);
        virtual void     unk_572(f32 unk, bool black); // switches the suit's behaviours
        virtual i32      unk_573();
        virtual bool     unk_574();
        virtual bool     unk_575();
        virtual bool     unk_576();
        virtual i32      unk_577();
        virtual bool     unk_578();
        virtual i32      unk_579(void* unk, void* unk_2, void* unk_3, i32 unk_4, i32 unk_5);
        virtual void     unk_580(f32 delta_t);
        virtual bool     unk_581(i32 unk, i32 unk_2);
        virtual i32      unk_582(f32 unk);
        virtual i32      is_in_webcast_approach();
        virtual f32      unk_584();
        virtual void     unk_585();
        virtual void*    unk_586(void* unk);
        virtual i32      unk_587(i32 unk, i32 unk_2);
        virtual i32      unk_588(void* unk, i32 unk_2);
        virtual i32      unk_589(i32 unk, f32 unk_2);
        virtual void*    unk_590(void* unk);
        virtual i32      unk_591(i32 unk, i32 unk_2);
        virtual i32      unk_592(void* unk, i32 unk_2);
        virtual void     unk_593(void* unk, i32 unk_2, f32 unk_3);
        virtual i32      unk_594(void* unk, i32 unk_2);
        virtual i32      unk_595(f32 unk);

        static f32 compute_ground_level_at_sweet_spot_position(const vector3d &position, f32 max_web_length, const region* reg); // straight down from the position

        static constexpr f32 timed_target_times[4] = { 3.0f, 0.5f, 3.0f, 5.0f }; // by kind

        static string_hash my_type_key;
        static bool        mario_jump_pressed; // a mario jump's press waits here for its release
    };

    ASSERT_SIZEOF  (spiderman_rvb_player_interface::unk_068c_t,           0x18);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface::unk_068c_t, position, 0x08);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface::unk_068c_t, handle,   0x14);

    ASSERT_SIZEOF  (spiderman_rvb_player_interface,                            0xCB0);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, rvb_black_mode,            0x530);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0534,                  0x534);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0540,                  0x540);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, web_shooters,              0x54C);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, trick_begin_callback_id,   0x574);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, jump_charge_time,          0x584);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, forced_jump_type,          0x598);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, heading_sampling_window,   0x5A0);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, cur_lateral_velocity,      0x630);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, swingers,                  0x638);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, swing_leftovers,           0x648);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0674,                  0x674);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, vertical_line_anchor_tune, 0x680);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_068c,                  0x68C);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_06bc,                  0x6BC);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_06c8,                  0x6C8);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, swing_override_anchor,     0x6E0);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_06fe,                  0x6FE);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_071c,                  0x71C);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, zip_override_anchor_num,   0x734);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0744,                  0x744);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0760,                  0x760);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_077c,                  0x77C);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0780,                  0x780);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_07b0,                  0x7B0);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_07c8,                  0x7C8);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_07e0,                  0x7E0);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_07fc,                  0x7FC);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0810,                  0x810);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0818,                  0x818);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, zipjump_ground_angle,      0x820);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0830,                  0x830);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0850,                  0x850);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0854,                  0x854);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, desired_zip_or_yoyo_data,  0x870);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_08cc,                  0x8CC);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, wallsprint_tune,           0x8D4);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, wall_sprint_surface_info,  0x954);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, _exterior_transition,      0xA14);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0a1c,                  0xA1C);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, my_strength_test_info,     0xA24);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, my_slingshot_point_info,   0xA44);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, my_agility_test_info,      0xA68);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, skydiving_tune,            0xACC);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0b54,                  0xB54);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0b68,                  0xB68);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0ba4,                  0xBA4);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0ba8,                  0xBA8);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0c04,                  0xC04);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0c0c,                  0xC0C);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0c10,                  0xC10);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0c18,                  0xC18);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0c58,                  0xC58);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, unk_0c70,                  0xC70);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, noone_attack_me_time,      0xCA0);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, fall_timer,                0xCA4);
    ASSERT_OFFSETOF(spiderman_rvb_player_interface, desired_swing_anchor_data, 0xCA8);
} // treyarch
