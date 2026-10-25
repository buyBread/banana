#include <cstring>

#include "retail.hh"
#include "treyarch/game/als/animation_logic_system.hh"
#include "treyarch/game/als/state_machine.hh"

using namespace treyarch;

// sub_5BB8B0
als::state_machine::state_machine() : shared_portion(nullptr),
                                      curr_state(nullptr),
                                      curr_category(nullptr),
                                      prev_state(nullptr),
                                      prev_category(nullptr),
                                      curr_events(nullptr),
                                      curr_hit_pause_events(nullptr),
                                      curr_blends(nullptr),
                                      unk_794(),
                                      unk_79c(0),
                                      search_cache_state(nullptr),
                                      search_cache_category(nullptr),
                                      the_controller(nullptr) {

    our_ret.did_do_trans                = false;
    our_ret.trans_from_force_or_request = false;
    our_ret.is_still_fulfilling         = false;
    our_ret.is_interruptable            = false;
    our_ret.is_active                   = false;
    our_ret.unk_19                      = false;

    curr_req_data.did_transition_occur = false;
    curr_req_data.is_trans_to_category = false;
    curr_req_data.ignore_no_transition = false;
    curr_req_data.unk_03               = false;
    curr_req_data.unk_04               = false;

    our_smi.requested_params        = &parameter_buffers[0];
    pending_requests.pending_params = &parameter_buffers[1];

    our_smi.is_set_kill         = false;
    our_smi.is_request_or_force = false;
    our_smi.is_force_state      = false;
    our_smi.unk_0b              = false;
    our_smi.request_id          = string_hash();

    std::memset(our_smi.requested_params, 0, sizeof(parameter_list));

    pending_requests.pending_cat.initialize(mash::ALLOCATED);

    unk_7a0[0] = -1;
    unk_7a0[1] = -1;
    unk_7a0[2] = -1;
    unk_7a0[3] = -1;
    unk_7bc    = false;
}

// sub_5B0030
als::state_machine::~state_machine() {}

// sub_5AFD00
als::category_id als::state_machine::get_category_id() const {
    if (is_active())
        return curr_state->get_cat_id();

    string_hash none;
    none.initialize(mash::ALLOCATED);

    return none;
}

// sub_5AFD40
als::state_id als::state_machine::get_state_id() const {
    if (is_active())
        return curr_state->get_state_id();

    string_hash none;
    none.initialize(mash::ALLOCATED);

    return none;
}

// sub_5AFD80
void als::state_machine::set_desired_param(external_float_parameter_types id, f32 data) {
    our_smi.requested_params->float_params[id] = data;
    our_smi.requested_params->float_set[id]    = true;
}

// sub_5AFDB0
void als::state_machine::set_desired_param(external_vector_parameter_types id, const vector3d &data) {
    u32 index = id - num_external_float_params;

    our_smi.requested_params->vector_params[index] = data;
    our_smi.requested_params->vector_set[index]    = true;
}

// sub_5AFDF0
void als::state_machine::set_desired_param(external_hash_parameter_types id, string_hash data) {
    u32 index = id - num_external_vector_params;

    our_smi.requested_params->hash_params[index] = data;
    our_smi.requested_params->hash_set[index]    = true;
}

// sub_5AFE50
void als::state_machine::request_category_transition(category_id                       requested_category,
                                                     animation_logic_system_interface* my_als,
                                                     bool                              unk) {

    if (our_smi.is_force_state) // a forced state wins over a category request
        return;

    our_smi.is_request_or_force = true;
    our_smi.request_id          = requested_category;
    our_smi.unk_0b              = unk;
    our_smi.is_set_kill         = false;

    my_als->suspend_logic_system(false);
}

// sub_5AFE80
bool als::state_machine::is_interruptable() const {
    return our_ret.is_interruptable || !our_ret.is_active;
}

// sub_5AFEA0
bool als::state_machine::did_transition_succeed() const {
    return our_ret.did_do_trans;
}

// sub_5AFEB0
bool als::state_machine::did_transition_succeed_from_request() const {
    return our_ret.did_do_trans && our_ret.trans_from_force_or_request;
}

// sub_5AFED0
bool als::state_machine::is_request_satisfied() const {
    return !our_ret.is_still_fulfilling;
}

// sub_6F6720
bool als::state_machine::is_active() const {
    return our_ret.is_active;
}

// sub_5AFEE0
void als::state_machine::force_als_state(state_id destination_state, only_if_you_know_what_you_are_doing) {
    our_smi.is_request_or_force = true;
    our_smi.is_force_state      = true;
    our_smi.request_id          = destination_state;
}

// sub_5BB5A0
void als::state_machine::set_speed(f32 speed) {
    ((void (__thiscall*)(state_machine*, f32))retail::sub_5BB5A0)(this, speed);
}

