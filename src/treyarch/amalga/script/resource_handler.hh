#pragma once

#include "treyarch/amalga/mashable_resource_handler.hh"
#include "treyarch/chuck/vm/script_executable.hh"
#include "treyarch/chuck/vm/script_var_container.hh"

namespace treyarch { namespace amalga {
    using script_resource_handler               = mashable_resource_handler<chuck::vm::script_executable>;
    using script_var_container_resource_handler = mashable_resource_handler<chuck::vm::script_var_container>; // script_gv and script_sv
}} // treyarch::amalga
