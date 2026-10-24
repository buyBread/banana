#include "treyarch/game/als/script_state.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;

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
