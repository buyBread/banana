#pragma once

#include "treyarch/amalga/mashable_resource_handler.hh"
#include "treyarch/chuck/vm/script_executable.hh"

namespace treyarch { namespace amalga {
    using script_resource_handler = mashable_resource_handler<chuck::vm::script_executable>;
}} // treyarch::amalga
