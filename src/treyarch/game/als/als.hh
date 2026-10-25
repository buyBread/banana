#pragma once

#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/tl_fixed_string.hh"

namespace treyarch {
    using nal_anim_id = tl_fixed_string;

    namespace als {
        // WoS has more layers than SM3's ten, so only the base layer keeps its name
        enum layer_types : u32 {
            base_layer = 0
        };

        using layer_id    = layer_types;
        using category_id = string_hash;
        using state_id    = string_hash;
    } // als
} // treyarch
