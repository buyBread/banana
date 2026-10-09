#pragma once

#include "treyarch/chuck/vm/script_instance.hh"
#include "treyarch/game/script/script_instance_shadow.hh"

namespace treyarch {
    class script {

    public:
        static void update_script_instance_shadow(chuck::vm::script_instance* inst);

        // inlined @ sub_825A30
        static script_instance_shadow* get_script_instance_shadow(chuck::vm::script_instance* inst) {
            return (script_instance_shadow*)inst->client_space;
        }
    };
} // treyarch
