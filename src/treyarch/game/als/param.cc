#include <cfloat>
#include <cmath>
#include <cstring>
#include <new>

#include "retail.hh"
#include "treyarch/game/als/animation_logic_system.hh"
#include "treyarch/game/als/internal.hh"
#include "treyarch/game/als/param.hh"
#include "treyarch/game/als/state_machine.hh"
#include "treyarch/game/animation/animation_controller.hh"
#include "treyarch/game/wds/ai/ai_core.hh"
#include "treyarch/game/wds/ai/info_node.hh"
#include "treyarch/game/wds/entity/actor.hh"
#include "treyarch/game/wds/entity/conglomerate.hh"
#include "treyarch/game/wds/entity/interface/physical_interface.hh"
#include "treyarch/game/wds/entity/interface/player_interface.hh"
#include "treyarch/game/wds/entity/interface/standing_interface.hh"
#include "treyarch/shared/math/references.hh"

using namespace treyarch;

// sub_5B76D0
als::parameter_list::parameter_list() : vector_set{},
                                        hash_set{} {

    std::memset(float_set, 0, sizeof(float_set));
    std::memset(this, 0, sizeof(*this));
}

// sub_5B0110
// inlined @ sub_5BBB90
bool als::parameter_list::has_param_been_set(param_id param) const {
    if (param < num_external_float_params)
        return float_set[param];

    if (param >= num_external_hash_params)
        return false;

    if (param < num_external_vector_params)
        return vector_set[param - num_external_float_params];

    return hash_set[param - num_external_vector_params];
}

// sub_5B7760
als::internal_parameter_cache::internal_parameter_cache() : vector_set{},
                                                            hash_set{} {

    std::memset(float_set, 0, sizeof(float_set));
    std::memset(this, 0, sizeof(*this));
}

// sub_5B0160
// inlined @ sub_5ED8C0
void als::internal_parameter_cache::invalidate_per_layer() { // the computed values that depend on the machine asking
    float_set[0]  = false;
    float_set[1]  = false;
    float_set[2]  = false;
    float_set[3]  = false;
    float_set[38] = false;
    float_set[30] = false;
    float_set[31] = false;
    float_set[39] = false;
    float_set[40] = false;
    float_set[41] = false;
    float_set[4]  = false;
    float_set[5]  = false;
    float_set[6]  = false;
    float_set[57] = false;
    float_set[58] = false;
    hash_set[2]   = false;
    hash_set[3]   = false;
    hash_set[4]   = false;
    hash_set[5]   = false;
}

// sub_5EC560
// inlined @ sub_5EEF20
// inlined @ sub_5F4BD0
// inlined @ sub_5EFE70
// inlined @ sub_5F0A00
f32 als::internal_parameter_cache::get_internal_float(animation_logic_system* the_als, state_machine* the_sm, param_id param) {
    u32 index = param - num_external_hash_params;

    if (!float_set[index]) {
        floats[index]    = calculate_internal_float(the_als, the_sm, param);
        float_set[index] = true;
    }

    return floats[index];
}

// sub_5CD4E0
vector3d als::internal_parameter_cache::get_internal_vector(animation_logic_system* the_als, state_machine* the_sm, param_id param) {
    u32 index = param - num_internal_float_params;

    if (!vector_set[index]) {
        vectors[index]    = calculate_internal_vector(the_als, the_sm, param);
        vector_set[index] = true;
    }

    return vectors[index];
}

// sub_5CD560
f32 als::internal_parameter_cache::get_internal_vector_component(animation_logic_system* the_als,
                                                                 state_machine*          the_sm,
                                                                 param_id                param,
                                                                 e_vector_component      component) {

    u32 index = param - num_internal_float_params;

    if (!vector_set[index]) {
        vectors[index]    = calculate_internal_vector(the_als, the_sm, param);
        vector_set[index] = true;
    }

    return get_vector_component(vectors[index], component);
}

// sub_5CD5D0
string_hash als::internal_parameter_cache::get_internal_hash(animation_logic_system* the_als, state_machine* the_sm, param_id param) {
    u32 index = param - num_internal_vector_params;

    if (!hash_set[index]) {
        hashes[index]   = calculate_internal_hash(the_als, the_sm, param);
        hash_set[index] = true;
    }

    return hashes[index];
}