// sub_5BB600
void als::state_machine::set_secondary_speed(f32 speed) {
    ((void (__thiscall*)(state_machine*, f32))retail::sub_5BB600)(this, speed);
}

// sub_5BB660
f32 als::state_machine::get_speed() const {
    return ((f32 (__thiscall*)(const state_machine*))retail::sub_5BB660)(this);
}

// sub_5BB6C0
f32 als::state_machine::get_secondary_speed() const {
    return ((f32 (__thiscall*)(const state_machine*))retail::sub_5BB6C0)(this);
}

// sub_5AFF00
void als::state_machine::kill_layer() {
    our_smi.is_set_kill = true;
}

// sub_5D5170
bool als::state_machine::does_category_exist(category_id the_cat_id) const {
    return find_category(the_cat_id, false) != nullptr;
}

// sub_5E8B80
f32 als::state_machine::get_pb_float(string_hash parameter) const {
    return ((f32 (__thiscall*)(const state_machine*, string_hash))retail::sub_5E8B80)(this, parameter);
}

// sub_5E8BC0
i32 als::state_machine::get_pb_int(string_hash parameter) const {
    return ((i32 (__thiscall*)(const state_machine*, string_hash))retail::sub_5E8BC0)(this, parameter);
}

// sub_5E8BF0
string_hash als::state_machine::get_pb_hash(string_hash parameter) const {
    string_hash result;
    ((string_hash* (__thiscall*)(const state_machine*, string_hash*, string_hash))retail::sub_5E8BF0)(this, &result, parameter);

    return result;
}

// sub_5E8CB0
const char* als::state_machine::get_pb_fixedstring(string_hash parameter) const {
    return ((const char* (__thiscall*)(const state_machine*, string_hash))retail::sub_5E8CB0)(this, parameter);
}

// sub_5E8C30
vector3d als::state_machine::get_pb_vector3d(string_hash parameter) const {
    vector3d result;
    ((vector3d* (__thiscall*)(const state_machine*, vector3d*, string_hash))retail::sub_5E8C30)(this, &result, parameter);

    return result;
}

// sub_5E8C70
variance_variable<f32> als::state_machine::get_pb_float_variance(string_hash parameter) const {
    using get_pb_float_variance_t = variance_variable<f32>* (__thiscall*)(const state_machine*, variance_variable<f32>*, string_hash);

    variance_variable<f32> result;
    ((get_pb_float_variance_t)retail::sub_5E8C70)(this, &result, parameter);

    return result;
}

// sub_5E8CB0
const char* als::state_machine::get_pb_mstring(string_hash parameter) const {
    return ((const char* (__thiscall*)(const state_machine*, string_hash))retail::sub_5E8CB0)(this, parameter);
}

// sub_5E8CE0
mash::mash_virtual_base* als::state_machine::get_pb_class(string_hash parameter) const {
    return ((mash::mash_virtual_base* (__thiscall*)(const state_machine*, string_hash))retail::sub_5E8CE0)(this, parameter);
}

// sub_5E8D10
f32 als::state_machine::get_optional_pb_float(      string_hash  parameter,
                                              const f32         &default_value,
                                                    bool*        exists) const {

    using get_optional_pb_float_t = f32 (__thiscall*)(const state_machine*, string_hash, const f32*, bool*);

    return ((get_optional_pb_float_t)retail::sub_5E8D10)(this, parameter, &default_value, exists);
}

// sub_5E8D70
i32 als::state_machine::get_optional_pb_int(      string_hash  parameter,
                                            const i32         &default_value,
                                                  bool*        exists) const {

    using get_optional_pb_int_t = i32 (__thiscall*)(const state_machine*, string_hash, const i32*, bool*);

    return ((get_optional_pb_int_t)retail::sub_5E8D70)(this, parameter, &default_value, exists);
}

// sub_5E8DC0
string_hash als::state_machine::get_optional_pb_hash(string_hash parameter, string_hash default_value, bool* exists) const {
    using get_optional_pb_hash_t = string_hash* (__thiscall*)(const state_machine*, string_hash*, string_hash, string_hash, bool*);

    string_hash result;
    ((get_optional_pb_hash_t)retail::sub_5E8DC0)(this, &result, parameter, default_value, exists);

    return result;
}

// sub_5E8E20
const char* als::state_machine::get_optional_pb_fixedstring(string_hash parameter, const char* default_value, bool* exists) const {
    using get_optional_pb_string_t = const char* (__thiscall*)(const state_machine*, string_hash, const char*, bool*);

    return ((get_optional_pb_string_t)retail::sub_5E8E20)(this, parameter, default_value, exists);
}

