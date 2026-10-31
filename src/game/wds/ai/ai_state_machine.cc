#include "retail.hh"
#include "game/wds/ai/ai_state_machine.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/entity/actor.hh"

using namespace treyarch;

string_hash launch_layer_state::my_type_key { 0x62F59477 }; // "launch_layer_state"

// inlined @ sub_4C4AE0
// inlined @ sub_4D70B0
void base_state::initialize(mash::allocation_scope) {
    my_state_machine = nullptr;
    my_mashed_state  = nullptr;
}

// sub_4C4930
ai_core* base_state::get_core() const {
    return my_state_machine->my_core;
}

// sub_4D2A40
actor* base_state::get_actor() const {
    return my_state_machine->my_core->my_actor;
}

// ==== enhanced_state ==== //

// slot 0
enhanced_state::~enhanced_state() {
    retail::sub_486520((u32*)this);
}

// slot 1
void enhanced_state::construct_mashed_class() {
    ((void (__thiscall*)(enhanced_state*))retail::sub_4C4B00)(this);
}

// slot 2
void enhanced_state::destruct_mashed_class() {
    ((void (__thiscall*)(enhanced_state*))retail::sub_4DA200)(this);
}

// sub_7AE090
void enhanced_state::unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}

// slot 4
mash::virtual_types_key enhanced_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const enhanced_state*))retail::sub_486570)(this);
}

// sub_689610
void enhanced_state::copy_values(const mash_virtual_base*) {}

// slot 6
bool enhanced_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const enhanced_state*, mash::virtual_types_key))retail::sub_486580)(this, parent_class);
}

// sub_4D6250
i32 enhanced_state::get_mash_sizeof() const {
    return sizeof(*this);
}

// slot 9
void enhanced_state::activate(      ai_state_machine* the_state_machine,
                              const mashed_state*     the_mashed_state,
                              const mashed_state*     prev_mashed_state,
                              const param_block*      extra_params) {

    ((void (__thiscall*)(enhanced_state*,
                         ai_state_machine*,
                         const mashed_state*,
                         const mashed_state*,
                         const param_block*))retail::sub_4DC120)(this,
                                                                 the_state_machine,
                                                                 the_mashed_state,
                                                                 prev_mashed_state,
                                                                 extra_params);
}

// slot 10
void enhanced_state::deactivate(const mashed_state* next_mashed_state) {
    ((void (__thiscall*)(enhanced_state*, const mashed_state*))retail::sub_4CE510)(this, next_mashed_state);
}

// slot 11
e_state_exit_code enhanced_state::frame_advance(f32 delta_t) {
    return ((e_state_exit_code (__thiscall*)(enhanced_state*, f32))retail::sub_4CD010)(this, delta_t);
}

// sub_689610
void enhanced_state::get_info_node_list(info_node_desc_list&) const {}

// sub_689610
void enhanced_state::get_state_graph_list(state_graph_list&) const {}

// sub_667F80
bool enhanced_state::is_enhanced_state() const {
    return true;
}

// sub_71F870
bool enhanced_state::is_combat_state() const {
    return false;
}

// sub_71F870
bool enhanced_state::unk_016() const {
    return false;
}

// sub_71F870
bool enhanced_state::unk_017() const {
    return false;
}

// slot 18
state_trans_action enhanced_state::process_exit_code(f32 delta_t, e_state_exit_code the_exit_code) const {
    state_trans_action action;

    ((state_trans_action* (__thiscall*)(const enhanced_state*,
                                        state_trans_action*,
                                        f32,
                                        e_state_exit_code))retail::sub_4CAEA0)(this,
                                                                               &action,
                                                                               delta_t,
                                                                               the_exit_code);

    return action;
}

// sub_A944D0
u32 enhanced_state::unk_019() const {
    return 0;
}

// inlined @ sub_4C4AE0
// inlined @ sub_4D70B0
void enhanced_state::initialize(mash::allocation_scope) {
    default_transitions = nullptr;
}

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
// inlined @ sub_4DBD60
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

            my_curr_state = base_state::create_subclass(my_graph->my_initial_state->my_state_type);
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

// sub_4DD9D0
void ai_state_machine::process_exit_code(f32 delta_t, e_state_exit_code the_exit_code) {
    state_trans_action action = my_curr_state->process_exit_code(delta_t, the_exit_code);

    switch (action.the_action) {
        case state_action_goto_state:
            transition_state(action.dest_state_name, action.extra_params, the_exit_code);
            break;

        case state_action_return:
            process_return();
            break;

        case state_action_machine_exit:
            my_exit_status = action.the_exit_code;

            if (my_children.size()) {
                my_curr_mode = exiting_wait_on_children;

                for (u32 index = 0; index < my_children.size(); ++index)
                    my_children[index]->request_exit();
            } else
                my_core->do_machine_exit(this);

            break;
    }
}

// sub_4DBF50
void ai_state_machine::process_return() { // back to the state the interrupt state interrupted
    const mashed_state* returning_state = my_curr_state->my_mashed_state;

    my_curr_state->deactivate(my_interrupted_state);
    delete my_curr_state;

    my_curr_state = base_state::create_subclass(my_interrupted_state->my_state_type);
    my_curr_state->activate(this, my_interrupted_state, returning_state, my_interrupted_params);
}

