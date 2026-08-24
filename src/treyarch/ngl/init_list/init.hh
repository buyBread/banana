#pragma once

#include "treyarch/ngl/init_list/init_list.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace ngl {
    void dispatch_init_list();

    namespace references {
        inline util::memory_reference<init_list_function> no_op_init_list { 0x00F4ABFC };
    } // references
}} // treyarch::ngl
