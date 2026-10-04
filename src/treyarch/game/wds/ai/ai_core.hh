#pragma once

#include "treyarch/shared/mash/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class info_node;

    class ai_core {

    public:
        u8                       reserved_000[0x684];
        mash::vector<info_node>* my_info_node_list;
        u32                      info_node_mask; // bit n is set while info node n exists

        info_node* get_info_node(u32 index) const;
    };

    ASSERT_OFFSETOF(ai_core, my_info_node_list, 0x684);
    ASSERT_OFFSETOF(ai_core, info_node_mask,    0x688);
} // treyarch
