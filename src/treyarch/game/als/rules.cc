#include "retail.hh"
#include "treyarch/game/als/animation_logic_system.hh"
#include "treyarch/game/als/rules.hh"
#include "treyarch/game/als/state_machine.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/wds/ai/ai_core.hh"
#include "treyarch/game/wds/ai/info_node.hh"
#include "treyarch/game/wds/entity/actor.hh"
#include "treyarch/game/wds/entity/interface/player_interface.hh"
#include "treyarch/shared/math/references.hh"

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
string_hash als::upgrade_number_compare_rule::my_type_key                            { 0xA3607A21 }; // "als::upgrade_number_compare_rule"
string_hash als::upgrade_bounded_number_rule::my_type_key                            { 0x70D167FB }; // "als::upgrade_bounded_number_rule"
string_hash als::upgrade_inverse_bounded_number_rule::my_type_key                    { 0xD51A0136 }; // "als::upgrade_inverse_bounded_number_rule"
string_hash als::als_state_number_compare_rule::my_type_key                          { 0xD2793819 }; // "als::als_state_number_compare_rule"
string_hash als::als_state_vector_component_number_compare_rule::my_type_key         { 0x6FEEF03D }; // "als::als_state_vector_component_number_compare_rule"
string_hash als::als_state_hash_equal_rule::my_type_key                              { 0x8A4152A5 }; // "als::als_state_hash_equal_rule"
string_hash als::als_state_hash_not_equal_rule::my_type_key                          { 0xAA9DF2D5 }; // "als::als_state_hash_not_equal_rule"
string_hash als::als_state_bounded_number_rule::my_type_key                          { 0x9FEA25F3 }; // "als::als_state_bounded_number_rule"
string_hash als::als_state_inverse_bounded_number_rule::my_type_key                  { 0x542D372E }; // "als::als_state_inverse_bounded_number_rule"
string_hash als::als_state_vector_component_bounded_number_rule::my_type_key         { 0x3D5FDE17 }; // "als::als_state_vector_component_bounded_number_rule"
string_hash als::als_state_vector_component_inverse_bounded_number_rule::my_type_key { 0x695AD352 }; // "als::als_state_vector_component_inverse_bounded_number_rule"
string_hash als::als_state_number_param_compare_rule::my_type_key                    { 0x36AAF5E9 }; // "als::als_state_number_param_compare_rule"
string_hash als::als_state_number_param_vector_component_compare_rule::my_type_key   { 0x1306B58D }; // "als::als_state_number_param_vector_component_compare_rule"
string_hash als::ai_number_compare_rule::my_type_key                                 { 0xB5F4D7E3 }; // "als::ai_number_compare_rule"
string_hash als::ai_vector_component_number_compare_rule::my_type_key                { 0xB5735D47 }; // "als::ai_vector_component_number_compare_rule"
string_hash als::ai_hash_equal_rule::my_type_key                                     { 0xECE09D6F }; // "als::ai_hash_equal_rule"
string_hash als::ai_hash_not_equal_rule::my_type_key                                 { 0x8E19929F }; // "als::ai_hash_not_equal_rule"
string_hash als::ai_bounded_number_rule::my_type_key                                 { 0x8365C5BD }; // "als::ai_bounded_number_rule"
string_hash als::ai_inverse_bounded_number_rule::my_type_key                         { 0xB70900F8 }; // "als::ai_inverse_bounded_number_rule"
string_hash als::ai_vector_component_bounded_number_rule::my_type_key                { 0x82E44B21 }; // "als::ai_vector_component_bounded_number_rule"
string_hash als::ai_vector_component_inverse_bounded_number_rule::my_type_key        { 0x7BD8AA5C }; // "als::ai_vector_component_inverse_bounded_number_rule"
string_hash als::ai_number_param_compare_rule::my_type_key                           { 0xB395C533 }; // "als::ai_number_param_compare_rule"
string_hash als::ai_number_param_vector_component_compare_rule::my_type_key          { 0xF92F4217 }; // "als::ai_number_param_vector_component_compare_rule"
string_hash als::target_ai_number_compare_rule::my_type_key                          { 0x471ED3C9 }; // "als::target_ai_number_compare_rule"
string_hash als::target_ai_vector_component_number_compare_rule::my_type_key         { 0x791561ED }; // "als::target_ai_vector_component_number_compare_rule"
string_hash als::target_ai_hash_equal_rule::my_type_key                              { 0x088C9655 }; // "als::target_ai_hash_equal_rule"
string_hash als::target_ai_hash_not_equal_rule::my_type_key                          { 0x1F438E85 }; // "als::target_ai_hash_not_equal_rule"
string_hash als::target_ai_bounded_number_rule::my_type_key                          { 0x148FC1A3 }; // "als::target_ai_bounded_number_rule"
string_hash als::target_ai_inverse_bounded_number_rule::my_type_key                  { 0x0BCB82DE }; // "als::target_ai_inverse_bounded_number_rule"
string_hash als::target_ai_vector_component_bounded_number_rule::my_type_key         { 0x46864FC7 }; // "als::target_ai_vector_component_bounded_number_rule"
string_hash als::target_ai_vector_component_inverse_bounded_number_rule::my_type_key { 0x3BEFF502 }; // "als::target_ai_vector_component_inverse_bounded_number_rule"
string_hash als::target_ai_number_param_compare_rule::my_type_key                    { 0x41E29599 }; // "als::target_ai_number_param_compare_rule"
string_hash als::target_ai_number_param_vector_component_compare_rule::my_type_key   { 0x4187AB3D }; // "als::target_ai_number_param_vector_component_compare_rule"

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