// sub_4DBD60
bool ai_state_machine::transition_state(string_hash new_state_id, const param_block* extra_params, e_state_exit_code the_exit_code) {
    const mashed_state* new_state = my_graph->find_state(new_state_id);

    if (!new_state) {
        // a state of an ancestor's graph is taken there, but only when the ancestor launched this branch
        ai_state_machine* child   = this;
        ai_state_machine* machine = my_parent;

        if (!machine)
            return false;

        while (!machine->my_graph->find_state(new_state_id)) {
            child   = machine;
            machine = machine->my_parent;

            if (!machine)
                return false;
        }

        if (!machine->my_curr_state->is_or_is_subclass_of(launch_layer_state::my_type_key.source_hash_code))
            return false;

        auto*        launcher = (launch_layer_state*)machine->my_curr_state;
        resource_key name     = child->get_name();

        if (!(launcher->get_layer_resource_key() == name))
            return false;

        child->external_request_exit();

        return machine->transition_state(new_state_id);
    }

    if (my_curr_state->my_mashed_state->is_flag_set(mashed_state::is_interrupt_state)) {
        if (!new_state->is_flag_set(mashed_state::is_interrupt_state)) {
            my_interrupted_state  = nullptr;
            my_interrupted_params = nullptr;
        }
    } else if (new_state->is_flag_set(mashed_state::is_interrupt_state)) {
        my_interrupted_state  = my_curr_state->my_mashed_state;
        my_interrupted_params = my_curr_state->my_extra_params;
    }

    my_prev_state = my_curr_state->my_mashed_state;

    if (the_exit_code > state_exit_code_exit_request &&
        the_exit_code < state_exit_code_no_exit      &&
        my_curr_state->is_enhanced_state()) {

        string_hash none;
        none.initialize(mash::ALLOCATED);

        string_hash event = my_curr_state->my_mashed_state->my_params.get_optional_pb_hash(references::event_hashes.get()[the_exit_code],
                                                                                            none,
                                                                                            nullptr);

        string_hash no_event;
        no_event.initialize(mash::ALLOCATED);

        if (event != no_event)
            my_curr_state->my_state_machine->my_core->get_actor()->raise_event(event);
    }

    my_curr_state->deactivate(new_state);
    delete my_curr_state;

    my_curr_state = base_state::create_subclass(new_state->my_state_type);
    my_curr_state->activate(this, new_state, my_prev_state, extra_params);

    return false;
}

// ==== attack_enabled_state ==== //

// slot 1
void attack_enabled_state::construct_mashed_class() {
    ((void (__thiscall*)(attack_enabled_state*))retail::sub_4028C0)(this);
}

// slot 2
void attack_enabled_state::destruct_mashed_class() {
    ((void (__thiscall*)(attack_enabled_state*))retail::sub_4028E0)(this);
}

// slot 3
void attack_enabled_state::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(attack_enabled_state*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_406650)(this,
                                                                                                                       mash_info,
                                                                                                                       containing_class_ptr,
                                                                                                                       buffer);
}

// slot 4
mash::virtual_types_key attack_enabled_state::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const attack_enabled_state*))retail::sub_4866A0)(this);
}

// slot 6
bool attack_enabled_state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const attack_enabled_state*, mash::virtual_types_key))retail::sub_5808A0)(this, parent_class);
}

// sub_4866B0
i32 attack_enabled_state::get_mash_sizeof() const {
    return sizeof(*this);
}

// slot 9
void attack_enabled_state::activate(      ai_state_machine* the_state_machine,
                                    const mashed_state*     the_mashed_state,
                                    const mashed_state*     prev_mashed_state,
                                    const param_block*      extra_params) {

    ((void (__thiscall*)(attack_enabled_state*,
                         ai_state_machine*,
                         const mashed_state*,
                         const mashed_state*,
                         const param_block*))retail::sub_4066F0)(this,
                                                                 the_state_machine,
                                                                 the_mashed_state,
                                                                 prev_mashed_state,
                                                                 extra_params);
}

// slot 10
void attack_enabled_state::deactivate(const mashed_state* next_mashed_state) {
    ((void (__thiscall*)(attack_enabled_state*, const mashed_state*))retail::sub_406CB0)(this, next_mashed_state);
}

// slot 11
e_state_exit_code attack_enabled_state::frame_advance(f32 delta_t) {
    return ((e_state_exit_code (__thiscall*)(attack_enabled_state*, f32))retail::sub_407F90)(this, delta_t);
}

// slot 20
i32 attack_enabled_state::unk_020(i32 unk) {
    return ((i32 (__thiscall*)(attack_enabled_state*, i32))retail::sub_403140)(this, unk);
}

// slot 21
i32 attack_enabled_state::unk_021(f32 unk) {
    return ((i32 (__thiscall*)(attack_enabled_state*, f32))retail::sub_403F10)(this, unk);
}

// slot 22
bool attack_enabled_state::unk_022(i32 unk_1, i32 unk_2) {
    return ((bool (__thiscall*)(attack_enabled_state*, i32, i32))retail::sub_405500)(this, unk_1, unk_2);
}

// slot 23
bool attack_enabled_state::unk_023() {
    return ((bool (__thiscall*)(attack_enabled_state*))retail::sub_405AD0)(this);
}
