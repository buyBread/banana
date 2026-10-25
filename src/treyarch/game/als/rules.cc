#include "treyarch/game/als/rules.hh"
#include "treyarch/game/als/state_machine.hh"

using namespace treyarch;

string_hash als::transition_rule_base::my_type_key                                          { 0x82EC38F0 }; // "als::transition_rule_base"
string_hash als::constant_number_compare_rule::my_type_key                                  { 0xEC24FB03 }; // "als::constant_number_compare_rule"
string_hash als::vector_component_constant_number_compare_rule::my_type_key                 { 0x303AEDC7 }; // "als::vector_component_constant_number_compare_rule"
string_hash als::number_compare_equal_rule::my_type_key                                     { 0x9BB1F131 }; // "als::number_compare_equal_rule"
string_hash als::number_compare_not_equal_rule::my_type_key                                 { 0x2912F761 }; // "als::number_compare_not_equal_rule"
string_hash als::number_compare_greater_than_rule::my_type_key                              { 0x7CD1BD6D }; // "als::number_compare_greater_than_rule"
string_hash als::number_compare_less_than_rule::my_type_key                                 { 0x1DAF5EDA }; // "als::number_compare_less_than_rule"
string_hash als::number_compare_greater_than_or_equal_to_rule::my_type_key                  { 0xF6151926 }; // "als::number_compare_greater_than_or_equal_to_rule"
string_hash als::number_compare_less_than_or_equal_to_rule::my_type_key                     { 0x5AE34613 }; // "als::number_compare_less_than_or_equal_to_rule"
string_hash als::vector_component_number_compare_equal_rule::my_type_key                    { 0x183B3A75 }; // "als::vector_component_number_compare_equal_rule"
string_hash als::vector_component_number_compare_not_equal_rule::my_type_key                { 0xEFE742A5 }; // "als::vector_component_number_compare_not_equal_rule"
string_hash als::vector_component_number_compare_greater_than_rule::my_type_key             { 0xEC6B7231 }; // "als::vector_component_number_compare_greater_than_rule"
string_hash als::vector_component_number_compare_less_than_rule::my_type_key                { 0xE483AA1E }; // "als::vector_component_number_compare_less_than_rule"
string_hash als::vector_component_number_compare_greater_than_or_equal_to_rule::my_type_key { 0x2AA013EA }; // "als::vector_component_number_compare_greater_than_or_equal_to_rule"
string_hash als::vector_component_number_compare_less_than_or_equal_to_rule::my_type_key    { 0xECBE9757 }; // "als::vector_component_number_compare_less_than_or_equal_to_rule"
string_hash als::param_compare_rule::my_type_key                                            { 0xF1F93642 }; // "als::param_compare_rule"
string_hash als::vector_component_param_compare_rule::my_type_key                           { 0xADC82406 }; // "als::vector_component_param_compare_rule"
string_hash als::vector_param_compare_rule::my_type_key                                     { 0x4B4A99D4 }; // "als::vector_param_compare_rule"
string_hash als::vector_component_vector_param_compare_rule::my_type_key                    { 0xC7D3E318 }; // "als::vector_component_vector_param_compare_rule"
string_hash als::hash_equal_rule::my_type_key                                               { 0xEF8AEA86 }; // "als::hash_equal_rule"
string_hash als::hash_not_equal_rule::my_type_key                                           { 0x48C29336 }; // "als::hash_not_equal_rule"
string_hash als::hash_param_equal_rule::my_type_key                                         { 0xDD63D156 }; // "als::hash_param_equal_rule"
string_hash als::hash_param_not_equal_rule::my_type_key                                     { 0xB3526206 }; // "als::hash_param_not_equal_rule"
string_hash als::bounded_number_rule::my_type_key                                           { 0x3E0EC654 }; // "als::bounded_number_rule"
string_hash als::inverse_bounded_number_rule::my_type_key                                   { 0xC018A88F }; // "als::inverse_bounded_number_rule"
string_hash als::vector_component_bounded_number_rule::my_type_key                          { 0x73BB6C98 }; // "als::vector_component_bounded_number_rule"
string_hash als::vector_component_inverse_bounded_number_rule::my_type_key                  { 0x841952D3 }; // "als::vector_component_inverse_bounded_number_rule"

