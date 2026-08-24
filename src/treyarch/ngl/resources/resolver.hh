#pragma once

#include "treyarch/shared/fixed_string.hh"

namespace treyarch { namespace ngl { namespace resources {
    void* __cdecl resolve(fixed_string* name, u32 type);
}}} // treyarch::ngl::resources
