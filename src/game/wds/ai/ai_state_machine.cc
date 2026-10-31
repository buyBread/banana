#include "retail.hh"
#include "game/wds/ai/ai_state_machine.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/info_node.hh"

using namespace treyarch;

// sub_4DAA10
// inlined @ sub_4DF760
bool enhanced_state::can_handle_exit_code(e_state_exit_code the_exit_code, bool include_always) {
    u32 word = the_exit_code >> 5;
    u32 bit  = 1u << (the_exit_code & 0x1F);

    // only the answer without the always params is kept
    if (exit_codes_checked[word] & bit) {
        if (exit_codes_handled[word] & bit)
            return true;

        if (!include_always)
            return false;
    }

    bool can_handle = false;

    const param_block &params = my_mashed_state->my_params;

    string_hash none;
    none.initialize(mash::ALLOCATED);

    string_hash to_state = params.get_optional_pb_hash(references::to_state_hashes.get()[the_exit_code], none, nullptr);

    if (to_state != none && my_state_machine->can_switch_to_state(to_state))
        can_handle = true;
    else {
        to_state = params.get_optional_pb_hash(references::to_state_always_hash.read(), none, nullptr);

        if (to_state != none && my_state_machine->can_switch_to_state(to_state))
            can_handle = true;
        else if (params.does_parameter_exist(references::exit_layer_hashes.get()[the_exit_code]))
            can_handle = true;
        else if (include_always && params.does_parameter_exist(references::exit_layer_always_hash.read()))
            can_handle = true;
    }

    if (!include_always) {
        exit_codes_checked[word] |= bit;

        if (can_handle)
            exit_codes_handled[word] |= bit;
    }

    return can_handle;
}

// sub_4C3A80
resource_key ai_state_machine::get_name() const {
    return my_graph->get_name();
}

bool ai_state_machine::can_switch_to_state(string_hash id) const {
    return retail::sub_4DA950((i32)this, (i32)id.source_hash_code);
}

// sub_4C6950
void ai_state_machine::request_exit() {
    my_curr_mode = blocked_on_all_children;

    for (u32 index = 0; index < my_children.size(); ++index)
        my_children[index]->request_exit();
}

// sub_4C6990
void ai_state_machine::external_request_exit() {
    if (my_curr_mode == blocked_on_all_children  ||
        my_curr_mode == exiting_wait_on_children ||
        my_curr_mode == processing_exit_request)

        return;

    request_exit();
}

// sub_4DF760
void ai_state_machine::process_mode(f32 delta_t) {
    switch (my_curr_mode) {
        case initial_mode: {
            u32 &guard = references::sync_state_exit_code_guard.get();

            if (!(guard & 1)) {
                guard |= 1;

                references::sync_state_exit_code.get().initialize(mash::ALLOCATED, "sync_state_exit_code");
                retail::sub_ADE5FD(retail::sub_B6A810); // the game's CRT atexit
            }

            // the als's sync_state_exit_code hands the first state an exit code right away
            auto*       als_node = (als_inode*)my_core->get_info_node(info_node_type_als);
            string_hash none;
            string_hash sync_code_name;

            retail::sub_44F550((u32**)als_node, // als_inode::get_optional_pb_hash
                               (i32)&sync_code_name,
                               (u32*)&references::sync_state_exit_code.get(),
                               (u32*)&none,
                               0,
                               0);

            auto sync_code = (e_state_exit_code)retail::sub_4C3AE0((u32*)&sync_code_name);

            my_curr_state = (base_state*)retail::sub_4D70B0(my_graph->my_initial_state->my_state_type); // base_state::create_subclass
            my_curr_state->activate(this, my_graph->my_initial_state, nullptr, nullptr);

            my_curr_mode = state_advance;

            if (sync_code == state_exit_code_no_exit)
                process_mode(delta_t);
            else if (my_curr_state->is_enhanced_state() &&
                     ((enhanced_state*)my_curr_state)->can_handle_exit_code(sync_code))

                process_exit_code(delta_t, sync_code);

            break;
        }

        case state_advance:
        case processing_exit_request:
            advance_curr_state(delta_t);
            break;

        case blocked_on_child_machine:
            if (!my_blocked_machine) {
                my_curr_mode = state_advance;
                process_mode(delta_t);
            }

            break;

        case blocked_on_all_children:
            if (!my_children.size()) {
                base_state* exiting_state = my_curr_state;

                my_curr_mode = processing_exit_request;

                if (exiting_state)
                    process_exit_code(delta_t, state_exit_code_exit_request);

                // a state that handled the request runs on
                if (exiting_state != my_curr_state)
                    process_mode(delta_t);
                else
                    my_core->do_machine_exit(this);
            }

            break;

        case exiting_wait_on_children:
            if (!my_children.size())
                my_core->do_machine_exit(this);

            break;
    }
}

// sub_4DEDD0
void ai_state_machine::advance_curr_state(f32 delta_t) {
    e_state_exit_code the_exit_code = my_curr_state->frame_advance(delta_t);

    if (the_exit_code != state_exit_code_no_exit)
        process_exit_code(delta_t, the_exit_code);
}

void ai_state_machine::process_exit_code(f32 delta_t, e_state_exit_code the_exit_code) {
    retail::sub_4DD9D0((i32)this, delta_t, (i32)the_exit_code);
}
