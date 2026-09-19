#pragma once

#include "treyarch/shared/fixed_string.hh"

namespace treyarch { namespace ngl { namespace resources {
    void* resolve(fixed_string* name, u32 type);
}}} // treyarch::ngl::resources
