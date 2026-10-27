#pragma once

#include "treyarch/base/tl_skip_list.hh"

namespace treyarch { namespace ngl {
    template <typename value_type, typename key_access>
    
    struct instance_bank : tl_skip_list<value_type, key_access> {};
}} // treyarch::ngl