// sub_5B03A0
bool als::compare_numbers(e_comparison comparison, f32 lhs, f32 rhs) {
    switch (comparison) {
        case e_comparison::equal:
            return lhs == rhs;

        case e_comparison::not_equal:
            return lhs != rhs;

        case e_comparison::greater_than:
            return lhs > rhs;

        case e_comparison::less_than:
            return lhs < rhs;

        case e_comparison::greater_than_or_equal_to:
            return lhs >= rhs;

        case e_comparison::less_than_or_equal_to:
            return lhs <= rhs;

        default:
            return false;
    }
}

// sub_5BD2D0
mash::virtual_types_key als::transition_rule_base::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_718FB0
bool als::transition_rule_base::is_subclass_of(mash::virtual_types_key parent_class) const {
    return mash::mash_virtual_base::get_virtual_type_key() == parent_class;
}

// sub_4C6B00
i32 als::transition_rule_base::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5BD2C0
bool als::transition_rule_base::evaluate(const rule_context &) const {
    return false;
}

// sub_5D6AC0
mash::virtual_types_key als::constant_number_compare_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD2F0
bool als::constant_number_compare_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (transition_rule_base::get_virtual_type_key() == parent_class)
        return true;

    if (transition_rule_base::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D7970
i32 als::constant_number_compare_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5D6AD0
mash::virtual_types_key als::vector_component_constant_number_compare_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5D79A0
bool als::vector_component_constant_number_compare_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (constant_number_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (constant_number_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 als::vector_component_constant_number_compare_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5D7910
mash::virtual_types_key als::number_compare_equal_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5D79A0
bool als::number_compare_equal_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (constant_number_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (constant_number_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D7970
i32 als::number_compare_equal_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5EF050
bool als::number_compare_equal_rule::evaluate(const rule_context &context) const {
    return context.the_sm->get_float_param(context.the_als, param) == value;
}

// sub_5D7920
mash::string als::number_compare_equal_rule::get_operator_string() const {
    return mash::string("==");
}

// sub_5D7960
mash::virtual_types_key als::number_compare_not_equal_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5D79A0
bool als::number_compare_not_equal_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (constant_number_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (constant_number_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D7970
i32 als::number_compare_not_equal_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5EF0E0
bool als::number_compare_not_equal_rule::evaluate(const rule_context &context) const {
    return context.the_sm->get_float_param(context.the_als, param) != value;
}

// sub_5BD360
mash::string als::number_compare_not_equal_rule::get_operator_string() const {
    return mash::string("!=");
}

// sub_5D7990
mash::virtual_types_key als::number_compare_greater_than_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5D79A0
bool als::number_compare_greater_than_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (constant_number_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (constant_number_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D7970
i32 als::number_compare_greater_than_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5EF170
bool als::number_compare_greater_than_rule::evaluate(const rule_context &context) const {
    return context.the_sm->get_float_param(context.the_als, param) > value;
}

// sub_5D79D0
mash::string als::number_compare_greater_than_rule::get_operator_string() const {
    return mash::string(">");
}

// sub_5D7A40
mash::virtual_types_key als::number_compare_less_than_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5D79A0
bool als::number_compare_less_than_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (constant_number_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (constant_number_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D7970
i32 als::number_compare_less_than_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5EF1F0
bool als::number_compare_less_than_rule::evaluate(const rule_context &context) const {
    return context.the_sm->get_float_param(context.the_als, param) < value;
}

// sub_5D7A10
mash::string als::number_compare_less_than_rule::get_operator_string() const {
    return mash::string("<");
}

// sub_5D7A90
mash::virtual_types_key als::number_compare_greater_than_or_equal_to_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5D79A0
bool als::number_compare_greater_than_or_equal_to_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (constant_number_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (constant_number_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D7970
i32 als::number_compare_greater_than_or_equal_to_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5EF270
bool als::number_compare_greater_than_or_equal_to_rule::evaluate(const rule_context &context) const {
    return context.the_sm->get_float_param(context.the_als, param) >= value;
}

// sub_5D7A60
mash::string als::number_compare_greater_than_or_equal_to_rule::get_operator_string() const {
    return mash::string(">=");
}

// sub_5D7AB0
mash::virtual_types_key als::number_compare_less_than_or_equal_to_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5D79A0
bool als::number_compare_less_than_or_equal_to_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (constant_number_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (constant_number_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D7970
i32 als::number_compare_less_than_or_equal_to_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5EF2F0
bool als::number_compare_less_than_or_equal_to_rule::evaluate(const rule_context &context) const {
    return context.the_sm->get_float_param(context.the_als, param) <= value;
}

// sub_5D7AC0
mash::string als::number_compare_less_than_or_equal_to_rule::get_operator_string() const {
    return mash::string("<=");
}

// sub_5D7950
mash::virtual_types_key als::vector_component_number_compare_equal_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5D7B00
bool als::vector_component_number_compare_equal_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (vector_component_constant_number_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (vector_component_constant_number_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 als::vector_component_number_compare_equal_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5D7B40
bool als::vector_component_number_compare_equal_rule::evaluate(const rule_context &context) const {
    return context.the_sm->get_vector_component_param(context.the_als, param, component) == value;
}

// sub_5D7920
mash::string als::vector_component_number_compare_equal_rule::get_operator_string() const {
    return mash::string("==");
}

// sub_5D7980
mash::virtual_types_key als::vector_component_number_compare_not_equal_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5D7B00
bool als::vector_component_number_compare_not_equal_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (vector_component_constant_number_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (vector_component_constant_number_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 als::vector_component_number_compare_not_equal_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5D7BA0
bool als::vector_component_number_compare_not_equal_rule::evaluate(const rule_context &context) const {
    return context.the_sm->get_vector_component_param(context.the_als, param, component) != value;
}

// sub_5BD360
mash::string als::vector_component_number_compare_not_equal_rule::get_operator_string() const {
    return mash::string("!=");
}

// sub_5D7A00
mash::virtual_types_key als::vector_component_number_compare_greater_than_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5D7B00
bool als::vector_component_number_compare_greater_than_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (vector_component_constant_number_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (vector_component_constant_number_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 als::vector_component_number_compare_greater_than_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5D7C00
bool als::vector_component_number_compare_greater_than_rule::evaluate(const rule_context &context) const {
    return context.the_sm->get_vector_component_param(context.the_als, param, component) > value;
}

// sub_5D79D0
mash::string als::vector_component_number_compare_greater_than_rule::get_operator_string() const {
    return mash::string(">");
}

// sub_5D7A50
mash::virtual_types_key als::vector_component_number_compare_less_than_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5D7B00
bool als::vector_component_number_compare_less_than_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (vector_component_constant_number_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (vector_component_constant_number_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 als::vector_component_number_compare_less_than_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5D7C60
bool als::vector_component_number_compare_less_than_rule::evaluate(const rule_context &context) const {
    return context.the_sm->get_vector_component_param(context.the_als, param, component) < value;
}

// sub_5D7A10
mash::string als::vector_component_number_compare_less_than_rule::get_operator_string() const {
    return mash::string("<");
}

// sub_5D7AA0
mash::virtual_types_key als::vector_component_number_compare_greater_than_or_equal_to_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5D7B00
bool als::vector_component_number_compare_greater_than_or_equal_to_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (vector_component_constant_number_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (vector_component_constant_number_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 als::vector_component_number_compare_greater_than_or_equal_to_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5D7CC0
bool als::vector_component_number_compare_greater_than_or_equal_to_rule::evaluate(const rule_context &context) const {
    return context.the_sm->get_vector_component_param(context.the_als, param, component) >= value;
}

// sub_5D7A60
mash::string als::vector_component_number_compare_greater_than_or_equal_to_rule::get_operator_string() const {
    return mash::string(">=");
}

// sub_5D7AF0
mash::virtual_types_key als::vector_component_number_compare_less_than_or_equal_to_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5D7B00
bool als::vector_component_number_compare_less_than_or_equal_to_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (vector_component_constant_number_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (vector_component_constant_number_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 als::vector_component_number_compare_less_than_or_equal_to_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5D7D20
bool als::vector_component_number_compare_less_than_or_equal_to_rule::evaluate(const rule_context &context) const {
    return context.the_sm->get_vector_component_param(context.the_als, param, component) <= value;
}

// sub_5D7AC0
mash::string als::vector_component_number_compare_less_than_or_equal_to_rule::get_operator_string() const {
    return mash::string("<=");
}

// sub_5BD2E0
mash::virtual_types_key als::param_compare_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD2F0
bool als::param_compare_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (transition_rule_base::get_virtual_type_key() == parent_class)
        return true;

    if (transition_rule_base::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 als::param_compare_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5EF370
bool als::param_compare_rule::evaluate(const rule_context &context) const {
    f32 lhs = context.the_sm->get_float_param(context.the_als, param);
    f32 rhs = context.the_sm->get_float_param(context.the_als, other_param);

    return compare_numbers(comparison, lhs, rhs);
}

// sub_5BD320
mash::virtual_types_key als::vector_component_param_compare_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD810
bool als::vector_component_param_compare_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (param_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (param_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D8160
i32 als::vector_component_param_compare_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5EF450
bool als::vector_component_param_compare_rule::evaluate(const rule_context &context) const {
    f32 lhs = context.the_sm->get_vector_component_param(context.the_als, param, component);
    f32 rhs = context.the_sm->get_float_param(context.the_als, other_param);

    return compare_numbers(comparison, lhs, rhs);
}

// sub_5BD330
mash::virtual_types_key als::vector_param_compare_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD810
bool als::vector_param_compare_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (param_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (param_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D8160
i32 als::vector_param_compare_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5EF500
bool als::vector_param_compare_rule::evaluate(const rule_context &context) const {
    f32 lhs = context.the_sm->get_float_param(context.the_als, param);
    f32 rhs = context.the_sm->get_vector_component_param(context.the_als, other_param, other_component);

    return compare_numbers(comparison, lhs, rhs);
}

// sub_5BD340
mash::virtual_types_key als::vector_component_vector_param_compare_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD650
bool als::vector_component_vector_param_compare_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (vector_param_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (vector_param_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_71D790
i32 als::vector_component_vector_param_compare_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5D7D80
bool als::vector_component_vector_param_compare_rule::evaluate(const rule_context &context) const {
    f32 lhs = context.the_sm->get_vector_component_param(context.the_als, param, component);
    f32 rhs = context.the_sm->get_vector_component_param(context.the_als, other_param, other_component);

    return compare_numbers(comparison, lhs, rhs);
}

// sub_5B04C0
void als::hash_equal_rule::construct_mashed_class() {
    value.construct_mashed_class();
}

// sub_5B04C0
void als::hash_equal_rule::destruct_mashed_class() {
    value.destruct_mashed_class();
}

// sub_5E2F30
void als::hash_equal_rule::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    value.unmash(mash_info, this, buffer);
}

// sub_5BD350
mash::virtual_types_key als::hash_equal_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD2F0
bool als::hash_equal_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (transition_rule_base::get_virtual_type_key() == parent_class)
        return true;

    if (transition_rule_base::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D7970
i32 als::hash_equal_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5D7E10
bool als::hash_equal_rule::evaluate(const rule_context &context) const {
    return context.the_sm->get_hash_param(context.the_als, param) == value;
}

// sub_5BD390
mash::virtual_types_key als::hash_not_equal_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD6A0
bool als::hash_not_equal_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (hash_equal_rule::get_virtual_type_key() == parent_class)
        return true;

    if (hash_equal_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D7970
i32 als::hash_not_equal_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5D7E40
bool als::hash_not_equal_rule::evaluate(const rule_context &context) const {
    return context.the_sm->get_hash_param(context.the_als, param) != value;
}

// sub_5BD360
mash::string als::hash_not_equal_rule::get_operator_string() const {
    return mash::string("!=");
}

// sub_5BD3A0
mash::virtual_types_key als::hash_param_equal_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD2F0
bool als::hash_param_equal_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (transition_rule_base::get_virtual_type_key() == parent_class)
        return true;

    if (transition_rule_base::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D7970
i32 als::hash_param_equal_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5D7E70
bool als::hash_param_equal_rule::evaluate(const rule_context &context) const {
    string_hash lhs = context.the_sm->get_hash_param(context.the_als, param);
    string_hash rhs = context.the_sm->get_hash_param(context.the_als, other_param);

    return lhs == rhs;
}

// sub_5BD3E0
mash::virtual_types_key als::hash_param_not_equal_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD3F0
bool als::hash_param_not_equal_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (hash_param_equal_rule::get_virtual_type_key() == parent_class)
        return true;

    if (hash_param_equal_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D7970
i32 als::hash_param_not_equal_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5D7EC0
bool als::hash_param_not_equal_rule::evaluate(const rule_context &context) const {
    string_hash lhs = context.the_sm->get_hash_param(context.the_als, param);
    string_hash rhs = context.the_sm->get_hash_param(context.the_als, other_param);

    return lhs != rhs;
}

// sub_5BD3B0
mash::string als::hash_param_not_equal_rule::get_operator_string() const {
    return mash::string("neq");
}

// sub_5BD420
mash::virtual_types_key als::bounded_number_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD2F0
bool als::bounded_number_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (transition_rule_base::get_virtual_type_key() == parent_class)
        return true;

    if (transition_rule_base::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 als::bounded_number_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5EF5B0
bool als::bounded_number_rule::evaluate(const rule_context &context) const {
    f32 number = context.the_sm->get_float_param(context.the_als, param);

    return number >= lower_bound && upper_bound >= number;
}

// sub_5BD430
mash::virtual_types_key als::inverse_bounded_number_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD740
bool als::inverse_bounded_number_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (bounded_number_rule::get_virtual_type_key() == parent_class)
        return true;

    if (bounded_number_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 als::inverse_bounded_number_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5EF640
bool als::inverse_bounded_number_rule::evaluate(const rule_context &context) const {
    f32 number = context.the_sm->get_float_param(context.the_als, param);

    return lower_bound >= number || number >= upper_bound;
}

// sub_5BD440
mash::virtual_types_key als::vector_component_bounded_number_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD740
bool als::vector_component_bounded_number_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (bounded_number_rule::get_virtual_type_key() == parent_class)
        return true;

    if (bounded_number_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D8160
i32 als::vector_component_bounded_number_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5D7F10
bool als::vector_component_bounded_number_rule::evaluate(const rule_context &context) const {
    f32 number = context.the_sm->get_vector_component_param(context.the_als, param, component);

    return number >= lower_bound && upper_bound >= number;
}

// sub_5BD450
mash::virtual_types_key als::vector_component_inverse_bounded_number_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD7C0
bool als::vector_component_inverse_bounded_number_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (vector_component_bounded_number_rule::get_virtual_type_key() == parent_class)
        return true;

    if (vector_component_bounded_number_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D8160
i32 als::vector_component_inverse_bounded_number_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5D7F80
bool als::vector_component_inverse_bounded_number_rule::evaluate(const rule_context &context) const {
    f32 number = context.the_sm->get_vector_component_param(context.the_als, param, component);

    return lower_bound >= number || number >= upper_bound;
}
