#pragma once

#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/tl_fixed_string.hh"

namespace treyarch {
    using nal_anim_id = tl_fixed_string;

    namespace als {
        using category_id = string_hash;
        using state_id    = string_hash;
    } // als
} // treyarch
