#include <cmath>
#include <cstring>

#include "retail.hh"
#include "treyarch/game/als/param.hh"

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
vector3d als::internal_parameter_cache::calculate_internal_vector(animation_logic_system* the_als, state_machine* the_sm, param_id param) {
    using calculate_internal_vector_t = vector3d* (__thiscall*)(internal_parameter_cache*, vector3d*, animation_logic_system*, state_machine*, param_id);

    vector3d result;
    ((calculate_internal_vector_t)retail::sub_5BC4E0)(this, &result, the_als, the_sm, param);

    return result;
}

// sub_5BC810
string_hash als::internal_parameter_cache::calculate_internal_hash(animation_logic_system* the_als, state_machine* the_sm, param_id param) {
    using calculate_internal_hash_t = string_hash* (__thiscall*)(internal_parameter_cache*, string_hash*, animation_logic_system*, state_machine*, param_id);

    string_hash result;
    ((calculate_internal_hash_t)retail::sub_5BC810)(this, &result, the_als, the_sm, param);

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

        case component_unit_x: {
            vector3d unit = vec;
            return unit.normalize().x;
        }

        case component_unit_y: {
            vector3d unit = vec;
            return unit.normalize().y;
        }

        case component_unit_z: {
            vector3d unit = vec;
            return unit.normalize().z;
        }

        case component_length:
            return vec.length();

        case component_length2:
            return vec.length2();

        case component_xz_length:
            return vec.xz_length();

        case component_xz_length2:
            return vec.xz_length2();

        case component_abs_y:
            return std::fabs(vec.y);

        default:
            return 0.0f;
    }
}
