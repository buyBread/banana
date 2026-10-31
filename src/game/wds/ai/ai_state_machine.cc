#include "game/wds/ai/ai_state_machine.hh"

using namespace treyarch;

// sub_4C3A80
resource_key ai_state_machine::get_name() const {
    return my_graph->get_name();
}

// sub_4C6950
void ai_state_machine::request_exit() {
    my_curr_mode = 3;

    for (u32 index = 0; index < my_children.size(); ++index)
        my_children[index]->request_exit();
}

// sub_4C6990
void ai_state_machine::external_request_exit() {
    if (my_curr_mode == 3 || my_curr_mode == 5 || my_curr_mode == 4)
        return;

    request_exit();
}
