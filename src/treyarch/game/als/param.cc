#include <cmath>
#include <cstring>
#include <new>

#include "retail.hh"
#include "treyarch/game/als/animation_logic_system.hh"
#include "treyarch/game/als/internal.hh"
#include "treyarch/game/als/param.hh"
#include "treyarch/game/als/state_machine.hh"
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
    using calculate_internal_float_t = f32 (__thiscall*)(internal_parameter_cache*, animation_logic_system*, state_machine*, param_id);

    return ((calculate_internal_float_t)retail::sub_5E90D0)(this, the_als, the_sm, param);
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