// sub_5E90D0
f32 als::internal_parameter_cache::calculate_internal_float(animation_logic_system* the_als, state_machine* the_sm, param_id param) {
    actor* the_actor = the_als->get_actor();

    switch (param) {
        case param_current_time_in_seconds: {
            f32 duration = the_sm->the_controller->get_anim_duration(the_sm->get_layer_id());

            return (f32)((f64)the_sm->the_controller->get_anim_normalized_time(the_sm->get_layer_id()) * duration);
        }

        case param_current_anim_time_in_seconds: {
            f32 t        = the_sm->the_controller->get_anim_normalized_time(the_sm->get_layer_id());
            f32 duration = the_sm->the_controller->get_anim_duration(the_sm->get_layer_id());

            return duration * (f32)(t - (i32)t);
        }

        case param_current_normalized_time:
            return the_sm->the_controller->get_anim_normalized_time(the_sm->get_layer_id());

        case param_current_anim_normalized_time: {
            f32 t = the_sm->the_controller->get_anim_normalized_time(the_sm->get_layer_id());

            return t - (i32)t;
        }

        // retail's pitch is the yaw as well
        case param_aim_pitch_angle:
        case param_aim_yaw_angle: {
            vector3d aim = the_sm->get_external_vector(param_aim_direction);

            return compute_angle_between_vectors(the_actor->my_abs_po->get_z_facing(), the_actor->my_abs_po->get_x_facing(), aim);
        }

        case param_frames_from_end_of_anim: {
            f32 left = (f32)-((f64)the_sm->the_controller->get_anim_normalized_time(the_sm->get_layer_id()) - 1.0);

            if (retail::sub_5BA330((i32*)the_sm->the_controller, the_sm->get_layer_id()))
                left = the_sm->the_controller->get_anim_duration(the_sm->get_layer_id()) * left;

            return (f32)((f64)left * 30.0);
        }

        case param_jump_charge:
            if (!retail::sub_402500((u32*)the_actor)) // has_player_ifc
                return 0.0f;

            return the_actor->player_ifc()->unk_149();

        case param_active_jump_charge:
            if (!retail::sub_402500((u32*)the_actor))
                return 0.0f;

            return the_actor->player_ifc()->unk_148();

        case param_damage_stunned:
            if (!retail::sub_4024A0((u32*)the_actor) || !retail::sub_40A880((u32*)the_actor->damage_ifc())) // has_damage_ifc
                return 0.0f;

            return 1.0f;

        case param_damage_hit_point_percent:
            if (!retail::sub_4024A0((u32*)the_actor))
                return 1.0f;

            return (f32)retail::sub_5BAB80((u32*)the_actor->damage_ifc());

        case param_damage_armor_percent:
            if (!retail::sub_4024A0((u32*)the_actor))
                return 0.0f;

            return (f32)retail::sub_5BABD0((u32*)the_actor->damage_ifc());

        case param_swing_time:
            if (!retail::sub_402500((u32*)the_actor))
                return 0.0f;

            return the_actor->player_ifc()->unk_374();

        case param_swing_to_anchor_angle:
            if (!retail::sub_402500((u32*)the_actor))
                return 1.0f;

            return the_actor->player_ifc()->unk_429();

        case param_swing_angle:
            if (!retail::sub_402500((u32*)the_actor))
                return 270.0f;

            return the_actor->player_ifc()->unk_425();

        case param_swing_side_angle:
            if (!retail::sub_402500((u32*)the_actor))
                return 1.0f;

            return the_actor->player_ifc()->unk_427();

        case param_swing_hand:
            if (!retail::sub_402500((u32*)the_actor))
                return -1.0f;

            return (f32)the_actor->player_ifc()->unk_401();

        case param_swing_entry_speed:
            if (!retail::sub_402500((u32*)the_actor))
                return 1.0f;

            return the_actor->player_ifc()->unk_432();

        case param_swing_exit_speed:
            if (!retail::sub_402500((u32*)the_actor))
                return 1.0f;

            return the_actor->player_ifc()->unk_433();

        case param_trick_window:
            if (!retail::sub_402500((u32*)the_actor) || !the_actor->player_ifc()->unk_152())
                return 0.0f;

            return 1.0f;

        case param_max_trick_combo:
            if (!retail::sub_402500((u32*)the_actor))
                return 0.0f;

            return (f32)the_actor->player_ifc()->unk_154();

        case param_blacksuit_swing_window:
            if (!retail::sub_402500((u32*)the_actor) || !the_actor->player_ifc()->unk_150())
                return 0.0f;

            return 1.0f;

        case param_swing_level_rating:
            if (!retail::sub_402500((u32*)the_actor))
                return 1.0f;

            return the_actor->player_ifc()->unk_403();

        // 1 or -1
        case param_is_carrying: {
            if (!the_actor->get_ai_core())
                return -1.0f;

            info_node* carry = the_actor->get_ai_core()->get_info_node(info_node_type_std_carry);

            if (carry && retail::sub_50C9D0((u32*)carry))
                return 1.0f;

            if (!retail::sub_402500((u32*)the_actor))
                return -1.0f;

            if (the_actor->player_ifc()->unk_244 == 21 || the_actor->player_ifc()->unk_244 == 22)
                return 1.0f;

            return -1.0f;
        }

        case param_is_being_carried: {
            if (!the_actor->get_ai_core())
                return -1.0f;

            auto* carry = (std_carry_inode*)the_actor->get_ai_core()->get_info_node(info_node_type_std_carry);

            if (!carry || !carry->unk_030)
                return -1.0f;

            return 1.0f;
        }

        case param_smoothed_speed:
            if (!retail::sub_402500((u32*)the_actor))
                return 0.0f;

            return (f32)retail::sub_402020((f32*)the_actor->player_ifc()->unk_095());

        case param_smoothed_lateral_speed:
            if (!retail::sub_402500((u32*)the_actor))
                return 0.0f;

            return (f32)retail::sub_4248B0((f32*)the_actor->player_ifc()->unk_095());

        case param_smoothed_vertical_speed:
            if (!retail::sub_402500((u32*)the_actor))
                return 0.0f;

            return the_actor->player_ifc()->unk_095()->y;

        case param_strafe_angle: {
            vector3d move = the_sm->get_external_vector(param_desired_movement_dir);

            return compute_angle_between_vectors(the_actor->my_abs_po->get_z_facing(), the_actor->my_abs_po->get_x_facing(), move);
        }

        case param_strafe_y_angle: {
            vector3d move = the_sm->get_external_vector(param_desired_movement_dir);

            return compute_angle_between_vectors(the_actor->my_abs_po->get_z_facing(), the_actor->my_abs_po->get_y_facing(), move);
        }

        case param_strafing_mode: {
            ai_core* core = the_actor->get_ai_core();

            if (!core || !retail::sub_4C2F60((u32*)core))
                return 0.0f;

            return 1.0f;
        }

        case param_elevation:
            if (!retail::sub_4024E0((u32*)the_actor)) // has_standing_ifc
                return 0.0f;

            return ((standing_interface*)the_actor->my_ifc_storage->get_ifc(entity_ifc_standing))->unk_03c;

        // the physics' heights, none when it reports a negative one
        case param_height_above_ground:
        case param_height_above_floor:
        case param_height_above_wall: {
            f32 height = FLT_MAX;

            if (!retail::sub_4024C0((u32*)the_actor)) // has_physical_ifc
                return height;

            height = (f32)retail::sub_60D730((u32*)the_actor->physical_ifc(), param == param_height_above_floor, param == param_height_above_wall);

            if (0.0f > height)
                height = FLT_MAX;

            return height;
        }

        case param_attack_angle: {
            vector3d      attack = the_sm->get_external_vector(param_attack_direction);
            math::vector4 dir(attack.x, attack.y, attack.z, 0.0f);
            math::vector4 reversed;

            retail::sub_401A20((f32*)&reversed, (f32*)&dir);

            vector3d from(reversed[0], reversed[1], reversed[2]);

            return compute_angle_between_vectors(the_actor->my_abs_po->get_z_facing(), the_actor->my_abs_po->get_x_facing(), from);
        }

        case param_computed_distance: {
            vector3d        destination = the_sm->get_external_vector(param_desired_destination);
            const vector3d &position    = the_actor->my_abs_po->get_position();

            math::vector4 offset(destination.x - position.x, destination.y - position.y, destination.z - position.z, 0.0f - 0.0f);

            return (f32)retail::sub_42A9E0((f32*)&offset);
        }

        case param_desired_angle: {
            vector3d forward = the_sm->get_external_vector(param_desired_forward_vector);

            return compute_angle_between_vectors(the_actor->my_abs_po->get_z_facing(), the_actor->my_abs_po->get_x_facing(), forward);
        }

        // the desired angle's quarter: ahead, right, behind, left
        case param_desired_dir: {
            f32 angle = get_internal_float(the_als, the_sm, param_desired_angle);

            if (angle >= -45.0f && angle < 45.0f)
                return 0.0f;

            if (angle >= 45.0f && angle < 135.0f)
                return 1.0f;

            if (angle >= 135.0f || angle < -135.0f)
                return 2.0f;

            if (angle >= -135.0f && angle < -45.0f)
                return 3.0f;

            return 0.0f;
        }

        case param_phys_stable:
            if (!retail::sub_4024C0((u32*)the_actor) || !the_actor->physical_ifc()->is_flagged(physical_interface_flag_biped_running) || retail::sub_600630((i32)the_actor->physical_ifc(), false))
                return 1.0f;

            return 0.0f;

        case param_phys_active:
            if (!retail::sub_4024C0((u32*)the_actor) || !the_actor->physical_ifc()->is_flagged(physical_interface_flag_biped_running))
                return 0.0f;

            return 1.0f;

        case param_phys_allowed:
            if (!retail::sub_4024C0((u32*)the_actor) || (the_actor->physical_ifc()->flags & 0x40000))
                return 0.0f;

            return 1.0f;

        case param_effectively_standing:
            if (!retail::sub_4024E0((u32*)the_actor) || !(((standing_interface*)the_actor->my_ifc_storage->get_ifc(entity_ifc_standing))->unk_044 & 2))
                return 0.0f;

            return 1.0f;

        // biped 0, crawl 1, flight 2, hybrid 3, teleporting 4; the swift layers by their state; -1 without one
        case param_ai_locomotion_mode: {
            ai_core* core = the_actor->get_ai_core();

            if (!core)
                return -1.0f;

            loco_inode* loco = core->my_loco_inode;

            if (!loco)
                return -1.0f;

            if (loco->get_virtual_type_key() == biped_layer_inode_key.source_hash_code)
                return 0.0f;

            if (loco->get_virtual_type_key() == crawl_layer_inode_key.source_hash_code)
                return 1.0f;

            if (loco->get_virtual_type_key() == flight_layer_inode_key.source_hash_code)
                return 2.0f;

            if (loco->get_virtual_type_key() == swift_flight_layer_inode_key.source_hash_code)
                return 2.0f;

            if (loco->get_virtual_type_key() == hybrid_layer_inode_key.source_hash_code)
                return 3.0f;

            if (loco->get_virtual_type_key() == swift_surface_layer_inode_key.source_hash_code)
                return (loco->unk_0ac & 2) ? 1.0f : 0.0f;

            if (loco->get_virtual_type_key() == swift_stationary_layer_inode_key.source_hash_code)
                return retail::sub_4517D0((u32*)loco) ? 2.0f : 0.0f;

            if (loco->get_virtual_type_key() == teleporting_layer_inode_key.source_hash_code)
                return 4.0f;

            return -1.0f;
        }

        case param_ai_flight_bank: {
            ai_core* core = the_actor->get_ai_core();

            if (!core)
                return 0.0f;

            loco_inode* loco = core->my_loco_inode;

            if (!loco || loco->get_virtual_type_key() != flight_layer_inode_key.source_hash_code)
                return 0.0f;

            return ((flight_layer_inode*)loco)->unk_118;
        }

        case param_ai_flight_pitch: {
            ai_core* core = the_actor->get_ai_core();

            if (!core)
                return 0.0f;

            loco_inode* loco = core->my_loco_inode;

            if (!loco || loco->get_virtual_type_key() != flight_layer_inode_key.source_hash_code)
                return 0.0f;

            return ((flight_layer_inode*)loco)->unk_128;
        }

        // the combat node's slice of its pie, while it has one
        case param_ai_pie_ring: {
            ai_core* core = the_actor->get_ai_core();

            if (!core)
                return -1.0f;

            info_node* combat = core->get_info_node(info_node_type_cpu_combat);

            if (!combat)
                return -1.0f;

            u8* slice = (u8*)combat + 0x60;

            if (!retail::sub_4E38A0((u32*)slice) || retail::sub_4E7640((f32*)slice))
                return -1.0f;

            return (f32)*(i32*)(slice + 0x0C);
        }

        case param_ai_idle_type: {
            ai_core* core = the_actor->get_ai_core();

            if (!core)
                return 0.0f;

            return (f32)(i32)core->unk_5bc;
        }

        case param_ai_walk_run_cycle: {
            ai_core* core = the_actor->get_ai_core();

            if (!core)
                return 0.0f;

            return (f32)(i32)core->unk_5c0;
        }

        case param_ai_walk_item: {
            ai_core* core = the_actor->get_ai_core();

            if (!core)
                return 0.0f;

            return (f32)(i32)core->unk_5c4;
        }

        // the basic node's seat, else the car node's
        case param_ai_car_seat: {
            ai_core* core = the_actor->get_ai_core();

            if (!core)
                return -1.0f;

            if (info_node* basic = core->get_info_node(info_node_type_ai_basic))
                return (f32)retail::sub_4E2C10((u32*)basic);

            info_node* car = core->get_info_node(info_node_type_ai_car);

            if (!car)
                return -1.0f;

            return (f32)*(i32*)((u8*)car + 0x80);
        }

        case param_ai_car_combat_engaged: {
            ai_core* core = the_actor->get_ai_core();

            if (!core)
                return 0.0f;

            info_node* car = core->get_info_node(info_node_type_ai_car);

            if (!car)
                return 0.0f;

            return retail::sub_493430((char*)car) ? 1.0f : 0.0f;
        }

        case param_ai_rappel_engaged: {
            ai_core* core = the_actor->get_ai_core();

            if (!core)
                return 0.0f;

            info_node* rappel = core->get_info_node(info_node_type_ai_rappel);

            return rappel && retail::sub_424DC0((u32*)rappel) ? 1.0f : 0.0f;
        }

        case param_ai_rappel_speed: {
            ai_core* core = the_actor->get_ai_core();

            if (!core)
                return 0.0f;

            info_node* rappel = core->get_info_node(info_node_type_ai_rappel);

            if (!rappel)
                return 0.0f;

            return *(f32*)((u8*)rappel + 0x48);
        }

        case param_random_zero_to_one:
            return (f32)retail::sub_408AC0();

        case param_random_plus_minus_one:
            return (f32)(retail::sub_408AE0(2.0f) - 1.0);

        // 1 or -1
        case param_on_fire: {
            ai_core* core = the_actor->get_ai_core();

            if (!core || !core->unk_63c)
                return -1.0f;

            return 1.0f;
        }

        case param_in_water: {
            ai_core* core = the_actor->get_ai_core();

            return core && core->unk_644 ? 1.0f : 0.0f;
        }

        case param_wading_water: {
            ai_core* core = the_actor->get_ai_core();

            return core && core->unk_654 ? 1.0f : 0.0f;
        }

        // whether the master's layer changed state this frame; -1 without a master
        case param_sync_master_changed_state: {
            animation_controller* ctrl = the_actor->anim_ctrl;

            if (!ctrl || !ctrl->has_master())
                return -1.0f;

            actor* master = ctrl->get_master()->get_actor();

            if (!master)
                return 0.0f;

            animation_logic_system_interface* master_als = master->get_als();

            if (!master_als)
                return 0.0f;

            state_machine_interface* layer = master_als->get_als_layer((layer_id)retail::sub_5AD300((i32*)ctrl));

            return layer && layer->did_transition_succeed() ? 1.0f : 0.0f;
        }

        // the same of the coordinator's layer; -1 without a coordinator
        case param_coordinator_changed_state: {
            if (!the_als->has_coordinator())
                return -1.0f;

            animation_logic_system_interface* coordinator = the_als->get_coordinator();
            state_machine_interface*          layer       = coordinator->get_als_layer((layer_id)coordinator->get_coordinator_unk());

            return layer && layer->did_transition_succeed() ? 1.0f : 0.0f;
        }

        case param_ai_cover_type: {
            ai_core* core = the_actor->get_ai_core();

            if (!core)
                return -1.0f;

            info_node* cover = core->get_info_node(info_node_type_ai_cover);

            if (!cover)
                return -1.0f;

            return (f32)retail::sub_454F20((char*)cover);
        }

        // where a conglomerate rolls against its facing, both level: 0 ahead, 1 behind; with 4 ways, 2 right and 3 left
        case param_ai_roll_type_2:
        case param_ai_roll_type_4: {
            if (!the_actor->is_a_conglomerate())
                return 0.0f;

            u32* physical = (u32*)retail::sub_5B76B0((u32*)the_actor);

            if (!physical)
                return 0.0f;

            vector3d velocity;
            retail::sub_60F130((i32)physical, (i32)&velocity); // physical_interface::get_velocity

            using get_member_abs_position_t = bool (__thiscall*)(actor*, const string_hash*, vector3d*, i32);

            // the pelvis toward the head when it has both, else the velocity
            vector3d facing = velocity;
            vector3d head;
            vector3d pelvis;

            if (((get_member_abs_position_t)retail::sub_402560)(the_actor, &references::bip01_head.get(), &head, 0) && // conglomerate::get_member_abs_position
                ((get_member_abs_position_t)retail::sub_402560)(the_actor, &references::bip01_pelvis.get(), &pelvis, 0)) {

                facing.x = head.x - pelvis.x;
                facing.z = head.z - pelvis.z;
            }

            facing.y = 0.0f;

            if (retail::sub_401FF0((f32*)&facing) > 0.0001f)
                retail::sub_402050((f32*)&facing);
            else
                facing = treyarch::references::unk_00e68098.get();

            vector3d moving = velocity;
            moving.y = 0.0f;

            if (retail::sub_401FF0((f32*)&moving) > 0.0001f)
                retail::sub_402050((f32*)&moving);
            else
                moving = facing;

            if (param == param_ai_roll_type_2)
                return moving * facing >= 0.0f ? 0.0f : 1.0f;

            const vector3d &world_up = treyarch::references::unk_00e6808c.get();
            const vector3d &zero     = treyarch::references::unk_00fd4370.get();

            math::vector4 dir(moving.x, moving.y, moving.z, 0.0f);
            math::vector4 up(world_up.x, world_up.y, world_up.z, 0.0f);
            math::vector4 origin(zero.x, zero.y, zero.z, 0.0f);

            po basis;
            retail::sub_402C60((u64*)&basis, (f32*)&dir, (f32*)&up, (i32)&origin);

            f32 along  = basis.get_z_facing() * facing;
            f32 across = basis.get_x_facing() * facing;

            if (std::fabs(along) > std::fabs(across))
                return along >= 0.0f ? 0.0f : 1.0f;

            return !(across >= 0.0f) ? 3.0f : 2.0f;
        }

        case param_is_visible:
            return retail::sub_4E1FC0((u32*)the_actor) ? 1.0f : 0.0f;

        default:
            break;
    }

    // the cases not in source yet
    using calculate_internal_float_t = f32 (__thiscall*)(internal_parameter_cache*, animation_logic_system*, state_machine*, param_id);

    return ((calculate_internal_float_t)retail::sub_5E90D0)(this, the_als, the_sm, param);
}

