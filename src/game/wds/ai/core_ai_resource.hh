#pragma once

#include "game/wds/ai/param_block.hh"
#include "treyarch/mash/mash_info.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "game/resource_key.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace amalga {
        struct resource_pack_slot;
    } // amalga

    class info_node;

    // .BAI; each ai_core unmashes its own copy of the info node image
    class alignas(8) core_ai_resource {

    public:
        void* self; // the resource handler points this at the resource itself

        u32                           unk_04;
        param_block                   my_params;
        mash::vector<info_node>*      my_info_nodes;
        u8*                           shared_info_node_buffer;
        mash::vector<resource_key>    my_base_graphs;
        mash::vector<resource_key>    my_locomotion_graphs;
        amalga::resource_pack_slot*   resource_context;
        u32                           normal_size_of_my_info_nodes;
        u32                           shared_size_of_my_info_nodes;
        bool                          low_priority_advance;
        u8                            pad_065[0x03];

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
        void custom_unmash(mash::mash_info_struct* mash_info,
                           void*                   containing_class_ptr,
                           mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (core_ai_resource,                               0x68);
    ASSERT_OFFSETOF(core_ai_resource, my_params,                    0x08);
    ASSERT_OFFSETOF(core_ai_resource, my_info_nodes,                0x28);
    ASSERT_OFFSETOF(core_ai_resource, shared_info_node_buffer,      0x2C);
    ASSERT_OFFSETOF(core_ai_resource, my_base_graphs,               0x30);
    ASSERT_OFFSETOF(core_ai_resource, my_locomotion_graphs,         0x44);
    ASSERT_OFFSETOF(core_ai_resource, resource_context,             0x58);
    ASSERT_OFFSETOF(core_ai_resource, normal_size_of_my_info_nodes, 0x5C);
    ASSERT_OFFSETOF(core_ai_resource, shared_size_of_my_info_nodes, 0x60);
    ASSERT_OFFSETOF(core_ai_resource, low_priority_advance,         0x64);
} // treyarch