// inlined @ sub_5B0780
// inlined @ sub_5B07F0
// inlined @ sub_5B0870
f32 als::get_upgrade_float_param(string_hash name) {
    using get_upgrade_id_t   = i32  (__thiscall*)(void*, const string_hash*);
    using is_upgrade_owned_t = bool (__thiscall*)(void*, i32);

    i32 id = ((get_upgrade_id_t)retail::sub_9210B0)(treyarch::references::upgrade_manager.read(), &name);

    return ((is_upgrade_owned_t)retail::sub_9219C0)(treyarch::references::upgrade_manager.read(), id) ? 1.0f : 0.0f;
}

// sub_5BD850
f32 als::get_als_state_float_param(state_machine* the_sm, string_hash name) {
    if (!the_sm || !the_sm->does_parameter_exist(name))
        return 0.0f;

    switch (the_sm->get_parameter_data_type(name)) {
        case param_type_float:
            return the_sm->get_pb_float(name);

        case param_type_int:
            return (f32)the_sm->get_pb_int(name);

        case param_type_float_variance:
            return the_sm->get_pb_float_variance(name).get_random_value();

        default:
            return 0.0f;
    }
}

// inlined @ sub_5B08F0
// inlined @ sub_5B0A00
// inlined @ sub_5B0AA0
f32 als::get_als_state_vector_component_param(state_machine* the_sm, string_hash name, e_vector_component component) {
    if (!the_sm)
        return 0.0f;

    return get_vector_component(the_sm->get_optional_pb_vector3d(name, treyarch::references::unk_00fd4370.get(), nullptr), component);
}

// sub_5B04D0
string_hash als::get_als_state_hash_param(state_machine* the_sm, string_hash name) {
    string_hash result;
    result.initialize(mash::ALLOCATED);

    if (the_sm && the_sm->does_parameter_exist(name)) {
        u32 type = the_sm->get_parameter_data_type(name);

        if (type == param_type_hash)
            result = the_sm->get_pb_hash(name);
        else if (type == param_type_string)
            result.initialize(mash::ALLOCATED, the_sm->get_pb_fixedstring(name));
    }

    return result;
}

// sub_5BD930
f32 als::get_ai_float_param(ai_core* core, string_hash name) {
    if (!core)
        return 0.0f;

    param_block &block = core->my_param_block;

    if (!block.does_parameter_exist(name))
        return 0.0f;

    switch (block.get_parameter_data_type(name)) {
        case param_type_float:
            return block.get_pb_float(name);

        case param_type_int:
            return (f32)block.get_pb_int(name);

        case param_type_float_variance:
            return block.get_pb_float_variance(name).get_random_value();

        default:
            return 0.0f;
    }
}

// inlined @ sub_5B0B40
// inlined @ sub_5B0D10
// inlined @ sub_5B0E00
// inlined @ sub_5BE110
// inlined @ sub_5BE440
// inlined @ sub_5BE530
f32 als::get_ai_vector_component_param(ai_core* core, string_hash name, e_vector_component component) {
    if (!core)
        return 0.0f;

    return get_vector_component(core->my_param_block.get_optional_pb_vector3d(name, treyarch::references::unk_00fd4370.get(), nullptr), component);
}