// sub_5AFC80
f32 als::compute_angle_between_vectors(const vector3d &forward, const vector3d &side, const vector3d &dir) {
    f32 along   = dir * forward;
    f32 across  = side * dir;
    f32 angle   = (f32)retail::sub_44D790(across, along);
    f32 degrees = (f32)(retail::sub_48DEE0(angle) * 57.29577791868205);

    if (degrees > 180.0f)
        return (f32)((f64)degrees - 360.0);

    return degrees;
}

// sub_5BC4E0
vector3d als::internal_parameter_cache::calculate_internal_vector(animation_logic_system* the_als, state_machine*, param_id param) {
    actor* the_actor = the_als->get_actor();

    switch (param) {
        case param_current_up_vector:
            return the_actor->my_abs_po->get_y_facing();

        case param_current_right_vector:
            return the_actor->my_abs_po->get_x_facing();

        case param_current_forward_vector:
            return the_actor->my_abs_po->get_z_facing();

        case param_current_velocity_vector: {
            // the physics' while they move the actor, else the body it's kept in, a player's own or the entity's
            if (the_actor->has_ifc(entity_ifc_physical)) {
                if (retail::sub_6005E0((u32*)the_actor->physical_ifc()) ||
                    the_actor->physical_ifc()->is_flagged(physical_interface_flag_biped_running) ||
                    ((the_actor->physical_ifc()->flags & 1) && !(the_actor->physical_ifc()->flags & 2) &&
                     (!retail::sub_4024E0((u32*)the_actor) || // has_standing_ifc
                      !(((standing_interface*)the_actor->my_ifc_storage->get_ifc(entity_ifc_standing))->unk_044 & 1) ||
                      !(((standing_interface*)the_actor->my_ifc_storage->get_ifc(entity_ifc_standing))->unk_044 & 2)))) {

                    vector3d velocity;
                    retail::sub_60F130((i32)the_actor->physical_ifc(), (i32)&velocity); // physical_interface::get_velocity

                    return velocity;
                }
            } else if (u32* entry = retail::sub_811DA0((u32*)the_actor)) {
                u8* body = (u8*)entry[2];

                if (!body)
                    return treyarch::references::unk_00fd4370.get();

                return *(vector3d*)(*(u8**)(body + 0xB4) + 0xD0);
            }

            if (retail::sub_402500((u32*)the_actor)) // has_player_ifc
                return *the_actor->player_ifc()->unk_095();

            vector3d velocity;
            retail::sub_614190((u32*)the_actor, (f32*)&velocity);

            return velocity;
        }

        case param_pelvis_facing:
            // the pelvis bone's z row reversed, else the forward row
            if (the_actor->is_a_conglomerate()) {
                using get_member_abs_po_t = bool (__thiscall*)(actor*, const string_hash*, po*, i32);

                po pelvis;
                retail::sub_4015B0((u64*)&pelvis);

                if (((get_member_abs_po_t)retail::sub_402590)(the_actor, &references::bip01_pelvis.get(), &pelvis, 0)) { // conglomerate::get_member_abs_po
                    const vector3d &z_row = pelvis.get_z_facing();

                    math::vector4 facing(z_row.x, z_row.y, z_row.z, 0.0f);
                    math::vector4 reversed;

                    retail::sub_401A20((f32*)&reversed, (f32*)&facing);

                    return vector3d(reversed[0], reversed[1], reversed[2]);
                }
            }

            return the_actor->my_abs_po->get_z_facing();

        case param_last_collision_normal:
            if (the_actor->has_ifc(entity_ifc_physical) && retail::sub_408F70((u32*)the_actor->physical_ifc()))
                return the_actor->physical_ifc()->last_collision_normal;

            return treyarch::references::unk_00fd4370.get();

        default:
            return treyarch::references::unk_00fd4370.get();
    }
}

