#pragma once

#include "treyarch/shared/container/skip_list.hh"

namespace treyarch { namespace ngl {
    template <typename value_type, typename key_access>
    
    struct instance_bank : container::skip_list<value_type, key_access> {};
}} // treyarch::ngl