// sub_5B0580
string_hash als::get_ai_hash_param(ai_core* core, string_hash name) {
    string_hash result;
    result.initialize(mash::ALLOCATED);

    if (core && core->my_param_block.does_parameter_exist(name)) {
        param_block &block = core->my_param_block;
        e_param_type type  = block.get_parameter_data_type(name);

        if (type == param_type_hash)
            result = block.get_pb_hash(name);
        else if (type == param_type_string)
            result.initialize(mash::ALLOCATED, block.get_pb_fixedstring(name));
    }

    return result;
}

// sub_5BDC00
ai_core* als::get_target_ai_core(actor* the_actor) {
    using has_target_t = bool  (__thiscall*)(info_node*, u32);
    using get_target_t = void* (__thiscall*)(info_node*, u32);

    if (!the_actor)
        return nullptr;

    interface_storage* ifc_storage = the_actor->my_ifc_storage;

    if (ifc_storage && ifc_storage->has_ifc(entity_ifc_player)) {
        auto       player = (player_interface*)ifc_storage->get_ifc(entity_ifc_player);
        arch_base* target = player->unk_4b0.resolve();

        if (target && target->is_an_actor())
            return ((actor*)target)->get_ai_core();
    }

    if (info_node* basic = the_actor->get_ai_core()->get_info_node(info_node_type_ai_basic)) {
        if (((has_target_t)retail::sub_4EF830)(basic, 2)) {
            void* record = ((get_target_t)retail::sub_4F24E0)(basic, 2);

            // an entity record
            if (record && ((i32 (__thiscall*)(void*))(*(void***)record)[2])(record) == 1) {
                auto target = (arch_base*)((void* (__thiscall*)(void*))retail::sub_44F9D0)(record);

                if (target && target->is_an_actor())
                    return ((actor*)target)->get_ai_core();
            }
        }
    }

    if (info_node* combat_target = the_actor->get_ai_core()->get_info_node(info_node_type_combat_target)) {
        if (((bool (__thiscall*)(info_node*))(*(void***)combat_target)[24])(combat_target)) {
            auto target = ((arch_base* (__thiscall*)(info_node*))(*(void***)combat_target)[26])(combat_target);

            if (target && target->is_an_actor())
                return ((actor*)target)->get_ai_core();
        }
    }

    return nullptr;
}

