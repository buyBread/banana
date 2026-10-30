#include <cstring>
#include <new>
#include <optional>

#include "retail.hh"
#include "game/amalga/resource_manager.hh"
#include "game/math/references.hh"
#include "game/wds/ai/param_block.hh"
#include "game/wds/entity/actor.hh"
#include "treyarch/mash/mash_info.hh"
#include "treyarch/mm/mm.hh"

using namespace treyarch;

string_hash param_base::my_type_key           { 0x4D18EE8B }; // "param_base"
string_hash param_float::my_type_key          { 0xF0851906 }; // "param_float"
string_hash param_int::my_type_key            { 0xAD00E0DB }; // "param_int"
string_hash param_hash::my_type_key           { 0x4D1C38D4 }; // "param_hash"
string_hash param_vector3d::my_type_key       { 0x7ABC801A }; // "param_vector3d"
string_hash param_float_variance::my_type_key { 0xD376EA6E }; // "param_float_variance"
string_hash param_string::my_type_key         { 0x200DD4C7 }; // "param_string"
string_hash param_class::my_type_key          { 0xF04E9646 }; // "param_class"
string_hash param_entity::my_type_key         { 0xFEFA228D }; // "param_entity"
string_hash param_pointer::my_type_key        { 0x2E8FE751 }; // "param_pointer"

// ==== param_base ==== //

// sub_42DD80
param_base::param_base(const string_hash &name) : my_name(name) {}

// sub_5B04C0
void param_base::construct_mashed_class() {
    my_name.construct_mashed_class();
}

// sub_5B04C0
void param_base::destruct_mashed_class() {
    my_name.destruct_mashed_class();
}

// sub_5E2F30
void param_base::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    my_name.unmash(mash_info, this, buffer);
}

// sub_42AB50
mash::virtual_types_key param_base::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_718FB0
bool param_base::is_subclass_of(mash::virtual_types_key parent_class) const {
    return mash::mash_virtual_base::get_virtual_type_key() == parent_class;
}

// sub_5D7970
i32 param_base::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_720050
f32 param_base::get_data_float() const {
    return 0.0f;
}

// sub_A944D0
i32 param_base::get_data_int() const {
    return 0;
}

// sub_42ADA0
string_hash param_base::get_data_hash() const {
    return string_hash();
}

// sub_42ADC0
const vector3d &param_base::get_data_vector3d() const {
    return references::unk_00fc3440.get();
}

// sub_435980
const variance_variable<f32> &param_base::get_data_float_variance() const {
    static const variance_variable<f32> none = { 0.0f, 0.0f };

    return none;
}

// sub_42E630
const mash::string &param_base::get_data_fixedstring() const {
    static const mash::string empty("");

    return empty;
}

// sub_A944D0
mash::mash_virtual_base* param_base::get_data_class() const {
    return nullptr;
}

// sub_42E690
const arch_base_vhandle &param_base::get_data_entity() const {
    static const arch_base_vhandle none;

    return none;
}

// sub_A944D0
void* param_base::get_data_pointer() const {
    return nullptr;
}

// sub_689610
void param_base::set_data_float(f32) {}

// sub_689610
void param_base::set_data_int(i32) {}

// sub_689610
void param_base::set_data_hash(string_hash) {}

// sub_689610
void param_base::set_data_vector3d(const vector3d&) {}

// sub_689610
void param_base::set_data_float_variance(const variance_variable<f32>&) {}

// sub_689610
void param_base::set_data_fixedstring(const mash::string&) {}

// sub_689610
void param_base::set_data_class(const mash::mash_virtual_base*) {}

// sub_689610
void param_base::set_data_class(mash::virtual_types_key) {}

// sub_689610
void param_base::set_data_entity(const arch_base_vhandle&) {}

// sub_689610
void param_base::set_data_pointer(const void*) {}

// ==== param_float ==== //

