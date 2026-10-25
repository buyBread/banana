#include "retail.hh"
#include "treyarch/game/als/script_state.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;

string_hash als::scripted_state::my_type_key            { 0xB7E96372 }; // "als::scripted_state"
string_hash als::base_layer_scripted_state::my_type_key { 0x0082A908 }; // "als::base_layer_scripted_state"

// sub_5B0220
als::basic_rule_list::~basic_rule_list() {
    if (rules) {
        for (u32 index = 0; index < num_rules; ++index)
            delete rules[index];

        memory::heap::free(rules);

        rules = nullptr;
    }

    num_rules = 0;
}

// inlined @ sub_5F48C0
// inlined @ sub_5F47B0
// inlined @ sub_5F7220
// inlined @ sub_5F6E70
// inlined @ sub_5F73C0
void als::basic_rule_list::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    custom_unmash(mash_info, containing_class_ptr, buffer);
}

// sub_5E28F0
void als::basic_rule_list::custom_unmash(mash::mash_info_struct* mash_info,
                                         void*                   containing_class_ptr,
                                         mash::buffer_type       buffer) {

    if (!num_rules) {
        rules = nullptr;

        return;
    }

    rules = (transition_rule_base**)mash_info->read_from_buffer(buffer, (i32)(num_rules * sizeof(transition_rule_base*)), 4);

    for (u32 index = 0; index < num_rules; ++index) {
        transition_rule_base* &rule = rules[index];

        // the slot holds the rule's class until the rule is read over it
        switch ((e_rule_class)(u32)rule) {
            case e_rule_class::number_compare_equal:
                mash_info->unmash_class((number_compare_equal_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::vector_component_number_compare_equal:
                mash_info->unmash_class((vector_component_number_compare_equal_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::number_compare_not_equal:
                mash_info->unmash_class((number_compare_not_equal_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::vector_component_number_compare_not_equal:
                mash_info->unmash_class((vector_component_number_compare_not_equal_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::number_compare_greater_than:
                mash_info->unmash_class((number_compare_greater_than_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::vector_component_number_compare_greater_than:
                mash_info->unmash_class((vector_component_number_compare_greater_than_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::number_compare_less_than:
                mash_info->unmash_class((number_compare_less_than_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::vector_component_number_compare_less_than:
                mash_info->unmash_class((vector_component_number_compare_less_than_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::number_compare_greater_than_or_equal_to:
                mash_info->unmash_class((number_compare_greater_than_or_equal_to_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::vector_component_number_compare_greater_than_or_equal_to:
                mash_info->unmash_class((vector_component_number_compare_greater_than_or_equal_to_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::number_compare_less_than_or_equal_to:
                mash_info->unmash_class((number_compare_less_than_or_equal_to_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::vector_component_number_compare_less_than_or_equal_to:
                mash_info->unmash_class((vector_component_number_compare_less_than_or_equal_to_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::param_compare:
                mash_info->unmash_class((param_compare_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::vector_component_param_compare:
                mash_info->unmash_class((vector_component_param_compare_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::vector_param_compare:
                mash_info->unmash_class((vector_param_compare_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::vector_component_vector_param_compare:
                mash_info->unmash_class((vector_component_vector_param_compare_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::hash_equal:
                mash_info->unmash_class((hash_equal_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::hash_not_equal:
                mash_info->unmash_class((hash_not_equal_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::hash_param_equal:
                mash_info->unmash_class((hash_param_equal_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::hash_param_not_equal:
                mash_info->unmash_class((hash_param_not_equal_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::bounded_number:
                mash_info->unmash_class((bounded_number_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::inverse_bounded_number:
                mash_info->unmash_class((inverse_bounded_number_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::vector_component_bounded_number:
                mash_info->unmash_class((vector_component_bounded_number_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::vector_component_inverse_bounded_number:
                mash_info->unmash_class((vector_component_inverse_bounded_number_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::upgrade_number_compare:
                mash_info->unmash_class((upgrade_number_compare_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::upgrade_bounded_number:
                mash_info->unmash_class((upgrade_bounded_number_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::upgrade_inverse_bounded_number:
                mash_info->unmash_class((upgrade_inverse_bounded_number_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::als_state_number_compare:
                mash_info->unmash_class((als_state_number_compare_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::als_state_vector_component_number_compare:
                mash_info->unmash_class((als_state_vector_component_number_compare_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::als_state_hash_equal:
                mash_info->unmash_class((als_state_hash_equal_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::als_state_hash_not_equal:
                mash_info->unmash_class((als_state_hash_not_equal_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::als_state_bounded_number:
                mash_info->unmash_class((als_state_bounded_number_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::als_state_inverse_bounded_number:
                mash_info->unmash_class((als_state_inverse_bounded_number_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::als_state_vector_component_bounded_number:
                mash_info->unmash_class((als_state_vector_component_bounded_number_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::als_state_vector_component_inverse_bounded_number:
                mash_info->unmash_class((als_state_vector_component_inverse_bounded_number_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::als_state_number_param_compare:
                mash_info->unmash_class((als_state_number_param_compare_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::als_state_number_param_vector_component_compare:
                mash_info->unmash_class((als_state_number_param_vector_component_compare_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ai_number_compare:
                mash_info->unmash_class((ai_number_compare_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ai_vector_component_number_compare:
                mash_info->unmash_class((ai_vector_component_number_compare_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ai_hash_equal:
                mash_info->unmash_class((ai_hash_equal_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ai_hash_not_equal:
                mash_info->unmash_class((ai_hash_not_equal_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ai_bounded_number:
                mash_info->unmash_class((ai_bounded_number_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ai_inverse_bounded_number:
                mash_info->unmash_class((ai_inverse_bounded_number_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ai_vector_component_bounded_number:
                mash_info->unmash_class((ai_vector_component_bounded_number_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ai_vector_component_inverse_bounded_number:
                mash_info->unmash_class((ai_vector_component_inverse_bounded_number_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ai_number_param_compare:
                mash_info->unmash_class((ai_number_param_compare_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ai_number_param_vector_component_compare:
                mash_info->unmash_class((ai_number_param_vector_component_compare_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::target_ai_number_compare:
                mash_info->unmash_class((target_ai_number_compare_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::target_ai_vector_component_number_compare:
                mash_info->unmash_class((target_ai_vector_component_number_compare_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::target_ai_hash_equal:
                mash_info->unmash_class((target_ai_hash_equal_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::target_ai_hash_not_equal:
                mash_info->unmash_class((target_ai_hash_not_equal_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::target_ai_bounded_number:
                mash_info->unmash_class((target_ai_bounded_number_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::target_ai_inverse_bounded_number:
                mash_info->unmash_class((target_ai_inverse_bounded_number_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::target_ai_vector_component_bounded_number:
                mash_info->unmash_class((target_ai_vector_component_bounded_number_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::target_ai_vector_component_inverse_bounded_number:
                mash_info->unmash_class((target_ai_vector_component_inverse_bounded_number_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::target_ai_number_param_compare:
                mash_info->unmash_class((target_ai_number_param_compare_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::target_ai_number_param_vector_component_compare:
                mash_info->unmash_class((target_ai_number_param_vector_component_compare_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ai_inode_number_compare:
                mash_info->unmash_class((ai_inode_number_compare_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ai_inode_vector_component_number_compare:
                mash_info->unmash_class((ai_inode_vector_component_number_compare_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ai_inode_hash_equal:
                mash_info->unmash_class((ai_inode_hash_equal_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ai_inode_hash_not_equal:
                mash_info->unmash_class((ai_inode_hash_not_equal_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ai_inode_bounded_number:
                mash_info->unmash_class((ai_inode_bounded_number_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ai_inode_inverse_bounded_number:
                mash_info->unmash_class((ai_inode_inverse_bounded_number_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ai_inode_vector_component_bounded_number:
                mash_info->unmash_class((ai_inode_vector_component_bounded_number_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ai_inode_vector_component_inverse_bounded_number:
                mash_info->unmash_class((ai_inode_vector_component_inverse_bounded_number_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ai_inode_number_param_compare:
                mash_info->unmash_class((ai_inode_number_param_compare_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ai_inode_number_param_vector_component_compare:
                mash_info->unmash_class((ai_inode_number_param_vector_component_compare_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ifc_number_compare:
                mash_info->unmash_class((ifc_number_compare_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ifc_vector_component_number_compare:
                mash_info->unmash_class((ifc_vector_component_number_compare_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ifc_hash_equal:
                mash_info->unmash_class((ifc_hash_equal_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ifc_hash_not_equal:
                mash_info->unmash_class((ifc_hash_not_equal_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ifc_bounded_number:
                mash_info->unmash_class((ifc_bounded_number_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ifc_inverse_bounded_number:
                mash_info->unmash_class((ifc_inverse_bounded_number_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ifc_vector_component_bounded_number:
                mash_info->unmash_class((ifc_vector_component_bounded_number_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ifc_vector_component_inverse_bounded_number:
                mash_info->unmash_class((ifc_vector_component_inverse_bounded_number_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ifc_number_param_compare:
                mash_info->unmash_class((ifc_number_param_compare_rule*&)rule, containing_class_ptr, buffer);
                break;
            case e_rule_class::ifc_number_param_vector_component_compare:
                mash_info->unmash_class((ifc_number_param_vector_component_compare_rule*&)rule, containing_class_ptr, buffer);
                break;
            default:
                break;
        }
    }
}

// inlined @ sub_5F47B0
void als::basic_rule_data::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    basic_rule_list::unmash(mash_info, containing_class_ptr, buffer);

    custom_unmash(mash_info, containing_class_ptr, buffer);
}

// inlined @ sub_5F47B0
void als::basic_rule_data::custom_unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    destinations = (destination*)mash_info->read_from_buffer(buffer, (i32)(num_destinations * sizeof(destination)), 4);
}

// inlined @ sub_5F71E0
// inlined @ sub_5F7F30
// inlined @ sub_5F80A0
// inlined @ sub_5F8120
void als::trans_rule::construct_mashed_class() {
    actions.construct_mashed_class();
}

// inlined @ sub_5F5400
// inlined @ sub_5F5490
// inlined @ sub_5F5520
void als::trans_rule::destruct_mashed_class() {
    actions.destruct_mashed_class();
}

// inlined @ sub_5F7220
// inlined @ sub_5F6E70
// inlined @ sub_5F73C0
void als::trans_rule::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    basic_rule_list::unmash(mash_info, containing_class_ptr, buffer);

    actions.unmash(mash_info, this, buffer);
}

// inlined @ sub_5F71E0
void als::explicit_trans::construct_mashed_class() {
    trans_rule::construct_mashed_class();

    category.construct_mashed_class();
}

// inlined @ sub_5F5490
void als::explicit_trans::destruct_mashed_class() {
    category.destruct_mashed_class();

    trans_rule::destruct_mashed_class();
}

// inlined @ sub_5F6E70
void als::explicit_trans::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    trans_rule::unmash(mash_info, containing_class_ptr, buffer);

    category.unmash(mash_info, this, buffer);
}

// sub_5F80A0
void als::scripted_state::construct_mashed_class() {
    state::construct_mashed_class();

    anim_name           .construct_mashed_class();
    trans_group_ref_list.construct_mashed_class();
    implicit_trans_list .construct_mashed_class();
    explicit_trans_list .construct_mashed_class();
    speed_range_list    .construct_mashed_class();
}

// sub_5F7020
void als::scripted_state::destruct_mashed_class() {
    anim_name           .destruct_mashed_class();
    trans_group_ref_list.destruct_mashed_class();
    implicit_trans_list .destruct_mashed_class();
    explicit_trans_list .destruct_mashed_class();
    speed_range_list    .destruct_mashed_class();

    state::destruct_mashed_class();
}

// sub_5F8220
void als::scripted_state::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    state::unmash(mash_info, containing_class_ptr, buffer);

    anim_name           .unmash(mash_info, this, buffer);
    trans_group_ref_list.unmash(mash_info, this, buffer);
    implicit_trans_list .unmash(mash_info, this, buffer);
    explicit_trans_list .unmash(mash_info, this, buffer);
    speed_range_list    .unmash(mash_info, this, buffer);
}

// sub_5F7F80
mash::virtual_types_key als::scripted_state::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5F7F90
bool als::scripted_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (state::get_virtual_type_key() == parent_class)
        return true;

    if (state::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5F7FC0
i32 als::scripted_state::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5B02F0
mash::virtual_types_key als::scripted_state::get_mocomp_type() const {
    return mash::invalid_type_key;
}

// sub_5D6E90
als::request_data als::scripted_state::do_implicit_trans(animation_logic_system* the_als, state_machine* the_sm) const {
    using do_implicit_trans_t = request_data* (__thiscall*)(const scripted_state*, request_data*, animation_logic_system*, state_machine*);

    request_data result;
    ((do_implicit_trans_t)retail::sub_5D6E90)(this, &result, the_als, the_sm);

    return result;
}

// sub_5D7090
als::request_data als::scripted_state::do_explicit_trans(animation_logic_system* the_als,
                                                         state_machine*          the_sm,
                                                         category_id             desired_category) const {

    using do_explicit_trans_t = request_data* (__thiscall*)(const scripted_state*, request_data*, animation_logic_system*, state_machine*, category_id);

    request_data result;
    ((do_explicit_trans_t)retail::sub_5D7090)(this, &result, the_als, the_sm, desired_category);

    return result;
}

// sub_5D7270
nal_anim_id als::scripted_state::get_nal_anim_name() const {
    return anim_name;
}

// sub_5D72C0
f32 als::scripted_state::get_secondary_playback_speed(f32 anim_time, f32 anim_duration) const {
    return ((f32 (__thiscall*)(const scripted_state*, f32, f32))retail::sub_5D72C0)(this, anim_time, anim_duration);
}

// sub_5F8110
void als::base_layer_scripted_state::construct_mashed_class() {
    scripted_state::construct_mashed_class();
}

// sub_5F70B0
void als::base_layer_scripted_state::destruct_mashed_class() {
    scripted_state::destruct_mashed_class();
}

// sub_5F82C0
void als::base_layer_scripted_state::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    scripted_state::unmash(mash_info, containing_class_ptr, buffer);
}

// sub_723BC0
mash::virtual_types_key als::base_layer_scripted_state::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_723BD0
bool als::base_layer_scripted_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (scripted_state::get_virtual_type_key() == parent_class)
        return true;

    if (scripted_state::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_723C00
i32 als::base_layer_scripted_state::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5B0300
mash::virtual_types_key als::base_layer_scripted_state::get_mocomp_type() const {
    return mocomp_type;
}