// sub_5D6AF0
mash::string als::get_vector_component_string(e_vector_component component) {
    mash::string result("->");

    switch (component) {
        case component_x:
            result += mash::string("x");
            break;

        case component_y:
            result += mash::string("y");
            break;

        case component_z:
            result += mash::string("z");
            break;

        case component_norm_x:
            result += mash::string("norm_x");
            break;

        case component_norm_y:
            result += mash::string("norm_y");
            break;

        case component_norm_z:
            result += mash::string("norm_z");
            break;

        case component_length:
            result += mash::string("length");
            break;

        case component_length_sq:
            result += mash::string("length_sq");
            break;

        case component_xz_length:
            result += mash::string("xz_length");
            break;

        case component_xz_length_sq:
            result += mash::string("xz_length_sq");
            break;

        case component_y_length:
            result += mash::string("y_length");
            break;

        default:
            result = mash::string();
            break;
    }

    return result;
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

// sub_5D7FF0
mash::virtual_types_key als::upgrade_number_compare_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5D79A0
bool als::upgrade_number_compare_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (constant_number_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (constant_number_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 als::upgrade_number_compare_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5B0780
bool als::upgrade_number_compare_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    return compare_numbers(comparison, get_upgrade_float_param(name), value);
}

// sub_5BD490
mash::virtual_types_key als::upgrade_bounded_number_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD740
bool als::upgrade_bounded_number_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (bounded_number_rule::get_virtual_type_key() == parent_class)
        return true;

    if (bounded_number_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 als::upgrade_bounded_number_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5B07F0
bool als::upgrade_bounded_number_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    f32 number = get_upgrade_float_param(name);

    return number >= lower_bound && upper_bound >= number;
}

// sub_5BD6D0
mash::string als::upgrade_bounded_number_rule::get_param_string() const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    return mash::string(name.to_string());
}

// sub_5BD460
mash::string als::upgrade_bounded_number_rule::get_prefix_string() const {
    return mash::string("upgrade::");
}

// sub_5BD4A0
mash::virtual_types_key als::upgrade_inverse_bounded_number_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD740
bool als::upgrade_inverse_bounded_number_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (bounded_number_rule::get_virtual_type_key() == parent_class)
        return true;

    if (bounded_number_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 als::upgrade_inverse_bounded_number_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5B0870
bool als::upgrade_inverse_bounded_number_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    f32 number = get_upgrade_float_param(name);

    return lower_bound >= number || number >= upper_bound;
}

// sub_5D8000
mash::virtual_types_key als::als_state_number_compare_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5D79A0
bool als::als_state_number_compare_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (constant_number_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (constant_number_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 als::als_state_number_compare_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5BDD60
bool als::als_state_number_compare_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    return compare_numbers(comparison, get_als_state_float_param(context.the_sm, name), value);
}

// sub_5D8010
mash::virtual_types_key als::als_state_vector_component_number_compare_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5D8020
bool als::als_state_vector_component_number_compare_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (als_state_number_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (als_state_number_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D8160
i32 als::als_state_vector_component_number_compare_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5B08F0
bool als::als_state_vector_component_number_compare_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    return compare_numbers(comparison, get_als_state_vector_component_param(context.the_sm, name, component), value);
}

// sub_5D80D0
mash::string als::als_state_vector_component_number_compare_rule::get_component_string() const {
    return get_vector_component_string(component);
}

// sub_5BD4B0
mash::virtual_types_key als::als_state_hash_equal_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD6A0
bool als::als_state_hash_equal_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (hash_equal_rule::get_virtual_type_key() == parent_class)
        return true;

    if (hash_equal_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D7970
i32 als::als_state_hash_equal_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5B0980
bool als::als_state_hash_equal_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    return get_als_state_hash_param(context.the_sm, name) == value;
}

// sub_5BD6D0
mash::string als::als_state_hash_equal_rule::get_param_string() const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    return mash::string(name.to_string());
}

// sub_5BD550
mash::string als::als_state_hash_equal_rule::get_prefix_string() const {
    return mash::string("als_state::");
}

// sub_5BD4C0
mash::virtual_types_key als::als_state_hash_not_equal_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD4D0
bool als::als_state_hash_not_equal_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (hash_not_equal_rule::get_virtual_type_key() == parent_class)
        return true;

    if (hash_not_equal_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D7970
i32 als::als_state_hash_not_equal_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5B09C0
bool als::als_state_hash_not_equal_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    return get_als_state_hash_param(context.the_sm, name) != value;
}

// sub_5BD6D0
mash::string als::als_state_hash_not_equal_rule::get_param_string() const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    return mash::string(name.to_string());
}

// sub_5BD550
mash::string als::als_state_hash_not_equal_rule::get_prefix_string() const {
    return mash::string("als_state::");
}

// sub_5BD510
mash::virtual_types_key als::als_state_bounded_number_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD740
bool als::als_state_bounded_number_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (bounded_number_rule::get_virtual_type_key() == parent_class)
        return true;

    if (bounded_number_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 als::als_state_bounded_number_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5BDDB0
bool als::als_state_bounded_number_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    f32 number = get_als_state_float_param(context.the_sm, name);

    return number >= lower_bound && upper_bound >= number;
}

// sub_5BD6D0
mash::string als::als_state_bounded_number_rule::get_param_string() const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    return mash::string(name.to_string());
}

// sub_5BD550
mash::string als::als_state_bounded_number_rule::get_prefix_string() const {
    return mash::string("als_state::");
}

// sub_5BD520
mash::virtual_types_key als::als_state_inverse_bounded_number_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD740
bool als::als_state_inverse_bounded_number_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (bounded_number_rule::get_virtual_type_key() == parent_class)
        return true;

    if (bounded_number_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 als::als_state_inverse_bounded_number_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5BDE20
bool als::als_state_inverse_bounded_number_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    f32 number = get_als_state_float_param(context.the_sm, name);

    return lower_bound >= number || number >= upper_bound;
}

