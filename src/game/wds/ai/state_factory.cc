#include "retail.hh"
#include "game/wds/ai/ai_state_machine.hh"
#include "treyarch/mm/mm.hh"

using namespace treyarch;

// inlined @ sub_4D70B0
template<typename F>
base_state* base_state::construct(u32 size, F constructor, void* vtable) {
    void* memory = mm::allocate(size);

    if (!memory)
        return nullptr;

    auto* state = ((base_state* (__thiscall*)(void*))constructor)(memory);

    if (vtable)
        *(void**)state = vtable;

    return state;
}

// inlined @ sub_4D70B0
enhanced_state* enhanced_state::construct_inline(u32 size, void* vtable) {
    auto* state = (enhanced_state*)mm::allocate(size);

    if (!state)
        return nullptr;

    state->base_state::initialize(mash::ALLOCATED);
    state->initialize(mash::ALLOCATED);

    *(void**)state = vtable;

    return state;
}

// sub_4D70B0
base_state* base_state::create_subclass(mash::virtual_types_key the_class_to_make) {
    switch (the_class_to_make) {
        case 0xC1AF29E5: // plr_wall_blocking_state
            return construct(0x80, retail::sub_580DB0);
        case 0xAB87938C: // plr_loco_obstacle_hop_state
            return construct(0xA4, retail::sub_580840);
        case 0x2F61C89B: // plr_loco_swing_base_state
            return construct(0x60, retail::sub_580080);
        case 0xCB7D35DB: // plr_loco_swing_launch_state
            return construct(0x6C, retail::sub_4D52A0);
        case 0xD64AA3A1: // plr_loco_swing_state
            return construct(0xB0, retail::sub_4D53E0);
        case 0x98E36A48: // plr_loco_wall_sprint_state
            return construct(0xC4, retail::sub_4D2590);
        case 0x3DC2D412: // plr_loco_wall_sprint_transition_state
            return construct(0x8C, retail::sub_4D2680);
        case 0x2F6A4BC8: // plr_loco_wallplant_state
            return construct(0x78, retail::sub_5805A0);
        case 0x30805936: // hit_switch_state
            return construct(0x4C, retail::sub_580EA0);
        case 0xA27DD823: // plr_hit_switch_state
            return construct(0x50, retail::sub_4D4310);
        case 0xDBC0FBD3: // plr_ranged_hit_switch_state
            return construct(0x50, retail::sub_4D43B0);
        case 0x245B0FC1: // plr_prop_throw_state
            return construct(0xC0, retail::sub_580C20);
        case 0x540B5071: // plr_extended_prop_throw_state
            return construct(0xD0, retail::sub_4D56A0);
        case 0x62F59477: // launch_layer_state
            return construct(0x54, retail::sub_4CF860);
        case 0x906AA72E: // steal_car_state
            return construct(0x54, retail::sub_4D2900);
        case 0x85BC0419: // abduction_state
            return construct(0x54, retail::sub_4D5750);
        case 0x287E5A16: // harassment_state
            return construct(0x54, retail::sub_4D6070);
        case 0xB4BAF48F: // take_hostage_state
            return construct(0x54, retail::sub_4D63F0);
        case 0x3B64D21C: // flee_state
            return construct(0x5C, retail::sub_4D1160);
        case 0x76E6EA12: // patrol_state
            return construct(0x54, retail::sub_4D0F30);
        case 0x8B6C2436: // search_state
            return construct(0x54, retail::sub_4D3EE0);
        case 0x21C2A517: // wait_for_layer_state
            return construct(0x4C, retail::sub_4CF920);
        case 0xB814C802: // attack_enabled_state
            return construct(0x154, retail::sub_486610);
        case 0x0EC3D69E: // plr_loco_evade_state
            return construct(0x194, retail::sub_580690);
        case 0x115926B9: // plr_loco_air_evade_state
            return construct(0x194, retail::sub_580750);
        case 0x7C9EBD4D: // plr_loco_wall_evade_state
            return construct(0x1C0, retail::sub_4D6780);
        case 0x35D1D8EE: // plr_loco_pole_swinging_state
            return construct(0x1E0, retail::sub_5804B0);
        case 0xB8B1080D: // plr_loco_running_grind_state
            return construct(0x17C, retail::sub_4D6AC0);
        case 0x8417F8D1: // plr_loco_standing_state
            return construct(0x1E4, retail::sub_4D6920);
        case 0xEEB4561D: // generic_sync_anim_state
            return construct(0x16C, retail::sub_4D0AA0);
        case 0x0C34884F: // ai_obs_hop_sync_state
            return construct(0x16C, retail::sub_4D0B30);
        case 0x218FDD11: // generic_slim_sync_anim_state
            return construct(0x16C, retail::sub_4D0BC0);
        case 0x58C6AED4: // plr_loco_sync_obstacle_avoid_state
            return construct(0x184, retail::sub_580970);
        case 0xC98C9073: // enter_car_state
            return construct(0x16C, retail::sub_4D29F0);
        case 0x551F42AF: // exit_car_state
            return construct(0x174, retail::sub_4D2AB0);
        case 0x1FD1A663: // hold_abductee_state
            return construct(0x17C, retail::sub_4D59A0);
        case 0xAEB0722D: // harass_victim_state
            return construct(0x16C, retail::sub_4D62C0);
        case 0xFC896F31: // hold_hostage_state
            return construct(0x178, retail::sub_4D65F0);
        case 0x8028B654: // pick_up_ai_state
            return construct(0x170, retail::sub_4C0B60);
        case 0xF2263541: // plr_pick_up_ai_state
            return construct(0x174, retail::sub_4D48D0);
        case 0xF9D700B9: // put_down_ai_state
            return construct(0x174, retail::sub_4C0C50);
        case 0xAB845D46: // plr_put_down_ai_state
            return construct(0x178, retail::sub_4D49C0);
        case 0x97220741: // biped_idle_state
            return construct(0x158, retail::sub_4BF3C0);
        case 0xD8DA578F: // generic_combat_idle_state
            return construct(0x15C, retail::sub_4D0CB0);
        case 0xE2DC00E1: // civ_beat_up_state
            return construct(0x15C, retail::sub_4D5C20);
        case 0x49C551B1: // cover_combat_idle_state
            return construct(0x15C, retail::sub_4D3C20);
        case 0x87D434BC: // cover_idle_state
            return construct(0x15C, retail::sub_4D3CB0);
        case 0x6563F94C: // generic_move_to_attack_state
            return construct(0x158, retail::sub_4D0D80);
        case 0xD45E4C90: // civ_beat_up_move_to_attack_state
            return construct(0x158, retail::sub_4D5D00);
        case 0xE6E90C1A: // generic_noncombat_idle_state
            return construct(0x158, retail::sub_4D0900);
        case 0xA71C7271: // generic_slim_biped_idle_state
            return construct(0x158, retail::sub_4D09A0);
        case 0xCB40BC2B: // car_combat_state
            return construct(0x15C, retail::sub_4D2CD0);
        case 0xA640A640: // car_shooting_state
            return construct(0x15C, retail::sub_4D2DA0);
        case 0x88C36C96: // find_car_state
            return construct(0x158, retail::sub_4D27F0);
        case 0x4F4BD11D: // find_abductee_state
            return construct(0x158, retail::sub_4D5850);
        case 0x5462CF6E: // wait_for_abductor_state
            return construct(0x158, retail::sub_4D5A70);
        case 0x76BB19AC: // civ_beat_up_victim_state
            return construct(0x15C, retail::sub_4D5E60);
        case 0x5383B967: // find_civ_for_beat_up_state
            return construct(0x158, retail::sub_4D5B10);
        case 0xE89F31A1: // find_harassment_victim_state
            return construct(0x158, retail::sub_4D6160);
        case 0x77709BD3: // wait_for_harasser_state
            return construct(0x158, retail::sub_4D6350);
        case 0xC7AC2AAB: // find_hostage_state
            return construct(0x158, retail::sub_4D64E0);
        case 0x56160EFB: // wait_for_hostage_taker_state
            return construct(0x158, retail::sub_4D6690);
        case 0x01CFA0EA: // medical_emergency_idle_state
            return construct(0x158, retail::sub_4D6B90);
        case 0x44908DBB: // puppet_idle_state
            return construct(0x158, retail::sub_4CFB60);
        case 0x41F75CC5: // puppet_facing_state
            return construct(0x158, retail::sub_4CFE50);
        case 0x23E2E6E1: // puppet_rappel_state
            return construct(0x158, retail::sub_4D0030);
        case 0x6B7F6E61: // puppet_release_rappel_state
            return construct(0x158, retail::sub_4D00D0);
        case 0x14DE83DB: // play_single_anim_state
            return construct(0x158, retail::sub_4BF4B0);
        case 0x12F104D6: // backup_state
            return construct(0x158, retail::sub_4D36A0);
        case 0x07C64AB8: // dodge_car_state
            return construct(0x15C, retail::sub_4D3740);
        case 0x3DA919DE: // manual_state
            return construct(0x15C, retail::sub_4D3520);
        case 0x32FA8477: // puppet_play_anim_state
            return construct(0x188, retail::sub_4CFC00);
        case 0xF641B6FB: // puppet_loop_anim_state
            return construct(0x188, retail::sub_4CFDA0);
        case 0x4D0DCA7A: // climb_over_goto_state
            return construct(0x158, retail::sub_4CF610);
        case 0x9D92D0DF: // climb_up_on_goto_state
            return construct(0x158, retail::sub_4CF570);
        case 0xBA5E1CDA: // generic_on_anim_signal_state
            return construct(0x164, retail::sub_4CF450);
        case 0xC05E7814: // step_up_on_goto_state
            return construct(0x158, retail::sub_4CF6B0);
        case 0x3ACDF0DF: // vault_over_goto_state
            return construct(0x158, retail::sub_4CF750);
        case 0x8E5250A5: // pick_up_obj_state
            return construct(0x160, retail::sub_4C0D30);
        case 0x3FFFAD32: // plr_pick_up_obj_state
            return construct(0x164, retail::sub_4D4A70);
        case 0x3DC9E7AA: // put_down_obj_state
            return construct(0x15C, retail::sub_4C0E80);
        case 0x2522D5D7: // plr_put_down_obj_state
            return construct(0x160, retail::sub_4D4B20);
        case 0x4E5B5073: // wall_splat_state
            return construct(0x1C0, retail::sub_4D3080);
        case 0xC9BA46EE: // search_area_state
            return construct(0x158, retail::sub_4D3E40);
        case 0xC70CBBE6: // synced_biped_idle_state
            return construct(0x158, retail::sub_4CF3B0);
        case 0x6830C178: // synced_controlled_idle_state
            return construct(0x170, retail::sub_4D3180);
        case 0x8CBB29AA: // stun_state
            return construct(0x158, retail::sub_4D2E70);
        case 0x3EB33AD7: // plr_stun_state
            return construct(0x178, retail::sub_4D4780);
        case 0xDEB0C09F: // find_cover_state
            return construct(0x158, retail::sub_4D3B10);
        case 0x0D0262C6: // death_state
            return construct(0x168, retail::sub_4D3270);
        case 0xFDFC9993: // plr_death_state
            return construct(0x168, retail::sub_4D4450);
        case 0x0808811E: // ped_death_state
            return construct(0x168, retail::sub_4D3610);
        case 0x3A4BBD4A: // std_cower_state
            return construct(0x158, retail::sub_4D0170);
        case 0xFEB16DA0: // flee_begin_state
            return construct(0x15C, retail::sub_4D1020);
        case 0x8ACBAACB: // flee_from_entity_state
            return construct(0x15C, retail::sub_4D10C0);
        case 0x54CC8956: // select_patrol_node_state
            return construct(0x158, retail::sub_4D0E20);
        case 0xB25BF009: // rappel_fire_state
            return construct(0x158, retail::sub_4CFA50);
        case 0x64750206: // puppet_fire_rappel_state
            return construct(0x158, retail::sub_4CFF40);
        case 0xF0D86B3A: // search_begin_state
            return construct(0x158, retail::sub_4D3D90);
        case 0x9B57AA69: // generic_loco_state
            return construct(0x158, retail::sub_486990);
        case 0x996F3794: // swift_crawl_jump_fly_transition_state
            return construct(0x15C, retail::sub_4D1830);
        case 0x11595712: // swift_jump_fly_state
            return construct(0x160, retail::sub_4873E0);
        case 0x7ECAA6C6: // swift_jump_land_state
            return construct(0x158, retail::sub_4D16F0);
        case 0x37C5EBA2: // swift_jump_launch_state
            return construct(0x15C, retail::sub_487340);
        case 0x566349EE: // hybrid_loco_state
            return construct(0x15C, retail::sub_487580);
        case 0x33F935DB: // hybrid_calculate_path_state
            return construct(0x15C, retail::sub_487610);
        case 0xDCBF08C6: // jump_fly_state
            return construct(0x15C, retail::sub_486EB0);
        case 0x33C139DF: // plr_loco_jump_fly_state
            return construct(0x1E8, retail::sub_4D6C30);
        case 0xD4286FD6: // plr_loco_falling_state
            return construct(0x1EC, retail::sub_4D6D60);
        case 0x08A97F5C: // plr_loco_wall_grind_state
            return construct(0x200, retail::sub_4D6E00);
        case 0x99A24987: // hybrid_jump_fly_state
            return construct(0x164, retail::sub_487840);
        case 0xCAB0727F: // fall_state
            return construct(0x180, retail::sub_4D0280);
        case 0xA3C025D7: // ped_fall_state
            return construct(0x180, retail::sub_4D37E0);
        case 0x9ED56754: // saved_civ_state
            return construct(0x184, retail::sub_4D3870);
        case 0x613219D1: // saved_fall_state
            return construct(0x194, retail::sub_4D3960);
        case 0x816100D5: // saved_wall_grind_state
            return construct(0x1E4, retail::sub_4D39F0);
        case 0xB15EAA55: // exploded_state
            return construct(0x180, retail::sub_4D2F60);
        case 0xD6258A02: // plr_exploded_state
            return construct(0x190, retail::sub_4D4670);
        case 0xFCF0EEE3: // civilian_exploded_state
            return construct(0x180, retail::sub_4D5FC0);

        case 0x7D26ECB1: { // zip_fall_state
            auto* state = (zip_fall_state*)mm::allocate(0x198);

            if (!state)
                return nullptr;

            ((fall_state* (__thiscall*)(fall_state*))retail::sub_4D0280)(state); // fall_state::fall_state

            *(void**)state = &references::zip_fall_state_vtable.get();

            state->unk_180 = false;
            state->unk_190 = false;

            return state;
        }

        case 0xE67CB0D6: // jump_launch_state
            return construct(0x15C, retail::sub_486DD0);
        case 0x112AF74F: // plr_loco_jump_launch_state
            return construct(0x170, retail::sub_57FF30);
        case 0xC001C5F7: // hybrid_jump_launch_state
            return construct(0x168, retail::sub_4876F0);
        case 0x722F4645: // loco_goto_state
            return construct(0x164, retail::sub_486A20);
        case 0x06735979: // nonpathed_goto_state
            return construct(0x174, retail::sub_486AB0);
        case 0x51686B6E: // pathed_goto_state
            return construct(0x178, retail::sub_486B90);
        case 0xAE0494F4: // crawl_ground_pathed_goto_state
            return construct(0x178, retail::sub_4D13B0);
        case 0x2AED808F: // hybrid_pathed_goto_state
            return construct(0x17C, retail::sub_487960);
        case 0x6178FDF1: // crawl_goto_state
            return construct(0x174, retail::sub_4D1290);
        case 0x84545D3F: // crawl_ground_nonpathed_goto_state
            return construct(0x174, retail::sub_4D1320);
        case 0x396E66FA: // hybrid_nonpathed_goto_state
            return construct(0x178, retail::sub_4878D0);
        case 0x2EA3ED83: // fly_goto_state
            return construct(0x178, retail::sub_4D1B30);
        case 0x7CED5516: // wounded_state
            return construct(0x1A8, retail::sub_4D41E0);
        case 0x95507323: // plr_wounded_state
            return construct(0x1AC, retail::sub_4D4560);
        case 0x89E68D4D: // plr_car_combat_wounded_state
            return construct(0x1AC, retail::sub_4D4DC0);
        case 0x29E23FE4: // civilian_wounded_state
            return construct(0x1A8, retail::sub_4D5F30);
        case 0x0E30D75D: // cpu_combat_state
            return construct(0x300, retail::sub_4D0890);
        case 0xC2CA2E5D: // civ_beat_up_cpu_combat_state
            return construct(0x300, retail::sub_4D5DF0);
        case 0xDA68A2A6: // generic_slim_combat_state
            return construct(0x2F8, retail::sub_4D0A40);
        case 0x6E055FE3: // plr_combat_state
            return construct(0x304, retail::sub_4D40E0);
        case 0xEE39BA2F: // plr_car_combat_attack_state
            return construct(0x304, retail::sub_4D4D40);
        case 0x05F66503: // rhino_ride_state
            return construct(0x318, retail::sub_4D4EA0);
        case 0x62C467AA: // plr_trick_state
            return construct(0x318, retail::sub_4D6F70);
        case 0xCA21AD52: // crawl_calculate_path_state
            return construct(0x44, retail::sub_4872B0);
        case 0x7342EB03: // crawl_transition_state
            return construct(0x5C, retail::sub_4D1460);
        case 0xDE0D251C: // plr_loco_crawl_transition_state
            return construct(0x78, retail::sub_4D50F0);
        case 0x664B685B: // plr_strength_test_state
            return construct(0x78, retail::sub_4D04C0);
        case 0x9004FAD1: // plr_slingshot_point_state
            return construct(0xA8, retail::sub_4D4F30);
        case 0x14748574: // ped_flee_state
            return enhanced_state::construct_inline(0x48, &references::ped_flee_state_vtable.get());
        case 0x15D97849: // plr_loco_yoyo_state
            return construct(0x48, retail::sub_4D5500);
        case 0x16BC36ED: // go_to_car_state
            return enhanced_state::construct_inline(0x44, &references::go_to_car_state_vtable.get());
        case 0x1CCAA661: // generic_wait_for_movement_state
            return enhanced_state::construct_inline(0x44, &references::generic_wait_for_movement_state_vtable.get());
        case 0x213CC54F: // swift_crawl_transition_state
            return enhanced_state::construct_inline(0x48, &references::swift_crawl_transition_state_vtable.get());

        case 0x24A49FCF: { // drive_car_state
            auto* state = (drive_car_state*)mm::allocate(0x48);

            if (!state)
                return nullptr;

            state->base_state    ::initialize(mash::ALLOCATED);
            state->enhanced_state::initialize(mash::ALLOCATED);

            state->car_combat_stop_car = false;

            *(void**)state = &references::drive_car_state_vtable.get();

            return state;
        }

        case 0x27EE78A8: // puppet_movement_state
            return enhanced_state::construct_inline(0x48, &references::puppet_movement_state_vtable.get());
        case 0x35A45CBD: // swift_obstacle_break_state
            return enhanced_state::construct_inline(0x44, &references::swift_obstacle_break_state_vtable.get());
        case 0x3D3E3B18: // plr_car_combat_state
            return enhanced_state::construct_inline(0x44, &references::plr_car_combat_state_vtable.get());

        case 0x3C18568D: { // plr_swim_state
            auto* state = (plr_swim_state*)mm::allocate(0x48);

            if (!state)
                return nullptr;

            state->base_state    ::initialize(mash::ALLOCATED);
            state->enhanced_state::initialize(mash::ALLOCATED);

            *(void**)state = &references::plr_swim_state_vtable.get();

            state->can_dodge = false;

            return state;
        }

        case 0x404D7FDD: { // force_drive_car_state
            auto* state = (force_drive_car_state*)mm::allocate(0x48);

            if (!state)
                return nullptr;

            state->base_state    ::initialize(mash::ALLOCATED);
            state->enhanced_state::initialize(mash::ALLOCATED);

            *(void**)state = &references::force_drive_car_state_vtable.get();

            state->car_combat_stop_car = false;

            return state;
        }

        case 0x5543A9D4: // go_to_abductee_state
            return enhanced_state::construct_inline(0x44, &references::go_to_abductee_state_vtable.get());
        case 0x59905975: // wait_for_car_stop_state
            return enhanced_state::construct_inline(0x44, &references::wait_for_car_stop_state_vtable.get());
        case 0x6277C838: // go_to_harassment_victim_state
            return enhanced_state::construct_inline(0x44, &references::go_to_harassment_victim_state_vtable.get());
        case 0x7B4DA8B2: // ped_fleeing_state
            return enhanced_state::construct_inline(0x44, &references::ped_fleeing_state_vtable.get());
        case 0x811A62FE: // swift_wall_locomotion_state
            return enhanced_state::construct_inline(0x5C, &references::swift_wall_locomotion_state_vtable.get());
        case 0x948553CB: // swift_obstacle_avoid_state
            return enhanced_state::construct_inline(0x68, &references::swift_obstacle_avoid_state_vtable.get());
        case 0x979A9D92: // plr_loco_crawl_state
            return enhanced_state::construct_inline(0x50, &references::plr_loco_crawl_state_vtable.get());
        case 0xA4AAA5F9: // civ_beat_up_goto_state
            return enhanced_state::construct_inline(0x44, &references::civ_beat_up_goto_state_vtable.get());
        case 0xA5149C52: // swift_attach_to_wall_state
            return enhanced_state::construct_inline(0x60, &references::swift_attach_to_wall_state_vtable.get());
        case 0xAEFFD7BF: // plr_agility_test_state
            return enhanced_state::construct_inline(0x68, &references::plr_agility_test_state_vtable.get());
        case 0xB3F3CFA2: // plr_loco_wall_sprint_base_state
            return construct(0x48, retail::sub_4D23F0);
        case 0xBB8A470F: // swift_locomotion_state
            return enhanced_state::construct_inline(0x50, &references::swift_locomotion_state_vtable.get());
        case 0xC080914F: // teleporting_locomotion_state
            return enhanced_state::construct_inline(0x44, &references::teleporting_locomotion_state_vtable.get());
        case 0xCB1AAB19: // swift_fly_locomotion_state
            return enhanced_state::construct_inline(0x50, &references::swift_fly_locomotion_state_vtable.get());
        case 0xCE057CB6: // go_to_cover_state
            return enhanced_state::construct_inline(0x44, &references::go_to_cover_state_vtable.get());
        case 0xD29CF08F: // ped_watch_state
            return enhanced_state::construct_inline(0x4C, &references::ped_watch_state_vtable.get());
        case 0xD152A815: // raise_event_state
            return enhanced_state::construct_inline(0x44, &references::raise_event_state_vtable.get());
        case 0xD5BD4A0C: // going_to_patrol_node_state
            return enhanced_state::construct_inline(0x44, &references::going_to_patrol_node_state_vtable.get());
        case 0xDC8A9456: // enhanced_state
            return enhanced_state::construct_inline(0x44, &references::enhanced_state_vtable.get());
        case 0xDF204882: // go_to_hostage_state
            return enhanced_state::construct_inline(0x44, &references::go_to_hostage_state_vtable.get());
        case 0xE0DF4C13: // unmoving_state
            return enhanced_state::construct_inline(0x44, &references::unmoving_state_vtable.get());
        case 0xE9CBFEA1: // wander_state
            return enhanced_state::construct_inline(0x48, &references::wander_state_vtable.get());
        case 0xEB53E7DA: // swift_stationary_state
            return enhanced_state::construct_inline(0x44, &references::swift_stationary_state_vtable.get());
        case 0xEB4FECFB: // slave_state
            return enhanced_state::construct_inline(0x44, &references::slave_state_vtable.get());
        case 0xED304924: // swift_find_and_attach_to_wall_state
            return enhanced_state::construct_inline(0x60, &references::swift_find_and_attach_to_wall_state_vtable.get());
        case 0xF63DD116: // plr_blocking_state
            return construct(0x80, retail::sub_580CA0);

        default:
            return nullptr;
    }
}
