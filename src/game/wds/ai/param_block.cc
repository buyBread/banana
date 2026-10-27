#include "retail.hh"
#include "game/wds/ai/param_block.hh"
#include "treyarch/mash/mash_info.hh"
#include "treyarch/mm/mm.hh"

using namespace treyarch;

// sub_401750
param_block::~param_block() {
    if (params && owns_params) {
        params->~vector();

        mm::free(params);

        params = nullptr;
    }
}

// sub_443E30
void param_block::construct_mashed_class() {
    // sub_58CA60
    lock.owner = 0;
    lock.state = 0;
    lock.depth = 0;

    if (params)
        params->construct_mashed_class();

    unk_018     = 0;
    owns_params = false;
}

// sub_4425C0
void param_block::destruct_mashed_class() {
    if (params && owns_params) {
        params->~vector();

        mm::free(params);

        params = nullptr;
    }

    if (params) {
        params->destruct_mashed_class();

        params = nullptr;
    }
}

// sub_442610
void param_block::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    u8 class_mashed;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(params, this, buffer);
    else
        params = nullptr;
}

// sub_435890
bool param_block::does_parameter_exist(string_hash parameter) const {
    return ((bool (__thiscall*)(const param_block*, string_hash))retail::sub_435890)(this, parameter);
}

// sub_435910
e_param_type param_block::get_parameter_data_type(string_hash parameter) const {
    return ((e_param_type (__thiscall*)(const param_block*, string_hash))retail::sub_435910)(this, parameter);
}

// sub_434D40
f32 param_block::get_pb_float(string_hash parameter) const {
    return ((f32 (__thiscall*)(const param_block*, string_hash))retail::sub_434D40)(this, parameter);
}

// sub_434DF0
i32 param_block::get_pb_int(string_hash parameter) const {
    return ((i32 (__thiscall*)(const param_block*, string_hash))retail::sub_434DF0)(this, parameter);
}

// sub_434E90
string_hash param_block::get_pb_hash(string_hash parameter) const {
    string_hash result;
    ((string_hash* (__thiscall*)(const param_block*, string_hash*, string_hash))retail::sub_434E90)(this, &result, parameter);

    return result;
}

// sub_434F20
vector3d param_block::get_pb_vector3d(string_hash parameter) const {
    vector3d result;
    ((vector3d* (__thiscall*)(const param_block*, vector3d*, string_hash))retail::sub_434F20)(this, &result, parameter);

    return result;
}

// sub_434FF0
variance_variable<f32> param_block::get_pb_float_variance(string_hash parameter) const {
    using get_pb_float_variance_t = variance_variable<f32>* (__thiscall*)(const param_block*, variance_variable<f32>*, string_hash);

    variance_variable<f32> result;
    ((get_pb_float_variance_t)retail::sub_434FF0)(this, &result, parameter);

    return result;
}

// sub_4350B0
const char* param_block::get_pb_fixedstring(string_hash parameter) const {
    return ((const char* (__thiscall*)(const param_block*, string_hash))retail::sub_4350B0)(this, parameter);
}

// sub_435150
mash::mash_virtual_base* param_block::get_pb_class(string_hash parameter) const {
    return ((mash::mash_virtual_base* (__thiscall*)(const param_block*, string_hash))retail::sub_435150)(this, parameter);
}

// sub_435280
f32 param_block::get_optional_pb_float(string_hash parameter, f32 default_value, bool* exists) const {
    return ((f32 (__thiscall*)(const param_block*, string_hash, f32, bool*))retail::sub_435280)(this, parameter, default_value, exists);
}

// sub_435340
i32 param_block::get_optional_pb_int(string_hash parameter, i32 default_value, bool* exists) const {
    return ((i32 (__thiscall*)(const param_block*, string_hash, i32, bool*))retail::sub_435340)(this, parameter, default_value, exists);
}

// sub_435670
const char* param_block::get_optional_pb_fixedstring(string_hash parameter, const char* default_value, bool* exists) const {
    return ((const char* (__thiscall*)(const param_block*, string_hash, const char*, bool*))retail::sub_435670)(this, parameter, default_value, exists);
}

// sub_4354C0
vector3d param_block::get_optional_pb_vector3d(string_hash parameter, const vector3d &default_value, bool* exists) const {
    using get_optional_pb_vector3d_t = vector3d* (__thiscall*)(const param_block*, vector3d*, string_hash, const vector3d*, bool*);

    vector3d result;
    ((get_optional_pb_vector3d_t)retail::sub_4354C0)(this, &result, parameter, &default_value, exists);

    return result;
}

// sub_435730
mash::mash_virtual_base* param_block::get_optional_pb_class(string_hash parameter, mash::mash_virtual_base* default_value, bool* exists) const {
    return ((mash::mash_virtual_base* (__thiscall*)(const param_block*, string_hash, mash::mash_virtual_base*, bool*))retail::sub_435730)(this, parameter, default_value, exists);
}