// sub_5BD530
mash::virtual_types_key als::als_state_vector_component_bounded_number_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD7C0
bool als::als_state_vector_component_bounded_number_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (vector_component_bounded_number_rule::get_virtual_type_key() == parent_class)
        return true;

    if (vector_component_bounded_number_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D8160
i32 als::als_state_vector_component_bounded_number_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5B0A00
bool als::als_state_vector_component_bounded_number_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    f32 number = get_als_state_vector_component_param(context.the_sm, name, component);

    return number >= lower_bound && upper_bound >= number;
}

// sub_5BD6D0
mash::string als::als_state_vector_component_bounded_number_rule::get_param_string() const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    return mash::string(name.to_string());
}

// sub_5BD550
mash::string als::als_state_vector_component_bounded_number_rule::get_prefix_string() const {
    return mash::string("als_state::");
}

// sub_5BD540
mash::virtual_types_key als::als_state_vector_component_inverse_bounded_number_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD7C0
bool als::als_state_vector_component_inverse_bounded_number_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (vector_component_bounded_number_rule::get_virtual_type_key() == parent_class)
        return true;

    if (vector_component_bounded_number_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D8160
i32 als::als_state_vector_component_inverse_bounded_number_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5B0AA0
bool als::als_state_vector_component_inverse_bounded_number_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    f32 number = get_als_state_vector_component_param(context.the_sm, name, component);

    return lower_bound >= number || number >= upper_bound;
}

// sub_5BD580
mash::virtual_types_key als::als_state_number_param_compare_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD810
bool als::als_state_number_param_compare_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (param_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (param_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 als::als_state_number_param_compare_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5EF6D0
bool als::als_state_number_param_compare_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    f32 lhs = get_als_state_float_param(context.the_sm, name);
    f32 rhs = context.the_sm->get_float_param(context.the_als, other_param);

    return compare_numbers(comparison, lhs, rhs);
}

// sub_5BD6D0
mash::string als::als_state_number_param_compare_rule::get_param_string() const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    return mash::string(name.to_string());
}

// sub_5BD550
mash::string als::als_state_number_param_compare_rule::get_prefix_string() const {
    return mash::string("als_state::");
}

// sub_5BD590
mash::virtual_types_key als::als_state_number_param_vector_component_compare_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD650
bool als::als_state_number_param_vector_component_compare_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (vector_param_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (vector_param_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D8160
i32 als::als_state_number_param_vector_component_compare_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5D8420
bool als::als_state_number_param_vector_component_compare_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    f32 lhs = get_als_state_float_param(context.the_sm, name);
    f32 rhs = context.the_sm->get_vector_component_param(context.the_als, other_param, other_component);

    return compare_numbers(comparison, lhs, rhs);
}

// sub_5BD6D0
mash::string als::als_state_number_param_vector_component_compare_rule::get_param_string() const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    return mash::string(name.to_string());
}

// sub_5BD550
mash::string als::als_state_number_param_vector_component_compare_rule::get_prefix_string() const {
    return mash::string("als_state::");
}

// sub_5D8060
mash::virtual_types_key als::ai_number_compare_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5D79A0
bool als::ai_number_compare_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (constant_number_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (constant_number_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 als::ai_number_compare_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5BDE90
bool als::ai_number_compare_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    ai_core* core = context.the_als->get_actor()->get_ai_core();

    return compare_numbers(comparison, get_ai_float_param(core, name), value);
}

// sub_5D8070
mash::virtual_types_key als::ai_vector_component_number_compare_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5D8080
bool als::ai_vector_component_number_compare_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (ai_number_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (ai_number_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D8160
i32 als::ai_vector_component_number_compare_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5B0B40
bool als::ai_vector_component_number_compare_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    ai_core* core = context.the_als->get_actor()->get_ai_core();

    return compare_numbers(comparison, get_ai_vector_component_param(core, name, component), value);
}

// sub_5D80D0
mash::string als::ai_vector_component_number_compare_rule::get_component_string() const {
    return get_vector_component_string(component);
}

// sub_5BD5A0
mash::virtual_types_key als::ai_hash_equal_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD6A0
bool als::ai_hash_equal_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (hash_equal_rule::get_virtual_type_key() == parent_class)
        return true;

    if (hash_equal_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D7970
i32 als::ai_hash_equal_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5B0C10
bool als::ai_hash_equal_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    ai_core* core = context.the_als->get_actor()->get_ai_core();

    return get_ai_hash_param(core, name) == value;
}

// sub_5BD6D0
mash::string als::ai_hash_equal_rule::get_param_string() const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    return mash::string(name.to_string());
}

// sub_5BD5C0
mash::string als::ai_hash_equal_rule::get_prefix_string() const {
    return mash::string("ai::");
}

// sub_5BD5B0
mash::virtual_types_key als::ai_hash_not_equal_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD4D0
bool als::ai_hash_not_equal_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (hash_not_equal_rule::get_virtual_type_key() == parent_class)
        return true;

    if (hash_not_equal_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D7970
i32 als::ai_hash_not_equal_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5B0C90
bool als::ai_hash_not_equal_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    ai_core* core = context.the_als->get_actor()->get_ai_core();

    return get_ai_hash_param(core, name) != value;
}

// sub_5BD6D0
mash::string als::ai_hash_not_equal_rule::get_param_string() const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    return mash::string(name.to_string());
}