// sub_5BC810
string_hash als::internal_parameter_cache::calculate_internal_hash(animation_logic_system* the_als, state_machine* the_sm, param_id param) {
    actor* the_actor = the_als->get_actor();

    // the weapon in that hand while it's drawn
    auto hand_weapon = [the_actor](i32 hand, string_hash* weapon_hash) {
        ai_core* core = the_actor->get_ai_core();

        if (!core)
            return false;

        info_node* weapons = core->get_info_node(info_node_type_ai_weapon);

        if (!weapons)
            return false;

        i32 state = retail::sub_4E7C20((u32*)weapons, hand);

        if (state != 1 && state != 2)
            return false;

        u8* weapon = (u8*)retail::sub_4E7D50((u32*)weapons, hand);

        if (!weapon)
            return false;

        *weapon_hash = *(string_hash*)(weapon + 0x20);

        return true;
    };

    string_hash result;

    switch (param) {
        case param_entity_id:
            return the_actor->id;

        case param_variant_id: {
            // the script data's Variant_ID string
            if (!retail::sub_408CF0((u32*)the_actor)) // has_script_data_ifc
                break;

            string_hash name;
            name.initialize(mash::ALLOCATED, "Variant_ID");

            const char* variant;
            the_actor->my_ifc_storage->get_ifc(entity_ifc_script_data)->get_ifc_str(name, &variant, 0);

            result.initialize(mash::ALLOCATED, variant);

            return result;
        }

        case param_als_state:
            if (the_sm->is_active() && the_sm->curr_state)
                return the_sm->curr_state->get_state_id();

            break;

        case param_als_prev_state:
            if (the_sm->is_active() && the_sm->prev_state)
                return the_sm->prev_state->get_state_id();

            break;

        case param_als_category:
            if (the_sm->is_active() && the_sm->curr_state)
                return the_sm->curr_state->get_cat_id();

            break;

        case param_als_prev_category:
            if (the_sm->is_active() && the_sm->prev_state)
                return the_sm->prev_state->get_cat_id();

            break;

        case param_coordinator_als_state:
            if (the_als->has_coordinator()) {
                animation_logic_system_interface* coordinator = the_als->get_coordinator();
                state_machine*                    layer       = (state_machine*)coordinator->get_als_layer((layer_id)coordinator->get_coordinator_unk());

                if (layer && layer->curr_state)
                    return layer->curr_state->get_state_id();
            }

            break;

        case param_coordinator_als_prev_state:
            if (the_als->has_coordinator()) {
                animation_logic_system_interface* coordinator = the_als->get_coordinator();
                state_machine*                    layer       = (state_machine*)coordinator->get_als_layer((layer_id)coordinator->get_coordinator_unk());

                if (layer && layer->prev_state)
                    return layer->prev_state->get_state_id();
            }

            break;

        case param_coordinator_als_category:
            if (the_als->has_coordinator()) {
                animation_logic_system_interface* coordinator = the_als->get_coordinator();
                state_machine*                    layer       = (state_machine*)coordinator->get_als_layer((layer_id)coordinator->get_coordinator_unk());

                if (layer && layer->curr_state)
                    return layer->curr_state->get_cat_id();
            }

            break;

        case param_coordinator_als_prev_category:
            if (the_als->has_coordinator()) {
                animation_logic_system_interface* coordinator = the_als->get_coordinator();
                state_machine*                    layer       = (state_machine*)coordinator->get_als_layer((layer_id)coordinator->get_coordinator_unk());

                if (layer && layer->prev_state)
                    return layer->prev_state->get_cat_id();
            }

            break;

        case param_r_hand_weapon:
            if (hand_weapon(0, &result))
                return result;

            break;

        case param_l_hand_weapon:
            if (hand_weapon(1, &result))
                return result;

            break;

        case param_ai_carry_type: {
            ai_core* core = the_actor->get_ai_core();

            if (!core)
                break;

            // the coordinator's actor's carry while it holds someone, else its own
            if (auto* als_node = (als_inode*)core->get_info_node(info_node_type_als)) {
                auto* node_als = (animation_logic_system_interface*)als_node->my_als;

                if (node_als->has_coordinator()) {
                    ai_core* other_core = node_als->get_coordinator()->get_actor()->get_ai_core();

                    if (other_core && other_core != core) {
                        auto* carry = (std_carry_inode*)other_core->get_info_node(info_node_type_std_carry);

                        if (carry && carry->unk_030) {
                            retail::sub_4924A0((u32*)carry, (u32*)&result);

                            return result;
                        }
                    }
                }
            }

            if (info_node* carry = core->get_info_node(info_node_type_std_carry)) {
                retail::sub_4924A0((u32*)carry, (u32*)&result);

                return result;
            }

            result.initialize(mash::ALLOCATED, nullptr, 0);

            return result;
        }

        case param_ai_car_skeleton_name: {
            // a driver's basic node's, else its car's, joined with the car's variant when it's a vehicle
            if (ai_core* core = the_actor->get_ai_core()) {
                if (info_node* basic = core->get_info_node(info_node_type_ai_basic)) {
                    retail::sub_4E2BA0((u32*)basic, (u32*)&result);

                    return result;
                }

                info_node* car_node = core->get_info_node(info_node_type_ai_car);
                auto*      car      = car_node ? (conglomerate*)((arch_base_vhandle*)((u8*)car_node + 0x7C))->resolve() : nullptr;

                if (car) {
                    if (!car->is_a_vehicle()) {
                        result.initialize(mash::ALLOCATED, nullptr, car->unk_174.source_hash_code);

                        return result;
                    }

                    string_hash variant;
                    variant.initialize(mash::ALLOCATED);

                    string_hash skeleton;
                    skeleton.initialize(mash::ALLOCATED, nullptr, car->unk_174.source_hash_code);

                    if (u8* unk = *(u8**)((u8*)car + 0x1A0))
                        variant = *(string_hash*)(unk + 0x28);

                    string_hash none;
                    none.initialize(mash::ALLOCATED);

                    // both names print into the one shared buffer, so the join repeats the skeleton's
                    if (variant != none) {
                        const char* variant_name  = variant.to_string();
                        const char* skeleton_name = skeleton.to_string();

                        alignas(mash::string) u8 storage[sizeof(mash::string)];

                        mash::string* joined = new (storage) mash::string(mash::string::fmt, "%s_%s", skeleton_name, variant_name);

                        string_hash joined_hash;
                        joined_hash.initialize(mash::ALLOCATED, joined->c_str());

                        variant = joined_hash;

                        // the temporary dies through finalize, the destructor's twin
                        joined->finalize(mash::ALLOCATED);
                    }

                    return variant;
                }
            }

            // a player's: the car behind unk_473, else the vehicle behind unk_061
            if (!retail::sub_402500((u32*)the_actor)) // has_player_ifc
                break;

            arch_base* car = the_actor->player_ifc()->unk_473().resolve();

            if (!car) {
                arch_base* other = the_actor->player_ifc()->unk_061().resolve();

                if (!other || !other->is_a_conglomerate() || !other->is_a_vehicle())
                    break;

                car = the_actor->player_ifc()->unk_061().resolve();
            }

            result.initialize(mash::ALLOCATED, nullptr, ((conglomerate*)car)->unk_174.source_hash_code);

            return result;
        }

        default:
            break;
    }

    result.initialize(mash::ALLOCATED);

    return result;
}

// sub_5AB070
f32 als::get_vector_component(const vector3d &vec, e_vector_component component) {
    switch (component) {
        case component_x:
            return vec.x;

        case component_y:
            return vec.y;

        case component_z:
            return vec.z;

        case component_norm_x: {
            vector3d norm = vec;
            return norm.normalize().x;
        }

        case component_norm_y: {
            vector3d norm = vec;
            return norm.normalize().y;
        }

        case component_norm_z: {
            vector3d norm = vec;
            return norm.normalize().z;
        }

        case component_length:
            return vec.length();

        case component_length_sq:
            return vec.length2();

        case component_xz_length:
            return vec.xz_length();

        case component_xz_length_sq:
            return vec.xz_length2();

        case component_y_length:
            return std::fabs(vec.y);

        default:
            return 0.0f;
    }
}
