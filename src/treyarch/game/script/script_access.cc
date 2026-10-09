#include "treyarch/game/script/script_access.hh"

using namespace treyarch;

// sub_825600
void script::update_script_instance_shadow(chuck::vm::script_instance* inst) {
    if (!inst || inst->client_space)
        return;

    script_instance_shadow* inst_shadow = new script_instance_shadow();
    inst_shadow->set_instance(inst);

    inst->client_space = inst_shadow;
}