// sub_5E8E70
vector3d als::state_machine::get_optional_pb_vector3d(      string_hash  parameter,
                                                      const vector3d    &default_value,
                                                            bool*        exists) const {

    using get_optional_pb_vector3d_t = vector3d* (__thiscall*)(const state_machine*, vector3d*, string_hash, const vector3d*, bool*);

    vector3d result;
    ((get_optional_pb_vector3d_t)retail::sub_5E8E70)(this, &result, parameter, &default_value, exists);

    return result;
}

// sub_5E8ED0
variance_variable<f32> als::state_machine::get_optional_pb_float_variance(      string_hash             parameter,
                                                                          const variance_variable<f32> &default_value,
                                                                                bool*                   exists) const {

    using get_optional_pb_float_variance_t = variance_variable<f32>* (__thiscall*)(const state_machine*,
                                                                                   variance_variable<f32>*,
                                                                                   string_hash,
                                                                                   const variance_variable<f32>*,
                                                                                   bool*);

    variance_variable<f32> result;
    ((get_optional_pb_float_variance_t)retail::sub_5E8ED0)(this, &result, parameter, &default_value, exists);

    return result;
}

// sub_5E8E20
const char* als::state_machine::get_optional_pb_mstring(string_hash parameter, const char* default_value, bool* exists) const {
    using get_optional_pb_string_t = const char* (__thiscall*)(const state_machine*, string_hash, const char*, bool*);

    return ((get_optional_pb_string_t)retail::sub_5E8E20)(this, parameter, default_value, exists);
}

// sub_5E8F30
mash::mash_virtual_base* als::state_machine::get_optional_pb_class(string_hash               parameter,
                                                                   mash::mash_virtual_base*  default_value,
                                                                   bool*                     exists) const {

    using get_optional_pb_class_t = mash::mash_virtual_base* (__thiscall*)(const state_machine*,
                                                                           string_hash,
                                                                           mash::mash_virtual_base*,
                                                                           bool*);

    return ((get_optional_pb_class_t)retail::sub_5E8F30)(this, parameter, default_value, exists);
}

// sub_5E8F80
bool als::state_machine::does_parameter_exist(string_hash parameter) const {
    return ((bool (__thiscall*)(const state_machine*, string_hash))retail::sub_5E8F80)(this, parameter);
}

// sub_5E8FA0
u32 als::state_machine::get_parameter_data_type(string_hash parameter) const {
    return ((u32 (__thiscall*)(const state_machine*, string_hash))retail::sub_5E8FA0)(this, parameter);
}

// sub_5D5190
f32 als::state_machine::get_time_to_end_of_anim(bool allow_looping, bool unk) const {
    return ((f32 (__thiscall*)(const state_machine*, bool, bool))retail::sub_5D5190)(this, allow_looping, unk);
}

// sub_5D57F0
f32 als::state_machine::get_time_to_signal(string_hash signal_name, bool* signal_exists, bool unk, u32* unk2) const {
    using get_time_to_signal_t = f32 (__thiscall*)(const state_machine*, string_hash, bool*, bool, u32*);

    return ((get_time_to_signal_t)retail::sub_5D57F0)(this, signal_name, signal_exists, unk, unk2);
}

// sub_5D59A0
f32 als::state_machine::unk_039(string_hash signal_name, bool* signal_exists, bool unk, u32* unk2) const {
    using get_time_to_signal_t = f32 (__thiscall*)(const state_machine*, string_hash, bool*, bool, u32*);

    return ((get_time_to_signal_t)retail::sub_5D59A0)(this, signal_name, signal_exists, unk, unk2);
}

// sub_5BB720
f32 als::state_machine::get_duration() const {
    return ((f32 (__thiscall*)(const state_machine*))retail::sub_5BB720)(this);
}

// sub_5BB740
f32 als::state_machine::get_current_normalized_time() const {
    return ((f32 (__thiscall*)(const state_machine*))retail::sub_5BB740)(this);
}

// sub_5BB790
f32 als::state_machine::get_current_anim_time() const {
    return ((f32 (__thiscall*)(const state_machine*))retail::sub_5BB790)(this);
}

// sub_5AFF10
bool als::state_machine::is_cat_our_prev_cat(category_id test_category) const {
    if (!our_ret.did_do_trans)
        return false;

    return test_category == our_ret.prev_cat && test_category != get_category_id();
}

// sub_5AFE10
bool als::state_machine::is_requesting_category(category_id requested_category) const {
    if (!our_smi.is_request_or_force)
        return false;

    requested_category.initialize(mash::ALLOCATED, nullptr, requested_category.source_hash_code);

    return our_smi.request_id == requested_category;
}

// sub_667F80
bool als::state_machine::unk_045() const {
    return true;
}

// sub_5B00E0
u32 als::state_machine::get_num_unk_44() const {
    return curr_state->unk_44.size;
}