// sub_5BD5C0
mash::string als::ai_hash_not_equal_rule::get_prefix_string() const {
    return mash::string("ai::");
}

// sub_5BD5F0
mash::virtual_types_key als::ai_bounded_number_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD740
bool als::ai_bounded_number_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (bounded_number_rule::get_virtual_type_key() == parent_class)
        return true;

    if (bounded_number_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 als::ai_bounded_number_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5BDF20
bool als::ai_bounded_number_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    ai_core* core = context.the_als->get_actor()->get_ai_core();

    f32 number = get_ai_float_param(core, name);

    return number >= lower_bound && upper_bound >= number;
}

// sub_5BD6D0
mash::string als::ai_bounded_number_rule::get_param_string() const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    return mash::string(name.to_string());
}

// sub_5BD5C0
mash::string als::ai_bounded_number_rule::get_prefix_string() const {
    return mash::string("ai::");
}

// sub_5BD600
mash::virtual_types_key als::ai_inverse_bounded_number_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD740
bool als::ai_inverse_bounded_number_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (bounded_number_rule::get_virtual_type_key() == parent_class)
        return true;

    if (bounded_number_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 als::ai_inverse_bounded_number_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5BDFD0
bool als::ai_inverse_bounded_number_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    ai_core* core = context.the_als->get_actor()->get_ai_core();

    f32 number = get_ai_float_param(core, name);

    return lower_bound >= number || number >= upper_bound;
}

// sub_5BD610
mash::virtual_types_key als::ai_vector_component_bounded_number_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD7C0
bool als::ai_vector_component_bounded_number_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (vector_component_bounded_number_rule::get_virtual_type_key() == parent_class)
        return true;

    if (vector_component_bounded_number_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D8160
i32 als::ai_vector_component_bounded_number_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5B0D10
bool als::ai_vector_component_bounded_number_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    ai_core* core = context.the_als->get_actor()->get_ai_core();

    f32 number = get_ai_vector_component_param(core, name, component);

    return number >= lower_bound && upper_bound >= number;
}

// sub_5BD6D0
mash::string als::ai_vector_component_bounded_number_rule::get_param_string() const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    return mash::string(name.to_string());
}

// sub_5BD5C0
mash::string als::ai_vector_component_bounded_number_rule::get_prefix_string() const {
    return mash::string("ai::");
}

// sub_5BD620
mash::virtual_types_key als::ai_vector_component_inverse_bounded_number_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD7C0
bool als::ai_vector_component_inverse_bounded_number_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (vector_component_bounded_number_rule::get_virtual_type_key() == parent_class)
        return true;

    if (vector_component_bounded_number_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D8160
i32 als::ai_vector_component_inverse_bounded_number_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5B0E00
bool als::ai_vector_component_inverse_bounded_number_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    ai_core* core = context.the_als->get_actor()->get_ai_core();

    f32 number = get_ai_vector_component_param(core, name, component);

    return lower_bound >= number || number >= upper_bound;
}

// sub_5BD630
mash::virtual_types_key als::ai_number_param_compare_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD810
bool als::ai_number_param_compare_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (param_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (param_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 als::ai_number_param_compare_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5EF780
bool als::ai_number_param_compare_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    ai_core* core = context.the_als->get_actor()->get_ai_core();

    f32 lhs = get_ai_float_param(core, name);
    f32 rhs = context.the_sm->get_float_param(context.the_als, other_param);

    return compare_numbers(comparison, lhs, rhs);
}

// sub_5BD6D0
mash::string als::ai_number_param_compare_rule::get_param_string() const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    return mash::string(name.to_string());
}

