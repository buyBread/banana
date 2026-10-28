#include "retail.hh"
#include "game/als/internal.hh"
#include "game/als/meta_anim_swing.hh"
#include "game/als/meta_linear_blend.hh"
#include "game/als/mocomp.hh"
#include "game/als/rules.hh"
#include "game/als/script_state.hh"
#include "game/meta_anims.hh"
#include "game/sm_mash.hh"
#include "game/wds/ai/param_block.hh"
#include "treyarch/mash/virtual_base.hh"

using namespace treyarch;

// sub_72BCE0
void treyarch::sm_generate_vtable() {
    references::generating_vtable.write(true);

    /* the source-owned part of the generated list (sub_723DA0), under its guard and in its order;
       it goes first, so the list's own registrations of these keys are refused and the source vtables stay */
    if (!references::vtables_generated.read()) {
        mash::mash_virtual_base::register_class<ai_puppet_anim_by_filename>();
        mash::mash_virtual_base::register_class<combat_block_meta_anim>();
        mash::mash_virtual_base::register_class<combat_dodge_meta_anim>();
        mash::mash_virtual_base::register_class<flight_mocomp>();
        mash::mash_virtual_base::register_class<als::animation_blends_list>();
        mash::mash_virtual_base::register_class<als::layer_state_machine_shared>();
        mash::mash_virtual_base::register_class<als::state_machine_shared>();
        mash::mash_virtual_base::register_class<als::als_meta_anim_swing>();
        mash::mash_virtual_base::register_class<als::als_meta_linear_blend>();
        mash::mash_virtual_base::register_class<als::motion_compensator>();
        mash::mash_virtual_base::register_class<als::action_face_only>();
        mash::mash_virtual_base::register_class<als::action_move_and_face>();
        mash::mash_virtual_base::register_class<als::action_move_only>();
        mash::mash_virtual_base::register_class<als::attach_hand_to_point>();
        mash::mash_virtual_base::register_class<als::biped_physics_mocomp>();
        mash::mash_virtual_base::register_class<als::combat_face_only>();
        mash::mash_virtual_base::register_class<als::combat_face_only_with_attack>();
        mash::mash_virtual_base::register_class<als::combat_move_and_face>();
        mash::mash_virtual_base::register_class<als::combat_move_and_face_with_attack>();
        mash::mash_virtual_base::register_class<als::combat_move_only>();
        mash::mash_virtual_base::register_class<als::combat_pole_swing_attach>();
        mash::mash_virtual_base::register_class<als::combat_radial_move_and_face>();
        mash::mash_virtual_base::register_class<als::constant_face_only>();
        mash::mash_virtual_base::register_class<als::constant_move_and_face>();
        mash::mash_virtual_base::register_class<als::constant_move_hand_to_point>();
        mash::mash_virtual_base::register_class<als::constant_move_only>();
        mash::mash_virtual_base::register_class<als::crawl_transition>();
        mash::mash_virtual_base::register_class<als::direct_mocomp>();
        mash::mash_virtual_base::register_class<als::ikcrawl_mocomp>();
        mash::mash_virtual_base::register_class<als::initial3D_velocity_orientation>();
        mash::mash_virtual_base::register_class<als::initial_velocity_orientation>();
        mash::mash_virtual_base::register_class<als::input_orientation>();
        mash::mash_virtual_base::register_class<als::momentum_combat_move_and_face>();
        mash::mash_virtual_base::register_class<als::momentum_grapple_unsynched>();
        mash::mash_virtual_base::register_class<als::momentum_sync_move_and_face>();
        mash::mash_virtual_base::register_class<als::move_and_face>();
        mash::mash_virtual_base::register_class<als::move_and_face_no_anim_movement>();
        mash::mash_virtual_base::register_class<als::relative_orientation>();
        mash::mash_virtual_base::register_class<als::relative_orientation_on_foot>();
        mash::mash_virtual_base::register_class<als::relative_strafe_mocomp>();
        mash::mash_virtual_base::register_class<als::relative_strafe_mocomp_on_foot>();
        mash::mash_virtual_base::register_class<als::rvb_crawl_transition>();
        mash::mash_virtual_base::register_class<als::signal_face_only>();
        mash::mash_virtual_base::register_class<als::signal_move_and_face>();
        mash::mash_virtual_base::register_class<als::signal_move_only>();
        mash::mash_virtual_base::register_class<als::simple_orientation>();
        mash::mash_virtual_base::register_class<als::simple_orientation_on_foot>();
        mash::mash_virtual_base::register_class<als::strafe_mocomp>();
        mash::mash_virtual_base::register_class<als::strafe_mocomp_on_foot>();
        mash::mash_virtual_base::register_class<als::strict_simple_orientation>();
        mash::mash_virtual_base::register_class<als::sturn_sync_move_and_face>();
        mash::mash_virtual_base::register_class<als::swing_attack_move_and_face>();
        mash::mash_virtual_base::register_class<als::sync_move_and_face>();
        mash::mash_virtual_base::register_class<als::towed_orientation>();
        mash::mash_virtual_base::register_class<als::use_anim_only>();
        mash::mash_virtual_base::register_class<als::velocity_orientation>();
        mash::mash_virtual_base::register_class<als::vertical_orientation>();
        mash::mash_virtual_base::register_class<als::wall_recover_move_and_face>();
        mash::mash_virtual_base::register_class<als::wallsprint_mocomp>();
        mash::mash_virtual_base::register_class<als::bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::constant_number_compare_rule>();
        mash::mash_virtual_base::register_class<als::hash_equal_rule>();
        mash::mash_virtual_base::register_class<als::hash_not_equal_rule>();
        mash::mash_virtual_base::register_class<als::hash_param_equal_rule>();
        mash::mash_virtual_base::register_class<als::hash_param_not_equal_rule>();
        mash::mash_virtual_base::register_class<als::inverse_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::number_compare_equal_rule>();
        mash::mash_virtual_base::register_class<als::number_compare_greater_than_or_equal_to_rule>();
        mash::mash_virtual_base::register_class<als::number_compare_greater_than_rule>();
        mash::mash_virtual_base::register_class<als::number_compare_less_than_or_equal_to_rule>();
        mash::mash_virtual_base::register_class<als::number_compare_less_than_rule>();
        mash::mash_virtual_base::register_class<als::number_compare_not_equal_rule>();
        mash::mash_virtual_base::register_class<als::param_compare_rule>();
        mash::mash_virtual_base::register_class<als::transition_rule_base>();
        mash::mash_virtual_base::register_class<als::vector_component_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::vector_component_constant_number_compare_rule>();
        mash::mash_virtual_base::register_class<als::vector_component_inverse_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::vector_component_number_compare_equal_rule>();
        mash::mash_virtual_base::register_class<als::vector_component_number_compare_greater_than_or_equal_to_rule>();
        mash::mash_virtual_base::register_class<als::vector_component_number_compare_greater_than_rule>();
        mash::mash_virtual_base::register_class<als::vector_component_number_compare_less_than_or_equal_to_rule>();
        mash::mash_virtual_base::register_class<als::vector_component_number_compare_less_than_rule>();
        mash::mash_virtual_base::register_class<als::vector_component_number_compare_not_equal_rule>();
        mash::mash_virtual_base::register_class<als::vector_component_param_compare_rule>();
        mash::mash_virtual_base::register_class<als::vector_component_vector_param_compare_rule>();
        mash::mash_virtual_base::register_class<als::vector_param_compare_rule>();
        mash::mash_virtual_base::register_class<als::ai_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::ai_hash_equal_rule>();
        mash::mash_virtual_base::register_class<als::ai_hash_not_equal_rule>();
        mash::mash_virtual_base::register_class<als::ai_inode_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::ai_inode_hash_equal_rule>();
        mash::mash_virtual_base::register_class<als::ai_inode_hash_not_equal_rule>();
        mash::mash_virtual_base::register_class<als::ai_inode_inverse_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::ai_inode_number_compare_rule>();
        mash::mash_virtual_base::register_class<als::ai_inode_number_param_compare_rule>();
        mash::mash_virtual_base::register_class<als::ai_inode_number_param_vector_component_compare_rule>();
        mash::mash_virtual_base::register_class<als::ai_inode_vector_component_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::ai_inode_vector_component_inverse_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::ai_inode_vector_component_number_compare_rule>();
        mash::mash_virtual_base::register_class<als::ai_inverse_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::ai_number_compare_rule>();
        mash::mash_virtual_base::register_class<als::ai_number_param_compare_rule>();
        mash::mash_virtual_base::register_class<als::ai_number_param_vector_component_compare_rule>();
        mash::mash_virtual_base::register_class<als::ai_vector_component_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::ai_vector_component_inverse_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::ai_vector_component_number_compare_rule>();
        mash::mash_virtual_base::register_class<als::als_state_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::als_state_hash_equal_rule>();
        mash::mash_virtual_base::register_class<als::als_state_hash_not_equal_rule>();
        mash::mash_virtual_base::register_class<als::als_state_inverse_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::als_state_number_compare_rule>();
        mash::mash_virtual_base::register_class<als::als_state_number_param_compare_rule>();
        mash::mash_virtual_base::register_class<als::als_state_number_param_vector_component_compare_rule>();
        mash::mash_virtual_base::register_class<als::als_state_vector_component_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::als_state_vector_component_inverse_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::als_state_vector_component_number_compare_rule>();
        mash::mash_virtual_base::register_class<als::class_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::class_constant_number_compare_rule>();
        mash::mash_virtual_base::register_class<als::class_hash_compare_rule>();
        mash::mash_virtual_base::register_class<als::class_param_compare_rule>();
        mash::mash_virtual_base::register_class<als::class_vector_component_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::class_vector_component_constant_number_compare_rule>();
        mash::mash_virtual_base::register_class<als::class_vector_component_param_compare_rule>();
        mash::mash_virtual_base::register_class<als::ifc_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::ifc_hash_equal_rule>();
        mash::mash_virtual_base::register_class<als::ifc_hash_not_equal_rule>();
        mash::mash_virtual_base::register_class<als::ifc_inverse_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::ifc_number_compare_rule>();
        mash::mash_virtual_base::register_class<als::ifc_number_param_compare_rule>();
        mash::mash_virtual_base::register_class<als::ifc_number_param_vector_component_compare_rule>();
        mash::mash_virtual_base::register_class<als::ifc_vector_component_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::ifc_vector_component_inverse_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::ifc_vector_component_number_compare_rule>();
        mash::mash_virtual_base::register_class<als::target_ai_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::target_ai_hash_equal_rule>();
        mash::mash_virtual_base::register_class<als::target_ai_hash_not_equal_rule>();
        mash::mash_virtual_base::register_class<als::target_ai_inverse_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::target_ai_number_compare_rule>();
        mash::mash_virtual_base::register_class<als::target_ai_number_param_compare_rule>();
        mash::mash_virtual_base::register_class<als::target_ai_number_param_vector_component_compare_rule>();
        mash::mash_virtual_base::register_class<als::target_ai_vector_component_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::target_ai_vector_component_inverse_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::target_ai_vector_component_number_compare_rule>();
        mash::mash_virtual_base::register_class<als::upgrade_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::upgrade_inverse_bounded_number_rule>();
        mash::mash_virtual_base::register_class<als::upgrade_number_compare_rule>();
        mash::mash_virtual_base::register_class<als::base_layer_scripted_state>();
        mash::mash_virtual_base::register_class<als::scripted_category>();
        mash::mash_virtual_base::register_class<als::scripted_state>();
        mash::mash_virtual_base::register_class<als::scripted_trans_group>();
        mash::mash_virtual_base::register_class<param_class>();
        mash::mash_virtual_base::register_class<param_entity>();
        mash::mash_virtual_base::register_class<param_float>();
        mash::mash_virtual_base::register_class<param_float_variance>();
        mash::mash_virtual_base::register_class<param_hash>();
        mash::mash_virtual_base::register_class<param_int>();
        mash::mash_virtual_base::register_class<param_pointer>();
        mash::mash_virtual_base::register_class<param_string>();
        mash::mash_virtual_base::register_class<param_vector3d>();
        mash::mash_virtual_base::register_class<generic_anim_by_filename>();
    }

    retail::sub_723DA0();

    references::generating_vtable.write(false);
}