// sub_5BC4D0
als::state_unk_44* als::state_machine::get_unk_44(u32 index) const {
    return curr_state->unk_44.data[index];
}

// sub_5E0980
void* als::state_machine::unk_048(bool unk, entity* the_entity) const {
    if (!curr_state)
        return nullptr;

    return ((void* (__thiscall*)(const state*, bool, entity*))retail::sub_5D4760)(curr_state, unk, the_entity);
}

// sub_5D5B50
mash::string als::state_machine::unk_049(string_hash signal_name, bool in_frames) const {
    using unk_049_t = mash::string* (__thiscall*)(const state_machine*, void*, string_hash, bool);

    alignas(mash::string) u8 slot[sizeof(mash::string)];
    mash::string* built = ((unk_049_t)retail::sub_5D5B50)(this, slot, signal_name, in_frames);

    mash::string result(*built);
    built->~string();

    return result;
}

// sub_5AFF50
mash::string als::state_machine::unk_050() const {
    alignas(mash::string) u8 slot[sizeof(mash::string)];
    mash::string* built = ((mash::string* (__thiscall*)(const state_machine*, void*))retail::sub_5AFF50)(this, slot);

    mash::string result(*built);
    built->~string();

    return result;
}

// sub_5BB810
bool als::state_machine::unk_051() const {
    return ((bool (__thiscall*)(const state_machine*))retail::sub_5BB810)(this);
}

// sub_5BA880
f32 als::state_machine::get_playback_speed() const {
    return ((f32 (__thiscall*)(const state_machine*))retail::sub_5BA880)(this);
}

// sub_5BAAD0
f32 als::state_machine::get_start_time() const {
    return ((f32 (__thiscall*)(const state_machine*))retail::sub_5BAAD0)(this);
}

// sub_5BBAB0
const als::category* als::state_machine::find_category(category_id the_id, bool error_on_not_found) const {
    return ((const category* (__thiscall*)(const state_machine*, category_id, bool))retail::sub_5BBAB0)(this, the_id, error_on_not_found);
}

// sub_5BBBF0
void als::state_machine::kill_als_state_script(animation_logic_system* the_als, bool unk) {
    ((void (__thiscall*)(state_machine*, animation_logic_system*, bool))retail::sub_5BBBF0)(this, the_als, unk);
}

// inlined @ sub_5BBD30
// inlined @ sub_5BBCA0
void als::state_machine::reset(state_machine_shared* shared_machine) {
    shared_portion = shared_machine;

    curr_state            = nullptr;
    curr_category         = nullptr;
    prev_state            = nullptr;
    prev_category         = nullptr;
    search_cache_state    = nullptr;
    search_cache_category = nullptr;

    our_smi.requested_params        = &parameter_buffers[0];
    pending_requests.pending_params = &parameter_buffers[1];

    our_smi.is_set_kill         = false;
    our_smi.is_request_or_force = false;
    our_smi.is_force_state      = false;
    our_smi.unk_0b              = false;
    our_smi.request_id          = string_hash();

    std::memset(our_smi.requested_params, 0, sizeof(parameter_list));

    pending_requests.pending_cat.initialize(mash::ALLOCATED);
}

// sub_A944D0
als::layer_id als::base_state_machine::get_layer_id() const {
    return base_layer;
}

// sub_823B70
bool als::base_state_machine::determine_if_request_satisfied(animation_logic_system*) const {
    return true;
}

// sub_5BBD30
void als::base_state_machine::init(animation_logic_system* the_als, state_machine_shared* shared_machine) {
    reset(shared_machine);

    for (state* candidate : shared_portion->state_list) {
        if (candidate->flags & state::initial_state) {
            curr_state = candidate;

            break;
        }
    }

    curr_category = curr_state ? find_category(curr_state->get_cat_id(), true) : nullptr;

    our_ret.is_active        = true;
    our_ret.is_interruptable = !(curr_state->flags & state::uninterruptable);

    the_controller = the_als->get_controller();
}

// sub_5B00B0
als::layer_id als::layer_state_machine::get_layer_id() const {
    return shared_portion->get_layer_id();
}

// sub_5B0090
bool als::layer_state_machine::determine_if_request_satisfied(animation_logic_system*) const {
    return our_ret.is_active && our_ret.did_do_trans;
}

// sub_5B00C0
void als::layer_state_machine::set_layer_id(layer_id new_id) {
    ((layer_state_machine_shared*)shared_portion)->set_layer_id(new_id);
}

// sub_5BBCA0
void als::layer_state_machine::init(animation_logic_system* the_als, layer_state_machine_shared* shared_machine) {
    reset(shared_machine);

    our_ret.is_active = false;

    the_controller = the_als->get_controller();
}