// sub_5BD5C0
mash::string als::ai_number_param_compare_rule::get_prefix_string() const {
    return mash::string("ai::");
}

// sub_5BD640
mash::virtual_types_key als::ai_number_param_vector_component_compare_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD650
bool als::ai_number_param_vector_component_compare_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (vector_param_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (vector_param_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D8160
i32 als::ai_number_param_vector_component_compare_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5D84A0
bool als::ai_number_param_vector_component_compare_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    ai_core* core = context.the_als->get_actor()->get_ai_core();

    f32 lhs = get_ai_float_param(core, name);
    f32 rhs = context.the_sm->get_vector_component_param(context.the_als, other_param, other_component);

    return compare_numbers(comparison, lhs, rhs);
}

// sub_5BD6D0
mash::string als::ai_number_param_vector_component_compare_rule::get_param_string() const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    return mash::string(name.to_string());
}

// sub_5BD5C0
mash::string als::ai_number_param_vector_component_compare_rule::get_prefix_string() const {
    return mash::string("ai::");
}

// sub_5D80C0
mash::virtual_types_key als::target_ai_number_compare_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5D79A0
bool als::target_ai_number_compare_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (constant_number_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (constant_number_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 als::target_ai_number_compare_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5BE080
bool als::target_ai_number_compare_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    ai_core* core = get_target_ai_core(context.the_als->get_actor());

    return compare_numbers(comparison, get_ai_float_param(core, name), value);
}

// sub_5D8100
mash::virtual_types_key als::target_ai_vector_component_number_compare_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5D8110
bool als::target_ai_vector_component_number_compare_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (target_ai_number_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (target_ai_number_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D8160
i32 als::target_ai_vector_component_number_compare_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5BE110
bool als::target_ai_vector_component_number_compare_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    ai_core* core = get_target_ai_core(context.the_als->get_actor());

    return compare_numbers(comparison, get_ai_vector_component_param(core, name, component), value);
}

// sub_5D80D0
mash::string als::target_ai_vector_component_number_compare_rule::get_component_string() const {
    return get_vector_component_string(component);
}

// sub_5BD690
mash::virtual_types_key als::target_ai_hash_equal_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD6A0
bool als::target_ai_hash_equal_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (hash_equal_rule::get_virtual_type_key() == parent_class)
        return true;

    if (hash_equal_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D7970
i32 als::target_ai_hash_equal_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5BE1E0
bool als::target_ai_hash_equal_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    ai_core* core = get_target_ai_core(context.the_als->get_actor());

    return get_ai_hash_param(core, name) == value;
}

// sub_5BD6D0
mash::string als::target_ai_hash_equal_rule::get_param_string() const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    return mash::string(name.to_string());
}

// sub_5BD770
mash::string als::target_ai_hash_equal_rule::get_prefix_string() const {
    return mash::string("target_ai::");
}

// sub_5BD710
mash::virtual_types_key als::target_ai_hash_not_equal_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD4D0
bool als::target_ai_hash_not_equal_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (hash_not_equal_rule::get_virtual_type_key() == parent_class)
        return true;

    if (hash_not_equal_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D7970
i32 als::target_ai_hash_not_equal_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5BE260
bool als::target_ai_hash_not_equal_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    ai_core* core = get_target_ai_core(context.the_als->get_actor());

    return get_ai_hash_param(core, name) != value;
}

// sub_5BD6D0
mash::string als::target_ai_hash_not_equal_rule::get_param_string() const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    return mash::string(name.to_string());
}

// sub_5BD770
mash::string als::target_ai_hash_not_equal_rule::get_prefix_string() const {
    return mash::string("target_ai::");
}

// sub_5BD720
mash::virtual_types_key als::target_ai_bounded_number_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD740
bool als::target_ai_bounded_number_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (bounded_number_rule::get_virtual_type_key() == parent_class)
        return true;

    if (bounded_number_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 als::target_ai_bounded_number_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5BE2E0
bool als::target_ai_bounded_number_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    ai_core* core = get_target_ai_core(context.the_als->get_actor());

    f32 number = get_ai_float_param(core, name);

    return number >= lower_bound && upper_bound >= number;
}

