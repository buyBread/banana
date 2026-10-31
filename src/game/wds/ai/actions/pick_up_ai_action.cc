#include "retail.hh"
#include "game/wds/ai/actions/pick_up_ai_action.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/entity/actor.hh"

using namespace treyarch;

// sub_4A1CD0
pick_up_ai_action::pick_up_ai_action(ai_core* owner, const string_hash &my_hash) : play_anim_ai_action(owner, my_hash),
                                                                                   target(),
                                                                                   finished(false),
                                                                                   engaged_slave(false),
                                                                                   unk_026(false) {}

// sub_4BF060
ai_action_nugget::e_action_exit_status pick_up_ai_action::frame_advance(f32 delta_t) {
    e_action_exit_status status = play_anim_ai_action::frame_advance(delta_t);

    if (status == no_exit)
        return unk_026 ? should_exit : no_exit;

    auto* carry = (std_carry_inode*)get_core()->get_info_node(info_node_type_std_carry);

    if (auto* the_target = (actor*)target.resolve()) {
        if (the_target->get_ai_core()) {
            if (auto* slave = (std_carry_inode*)the_target->get_ai_core()->get_info_node(info_node_type_std_carry))
                carry->add_slave(slave, false);
        }
    }

    finished = true;

    return status;
}

// sub_4A1DA0
void pick_up_ai_action::activate() {
    play_anim_ai_action::activate();

    finished      = false;
    engaged_slave = false;

    auto* the_target = (actor*)target.resolve();

    if (!the_target)
        return;

    if (!the_target->get_ai_core()->is_in_slave_mode() &&
        the_target->get_ai_core()->attempt_to_engage_slave_mode(get_core()->get_actor()->my_handle, (als::layer_id)9))

        engaged_slave = true;

    animation_controller* master_ctrl = get_core()->get_actor()->anim_ctrl;

    if (animation_controller* target_ctrl = the_target->anim_ctrl)
        retail::sub_5F5E90(target_ctrl, (u32**)master_ctrl, nullptr); // animation_controller::set_master

    if (auto* target_als = (als_inode*)the_target->get_ai_core()->get_info_node(info_node_type_als)) {
        string_hash picked_up_by_ai;
        picked_up_by_ai.initialize(mash::ALLOCATED, "Picked_Up_By_AI");

        retail::sub_457B20((u32*)target_als, (i32)picked_up_by_ai.source_hash_code, 9, 1, 1, nullptr, 0);
    }

    the_target->raise_event(references::begin_pick_up.read());
}

// slot 3
void pick_up_ai_action::deactivate(bool on_finalize) {
    ((void (__thiscall*)(pick_up_ai_action*, bool))retail::sub_4A1EA0)(this, on_finalize);
}

// slot 11
void pick_up_ai_action::setup(std_carry_inode* _slave) {
    ((void (__thiscall*)(pick_up_ai_action*, std_carry_inode*))retail::sub_4A1D00)(this, _slave);
}
