#include "retail.hh"
#include "treyarch/game/als/internal.hh"
#include "treyarch/game/als/meta_anim_swing.hh"
#include "treyarch/game/als/meta_linear_blend.hh"
#include "treyarch/game/als/script_state.hh"
#include "treyarch/game/meta_anims.hh"
#include "treyarch/game/sm_mash.hh"
#include "treyarch/shared/mash/virtual_base.hh"

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
        mash::mash_virtual_base::register_class<als::animation_blends_list>();
        mash::mash_virtual_base::register_class<als::layer_state_machine_shared>();
        mash::mash_virtual_base::register_class<als::state_machine_shared>();
        mash::mash_virtual_base::register_class<als::als_meta_anim_swing>();
        mash::mash_virtual_base::register_class<als::als_meta_linear_blend>();
        mash::mash_virtual_base::register_class<als::base_layer_scripted_state>();
        mash::mash_virtual_base::register_class<als::scripted_category>();
        mash::mash_virtual_base::register_class<als::scripted_state>();
        mash::mash_virtual_base::register_class<als::scripted_trans_group>();
        mash::mash_virtual_base::register_class<generic_anim_by_filename>();
    }

    retail::sub_723DA0();

    references::generating_vtable.write(false);
}