// sub_5BD6D0
mash::string als::target_ai_bounded_number_rule::get_param_string() const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    return mash::string(name.to_string());
}

// sub_5BD770
mash::string als::target_ai_bounded_number_rule::get_prefix_string() const {
    return mash::string("target_ai::");
}

// sub_5BD730
mash::virtual_types_key als::target_ai_inverse_bounded_number_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD740
bool als::target_ai_inverse_bounded_number_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (bounded_number_rule::get_virtual_type_key() == parent_class)
        return true;

    if (bounded_number_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 als::target_ai_inverse_bounded_number_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5BE390
bool als::target_ai_inverse_bounded_number_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    ai_core* core = get_target_ai_core(context.the_als->get_actor());

    f32 number = get_ai_float_param(core, name);

    return lower_bound >= number || number >= upper_bound;
}

// sub_5BD7A0
mash::virtual_types_key als::target_ai_vector_component_bounded_number_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD7C0
bool als::target_ai_vector_component_bounded_number_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (vector_component_bounded_number_rule::get_virtual_type_key() == parent_class)
        return true;

    if (vector_component_bounded_number_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D8160
i32 als::target_ai_vector_component_bounded_number_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5BE440
bool als::target_ai_vector_component_bounded_number_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    ai_core* core = get_target_ai_core(context.the_als->get_actor());

    f32 number = get_ai_vector_component_param(core, name, component);

    return number >= lower_bound && upper_bound >= number;
}

// sub_5BD6D0
mash::string als::target_ai_vector_component_bounded_number_rule::get_param_string() const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    return mash::string(name.to_string());
}

// sub_5BD770
mash::string als::target_ai_vector_component_bounded_number_rule::get_prefix_string() const {
    return mash::string("target_ai::");
}

// sub_5BD7B0
mash::virtual_types_key als::target_ai_vector_component_inverse_bounded_number_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD7C0
bool als::target_ai_vector_component_inverse_bounded_number_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (vector_component_bounded_number_rule::get_virtual_type_key() == parent_class)
        return true;

    if (vector_component_bounded_number_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D8160
i32 als::target_ai_vector_component_inverse_bounded_number_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5BE530
bool als::target_ai_vector_component_inverse_bounded_number_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    ai_core* core = get_target_ai_core(context.the_als->get_actor());

    f32 number = get_ai_vector_component_param(core, name, component);

    return lower_bound >= number || number >= upper_bound;
}

// sub_5BD800
mash::virtual_types_key als::target_ai_number_param_compare_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD810
bool als::target_ai_number_param_compare_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (param_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (param_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 als::target_ai_number_param_compare_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5EF870
bool als::target_ai_number_param_compare_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    ai_core* core = get_target_ai_core(context.the_als->get_actor());

    f32 lhs = get_ai_float_param(core, name);
    f32 rhs = context.the_sm->get_float_param(context.the_als, other_param);

    return compare_numbers(comparison, lhs, rhs);
}

// sub_5BD6D0
mash::string als::target_ai_number_param_compare_rule::get_param_string() const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    return mash::string(name.to_string());
}

// sub_5BD770
mash::string als::target_ai_number_param_compare_rule::get_prefix_string() const {
    return mash::string("target_ai::");
}

// sub_5BD840
mash::virtual_types_key als::target_ai_number_param_vector_component_compare_rule::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5BD650
bool als::target_ai_number_param_vector_component_compare_rule::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (vector_param_compare_rule::get_virtual_type_key() == parent_class)
        return true;

    if (vector_param_compare_rule::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5D8160
i32 als::target_ai_number_param_vector_component_compare_rule::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5D8560
bool als::target_ai_number_param_vector_component_compare_rule::evaluate(const rule_context &context) const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    ai_core* core = get_target_ai_core(context.the_als->get_actor());

    f32 lhs = get_ai_float_param(core, name);
    f32 rhs = context.the_sm->get_vector_component_param(context.the_als, other_param, other_component);

    return compare_numbers(comparison, lhs, rhs);
}

// sub_5BD6D0
mash::string als::target_ai_number_param_vector_component_compare_rule::get_param_string() const {
    string_hash name;
    name.initialize(mash::ALLOCATED, nullptr, param);

    return mash::string(name.to_string());
}

// sub_5BD770
mash::string als::target_ai_number_param_vector_component_compare_rule::get_prefix_string() const {
    return mash::string("target_ai::");
}
