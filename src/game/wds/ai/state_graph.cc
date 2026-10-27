#include "game/wds/ai/state_graph.hh"

using namespace treyarch;

// inlined @ sub_4CF0B0
void mashed_state::construct_mashed_class() {
    my_params                   .construct_mashed_class();
    my_default_state_transitions.construct_mashed_class();
}

// inlined @ sub_4DB230
void mashed_state::destruct_mashed_class() {
    my_params                   .destruct_mashed_class();
    my_default_state_transitions.destruct_mashed_class();
}

// inlined @ sub_4CDB50
void mashed_state::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    my_params                   .unmash(mash_info, this, buffer);
    my_default_state_transitions.unmash(mash_info, this, buffer);
}

// sub_4DA210
void state_graph::construct_mashed_class() {
    self = nullptr;

    my_name  .construct_mashed_class();
    my_states.construct_mashed_class();

    if (my_initial_state)
        my_initial_state->construct_mashed_class();
}

// sub_4DEE40
void state_graph::destruct_mashed_class() {
    my_name  .destruct_mashed_class();
    my_states.destruct_mashed_class();

    if (my_initial_state) {
        my_initial_state->destruct_mashed_class();
        my_initial_state = nullptr;
    }
}

// sub_4DABB0
void state_graph::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    my_name  .unmash(mash_info, this, buffer);
    my_states.unmash(mash_info, this, buffer);

    u8 class_mashed;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(my_initial_state, this, buffer);
    else
        my_initial_state = nullptr;
}