// inlined @ sub_43E650
// inlined @ sub_42DEF0
param_float::param_float(const string_hash &name, f32 value) : param_base(name),
                                                               value(value) {

    my_type = param_type_float;
}

// sub_42DEE0
mash::virtual_types_key param_float::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_42E160
bool param_float::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (param_base::get_virtual_type_key() == parent_class)
        return true;

    if (param_base::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 param_float::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_42DED0
void param_float::copy_data(const param_base* other) {
    value = ((const param_float*)other)->value;
}

// sub_42DEF0
param_base* param_float::clone() const {
    return new param_float(my_name, value);
}

// sub_90D1B0
f32 param_float::get_data_float() const {
    return value;
}

// sub_68B970
void param_float::set_data_float(f32 data) {
    value = data;
}

// sub_42DE90
bool param_float::deep_compare(const param_base* other) const {
    if (other->my_type != param_type_float)
        return false;

    return value == ((const param_float*)other)->value;
}

// ==== param_int ==== //

// inlined @ sub_43E6E0
// inlined @ sub_42DE20
param_int::param_int(const string_hash &name, i32 value) : param_base(name),
                                                           value(value) {

    my_type = param_type_int;
}

// sub_42DE10
mash::virtual_types_key param_int::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_42E160
bool param_int::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (param_base::get_virtual_type_key() == parent_class)
        return true;

    if (param_base::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 param_int::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_42DE00
void param_int::copy_data(const param_base* other) {
    value = ((const param_int*)other)->value;
}

// sub_42DE20
param_base* param_int::clone() const {
    return new param_int(my_name, value);
}

// sub_5AF170
i32 param_int::get_data_int() const {
    return value;
}

// sub_A6E470
void param_int::set_data_int(i32 data) {
    value = data;
}

// sub_42DDE0
bool param_int::deep_compare(const param_base* other) const {
    if (other->my_type != param_type_int)
        return false;

    return value == ((const param_int*)other)->value;
}

// ==== param_hash ==== //

// sub_42DF80
param_hash::param_hash(const string_hash &name, string_hash value) : param_base(name),
                                                                     value(value) {

    my_type = param_type_hash;
}

// sub_5B0490
void param_hash::construct_mashed_class() {
    my_name.construct_mashed_class();
    value.construct_mashed_class();
}

// sub_42ADD0
void param_hash::destruct_mashed_class() {
    value.destruct_mashed_class();
    my_name.destruct_mashed_class();
}

// sub_5E2F40
void param_hash::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    my_name.unmash(mash_info, this, buffer);
    value.unmash(mash_info, this, buffer);
}

// sub_42E010
mash::virtual_types_key param_hash::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_42E160
bool param_hash::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (param_base::get_virtual_type_key() == parent_class)
        return true;

    if (param_base::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 param_hash::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_42DE00
void param_hash::copy_data(const param_base* other) {
    value = ((const param_hash*)other)->value;
}

// sub_42E020
param_base* param_hash::clone() const {
    return new param_hash(my_name, value);
}

// sub_905E30
string_hash param_hash::get_data_hash() const {
    return value;
}

// sub_A6E470
void param_hash::set_data_hash(string_hash data) {
    value = data;
}

// sub_42DFF0
bool param_hash::deep_compare(const param_base* other) const {
    if (other->my_type != param_type_hash)
        return false;

    return value == ((const param_hash*)other)->value;
}

// ==== param_vector3d ==== //

// inlined @ sub_43E7C0
// inlined @ sub_42E190
param_vector3d::param_vector3d(const string_hash &name, const vector3d &value) : param_base(name),
                                                                                 value(value) {

    my_type = param_type_vector3d;
}

// sub_42E150
mash::virtual_types_key param_vector3d::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_42E160
bool param_vector3d::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (param_base::get_virtual_type_key() == parent_class)
        return true;

    if (param_base::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_71D790
i32 param_vector3d::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_42E130
void param_vector3d::copy_data(const param_base* other) {
    value = ((const param_vector3d*)other)->value;
}

// sub_42E190
param_base* param_vector3d::clone() const {
    return new param_vector3d(my_name, value);
}

// sub_434A50
const vector3d &param_vector3d::get_data_vector3d() const {
    return value;
}

// sub_42E090
void param_vector3d::set_data_vector3d(const vector3d &data) {
    value = data;
}

// sub_42E0B0
bool param_vector3d::deep_compare(const param_base* other) const {
    if (other->my_type != param_type_vector3d)
        return false;

    const vector3d &theirs = ((const param_vector3d*)other)->value;

    return value.x == theirs.x &&
           value.y == theirs.y &&
           value.z == theirs.z;
}

// ==== param_float_variance ==== //

// inlined @ sub_43E860
// inlined @ sub_434BD0
param_float_variance::param_float_variance(const string_hash            &name,
                                           const variance_variable<f32> &value) : param_base(name),
                                                                                  value(value) {

    my_type = param_type_float_variance;
}

// sub_434A60
mash::virtual_types_key param_float_variance::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_42E160
bool param_float_variance::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (param_base::get_virtual_type_key() == parent_class)
        return true;

    if (param_base::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D8160
i32 param_float_variance::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_434BB0
void param_float_variance::copy_data(const param_base* other) {
    value = ((const param_float_variance*)other)->value;
}

// sub_434BD0
param_base* param_float_variance::clone() const {
    return new param_float_variance(my_name, value);
}

// sub_434A50
const variance_variable<f32> &param_float_variance::get_data_float_variance() const {
    return value;
}

// sub_434AB0
void param_float_variance::set_data_float_variance(const variance_variable<f32> &data) {
    value = data;
}

// sub_434AD0
bool param_float_variance::deep_compare(const param_base* other) const {
    if (other->my_type != param_type_float_variance)
        return false;

    const f32 mine = value.get_random_value_inline();

    return mine == ((const param_float_variance*)other)->value.get_random_value_inline();
}

// ==== param_string ==== //

// sub_42E210
param_string::param_string(const string_hash &name, const mash::string &value) : param_base(name),
                                                                                 value() {

    my_type     = param_type_string;
    this->value = value;
}

// sub_42ADF0
void param_string::construct_mashed_class() {
    my_name.construct_mashed_class();
    value.construct_mashed_class();
}

// sub_42AE10
void param_string::destruct_mashed_class() {
    value.destruct_mashed_class();
    my_name.destruct_mashed_class();
}

// sub_439AF0
void param_string::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    my_name.unmash(mash_info, this, buffer);
    value.unmash(mash_info, this, buffer);
}

// sub_42AB90
mash::virtual_types_key param_string::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_42E160
bool param_string::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (param_base::get_virtual_type_key() == parent_class)
        return true;

    if (param_base::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_71D790
i32 param_string::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_42AB70
void param_string::copy_data(const param_base* other) {
    value = ((const param_string*)other)->value;
}

// sub_42E2B0
param_base* param_string::clone() const {
    return new param_string(my_name, value);
}

// sub_434A50
const mash::string &param_string::get_data_fixedstring() const {
    return value;
}

// sub_42AB60
void param_string::set_data_fixedstring(const mash::string &data) {
    value = data;
}

// sub_42E280
bool param_string::deep_compare(const param_base* other) const {
    if (other->my_type != param_type_string)
        return false;

    return !std::strncmp(value.c_str(),
                         ((const param_string*)other)->value.c_str(),
                         mash::string::max_length);
}

// ==== param_class ==== //

param_block_creatable_class* param_block_creatable_class::create_subclass(mash::virtual_types_key the_class_to_make) {
    return (param_block_creatable_class*)retail::sub_42AED0((i32)the_class_to_make);
}

// sub_434C50
param_class::param_class(const string_hash &name, const mash::mash_virtual_base* object) : param_base(name),
                                                                                          value(nullptr) {

    my_type = param_type_class;

    value = param_block_creatable_class::create_subclass(object->get_virtual_type_key());
    value->copy_values(object);
}

// sub_42E320
param_class::~param_class() {
    delete value;
}

// sub_439B20
void param_class::construct_mashed_class() {
    my_name.construct_mashed_class();

    if (value)
        value = mash::mash_virtual_base::construct_class_helper(value);
}

// sub_42AE30
void param_class::destruct_mashed_class() {
    if (value) {
        // an actor tears down inside the pack it came from
        amalga::resource_pack_slot* slot = value->is_or_is_subclass_of(actor::my_type_key.source_hash_code) ?
            ((actor*)value)->pack_slot : nullptr;

        std::optional<amalga::resource_context_stack_object> context;

        if (slot)
            context.emplace(slot);

        value->destruct_mashed_class();
    }

    // and once more, outside it
    if (value) {
        value->destruct_mashed_class();

        value = nullptr;
    }

    my_name.destruct_mashed_class();
}

// sub_439B50
void param_class::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    my_name.unmash(mash_info, this, buffer);

    u8 class_mashed;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(value, this, buffer);
    else
        value = nullptr;
}

// sub_42E390
mash::virtual_types_key param_class::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_42E160
bool param_class::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (param_base::get_virtual_type_key() == parent_class)
        return true;

    if (param_base::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 param_class::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_42DE00
void param_class::copy_data(const param_base* other) { // shares the other's object
    value = ((const param_class*)other)->value;
}

// sub_434CD0
param_base* param_class::clone() const {
    return new param_class(my_name, value);
}

// sub_5AF170
mash::mash_virtual_base* param_class::get_data_class() const {
    return value;
}

// sub_42E6C0
void param_class::set_data_class(const mash::mash_virtual_base* object) {
    if (!value)
        value = param_block_creatable_class::create_subclass(object->get_virtual_type_key());

    value->copy_values(object);
}

// sub_42E700
void param_class::set_data_class(mash::virtual_types_key the_class_to_make) { // the old object isn't freed
    value = param_block_creatable_class::create_subclass(the_class_to_make);
}

// sub_42E370
bool param_class::deep_compare(const param_base* other) const {
    if (other->my_type != param_type_class)
        return false;

    return value == ((const param_class*)other)->value;
}

// ==== param_entity ==== //

// inlined @ sub_43E990
// inlined @ sub_42E400
param_entity::param_entity(const string_hash &name, arch_base_vhandle value) : param_base(name),
                                                                               value(value) {

    my_type = param_type_entity;
}

// sub_42E3D0
mash::virtual_types_key param_entity::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_42E160
bool param_entity::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (param_base::get_virtual_type_key() == parent_class)
        return true;

    if (param_base::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 param_entity::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_42DE00
void param_entity::copy_data(const param_base* other) {
    value = ((const param_entity*)other)->value;
}

// sub_42E400
param_base* param_entity::clone() const {
    return new param_entity(my_name, value);
}

// sub_434A50
const arch_base_vhandle &param_entity::get_data_entity() const {
    return value;
}

// sub_42E3C0
void param_entity::set_data_entity(const arch_base_vhandle &data) {
    value = data;
}

// sub_42E3E0
bool param_entity::deep_compare(const param_base* other) const {
    if (other->my_type != param_type_entity)
        return false;

    return value.value == ((const param_entity*)other)->value.value;
}

// ==== param_pointer ==== //

// inlined @ sub_42E4C0
param_pointer::param_pointer(const string_hash &name, void* value) : param_base(name),
                                                                     value(value) {

    my_type = param_type_pointer;
}

// sub_42E4B0
mash::virtual_types_key param_pointer::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_42E160
bool param_pointer::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (param_base::get_virtual_type_key() == parent_class)
        return true;

    if (param_base::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 param_pointer::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_42DE00
void param_pointer::copy_data(const param_base* other) {
    value = ((const param_pointer*)other)->value;
}

// sub_42E4C0
param_base* param_pointer::clone() const {
    return new param_pointer(my_name, value);
}

// sub_5AF170
void* param_pointer::get_data_pointer() const {
    return value;
}

// sub_A6E470
void param_pointer::set_data_pointer(const void* data) {
    value = (void*)data;
}

// sub_42E490
bool param_pointer::deep_compare(const param_base* other) const {
    if (other->my_type != param_type_pointer)
        return false;

    return value == ((const param_pointer*)other)->value;
}

// ==== param_block ==== //

// sub_401750
param_block::~param_block() {
    if (param_array && param_array_is_dynamic) {
        param_array->~vector();

        mm::free(param_array);

        param_array = nullptr;
    }
}

// sub_42E580
param_base* param_block::common_find_data(string_hash parameter) const {
    if (cached_param && cached_param->get_name() == parameter)
        return cached_param;

    if (!param_array)
        return nullptr;

    const i32 count = (i32)param_array->size;

    if (count <= 0)
        return nullptr;

    // the first probe is the middle of the whole list
    i32 low    = 0;
    i32 high   = count;
    i32 middle = count >> 1;

    do {
        const string_hash name = param_array->array[middle]->get_name();

        if (parameter < name)
            high = middle;
        else if (parameter == name)
            return cached_param = param_array->array[middle];
        else
            low = middle + 1;

        middle = (low + high) >> 1;
    } while (low < high);

    return nullptr;
}

// inlined @ sub_43E3F0
// inlined @ sub_43E5B0
void param_block::allocate_param_array() {
    void* memory = mm::allocate(sizeof(mash::vector<param_base>));

    param_array            = memory ? new (memory) mash::vector<param_base>() : nullptr;
    param_array_is_dynamic = true;
}

// sub_43E3F0
void param_block::add_param(const param_base*                   param,
                            only_if_you_know_what_you_are_doing ignore_param_search_and_sort) {

    engine_lock_scope scope(&lock);

    if (ignore_param_search_and_sort != i_know_what_i_am_doing) {
        if (param_base* found = common_find_data(param->get_name())) {
            found->copy_data(param);

            return;
        }
    }

    param_base* copy = param->clone();

    if (!param_array)
        allocate_param_array();

    if (ignore_param_search_and_sort != i_know_what_i_am_doing) {
        const i32 count = (i32)param_array->size;

        // only a name below the last one searches for its place
        if (count > 0 && param_array->array[count - 1]->my_name.source_hash_code > copy->my_name.source_hash_code) {
            const u32 name = copy->my_name.source_hash_code;

            i32 low    = 0;
            i32 middle = 0;
            i32 high   = count;

            do {
                middle = (high + low) >> 1;

                const u32 probe = param_array->array[middle]->my_name.source_hash_code;

                if (name < probe)
                    high = middle;
                else if (name > probe)
                    low = middle + 1;
            } while (low < high);

            if (param_array->array[middle]->my_name.source_hash_code < name)
                ++middle;

            param_array->push_back(copy);

            if ((u32)middle < (u32)count) {
                param_base** slot = param_array->array + middle;

                std::memmove(slot + 1,
                             slot,
                             (param_array->size - middle) * sizeof(param_base*) - sizeof(param_base*));

                *slot = copy;
            }

            cached_param = nullptr;

            return;
        }
    }

    param_array->push_back(copy);

    cached_param = nullptr;
}

// sub_43E650
void param_block::set_pb_float(string_hash parameter, f32 value) {
    param_float param(parameter, value);

    add_param(&param, i_am_not_sure);
}

// sub_43E6E0
void param_block::set_pb_int(string_hash parameter, i32 value) {
    param_int param(parameter, value);

    add_param(&param, i_am_not_sure);
}

// sub_43E760
void param_block::set_pb_hash(string_hash parameter, const string_hash &value) {
    param_hash param(parameter, value);

    add_param(&param, i_am_not_sure);
}

// sub_43E8F0
void param_block::set_pb_fixedstring(string_hash parameter, const char* value) {
    mash::string string(value);
    param_string param(parameter, string);

    add_param(&param, i_am_not_sure);
}

// sub_43E7C0
void param_block::set_pb_vector3d(string_hash parameter, const vector3d &value) {
    param_vector3d param(parameter, value);

    add_param(&param, i_am_not_sure);
}

// sub_43E860
void param_block::set_pb_float_variance(string_hash parameter, const variance_variable<f32> &value) {
    param_float_variance param(parameter, value);

    add_param(&param, i_am_not_sure);
}

// sub_43E990
void param_block::set_pb_entity(string_hash parameter, const arch_base_vhandle &value) {
    param_entity param(parameter, value);

    add_param(&param, i_am_not_sure);
}

// sub_434D40
f32 param_block::get_pb_float(string_hash parameter) const {
    engine_lock_scope scope(&lock);

    param_base* found = common_find_data(parameter);

    return found ? found->get_data_float() : 0.0f;
}

// sub_434DF0
i32 param_block::get_pb_int(string_hash parameter) const {
    engine_lock_scope scope(&lock);

    param_base* found = common_find_data(parameter);

    return found ? found->get_data_int() : 0;
}

// sub_434E90
string_hash param_block::get_pb_hash(string_hash parameter) const {
    engine_lock_scope scope(&lock);

    param_base* found = common_find_data(parameter);

    if (found)
        return found->get_data_hash();

    string_hash none;
    none.initialize(mash::ALLOCATED);

    return none;
}

// sub_4350B0
const char* param_block::get_pb_fixedstring(string_hash parameter) const {
    engine_lock_scope scope(&lock);

    param_base* found = common_find_data(parameter);

    return found ? found->get_data_fixedstring().c_str() : "";
}

// sub_434F20
vector3d param_block::get_pb_vector3d(string_hash parameter) const {
    engine_lock_scope scope(&lock);

    param_base* found = common_find_data(parameter);

    return found ? found->get_data_vector3d() : references::unk_00fc3440.get();
}

// sub_434FF0
variance_variable<f32> param_block::get_pb_float_variance(string_hash parameter) const {
    engine_lock_scope scope(&lock);

    param_base* found = common_find_data(parameter);

    return found ? found->get_data_float_variance() : variance_variable<f32> { 0.0f, 0.0f };
}

// sub_435150
mash::mash_virtual_base* param_block::get_pb_class(string_hash parameter) const {
    engine_lock_scope scope(&lock);

    param_base* found = common_find_data(parameter);

    return found ? found->get_data_class() : nullptr;
}

// sub_4351F0
arch_base_vhandle param_block::get_pb_entity(string_hash parameter) const {
    engine_lock_scope scope(&lock);

    param_base* found = common_find_data(parameter);

    return found ? found->get_data_entity() : arch_base_vhandle();
}

// sub_435280
f32 param_block::get_optional_pb_float(string_hash parameter, f32 default_value, bool* exists) const {
    engine_lock_scope scope(&lock);

    param_base* found = common_find_data(parameter);

    if (exists)
        *exists = found != nullptr;

    return found ? found->get_data_float() : default_value;
}

// sub_435340
i32 param_block::get_optional_pb_int(string_hash parameter, i32 default_value, bool* exists) const {
    engine_lock_scope scope(&lock);

    param_base* found = common_find_data(parameter);

    if (exists)
        *exists = found != nullptr;

    return found ? found->get_data_int() : default_value;
}

// sub_435400
string_hash param_block::get_optional_pb_hash(string_hash parameter, string_hash default_value, bool* exists) const {
    engine_lock_scope scope(&lock);

    param_base* found = common_find_data(parameter);

    if (exists)
        *exists = found != nullptr;

    return found ? found->get_data_hash() : default_value;
}

// sub_435670
const char* param_block::get_optional_pb_fixedstring(string_hash parameter, const char* default_value, bool* exists) const {
    engine_lock_scope scope(&lock);

    param_base* found = common_find_data(parameter);

    if (exists)
        *exists = found != nullptr;

    return found ? found->get_data_fixedstring().c_str() : default_value;
}

// sub_4354C0
vector3d param_block::get_optional_pb_vector3d(string_hash parameter, const vector3d &default_value, bool* exists) const {
    engine_lock_scope scope(&lock);

    param_base* found = common_find_data(parameter);

    if (exists)
        *exists = found != nullptr;

    return found ? found->get_data_vector3d() : default_value;
}

// sub_4355A0
variance_variable<f32> param_block::get_optional_pb_float_variance(string_hash                   parameter,
                                                                   const variance_variable<f32> &default_value,
                                                                   bool*                         exists) const {

    engine_lock_scope scope(&lock);

    param_base* found = common_find_data(parameter);

    if (exists)
        *exists = found != nullptr;

    return found ? found->get_data_float_variance() : default_value;
}

// sub_435730
mash::mash_virtual_base* param_block::get_optional_pb_class(string_hash parameter, mash::mash_virtual_base* default_value, bool* exists) const {
    engine_lock_scope scope(&lock);

    param_base* found = common_find_data(parameter);

    if (exists)
        *exists = found != nullptr;

    return found ? found->get_data_class() : default_value;
}

// sub_4357F0
arch_base_vhandle param_block::get_optional_pb_entity(string_hash parameter, arch_base_vhandle default_value, bool* exists) const {
    engine_lock_scope scope(&lock);

    param_base* found = common_find_data(parameter);

    if (exists)
        *exists = found != nullptr;

    return found ? found->get_data_entity() : default_value;
}

// sub_435890
bool param_block::does_parameter_exist(string_hash parameter) const {
    engine_lock_scope scope(&lock);

    return common_find_data(parameter) != nullptr;
}

// sub_435910
e_param_type param_block::get_parameter_data_type(string_hash parameter) const {
    engine_lock_scope scope(&lock);

    return common_find_data(parameter)->my_type;
}

// sub_43E5B0
void param_block::reserve(u32 max_params) {
    engine_lock_scope scope(&lock);

    if (!param_array)
        allocate_param_array();

    param_array->reserve(max_params);
}

// sub_442500
void param_block::copy_from_pb_override(const param_block &old_data) {
    if (!old_data.param_array)
        return;

    engine_lock_scope old_scope(&old_data.lock);

    param_base** param = old_data.param_array->begin();
    param_base** end   = old_data.param_array->end();

    // the old block is sorted already, so an empty one takes it in order
    only_if_you_know_what_you_are_doing sort = param_array && param_array->size ? i_am_not_sure : i_know_what_i_am_doing;

    reserve(old_data.param_array->size);

    engine_lock_scope scope(&lock);

    for (; param != end; ++param) {
        if (*param)
            add_param(*param, sort);
    }
}

// sub_443E30
void param_block::construct_mashed_class() {
    // sub_58CA60
    lock.owner = 0;
    lock.state = 0;
    lock.depth = 0;

    if (param_array)
        param_array->construct_mashed_class();

    cached_param           = nullptr;
    param_array_is_dynamic = false;
}

// sub_4425C0
void param_block::destruct_mashed_class() {
    if (param_array && param_array_is_dynamic) {
        param_array->~vector();

        mm::free(param_array);

        param_array = nullptr;
    }

    if (param_array) {
        param_array->destruct_mashed_class();

        param_array = nullptr;
    }
}

// sub_442610
void param_block::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    u8 class_mashed;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(param_array, this, buffer);
    else
        param_array = nullptr;
}
