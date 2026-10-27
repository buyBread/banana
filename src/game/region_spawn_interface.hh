#pragma once

#include "treyarch/mash/types.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace mash {
        class mash_info_struct;
    } // mash

    class region_spawn_interface {

    public:
        enum e_ped_reaction_type : i32 {
            ped_reaction_type_ignore,
            ped_reaction_type_cheer,
            ped_reaction_type_fear,
            ped_reaction_type_attack
        };

        void* self; // the resource handler points this at the interface itself

        e_ped_reaction_type ped_def_react_red;
        e_ped_reaction_type ped_def_react_black;
        e_ped_reaction_type ped_def_react_peter;

        // sub_7D3AE0
        void construct_mashed_class() {
            self = nullptr;
        }

        // sub_7C2DE0
        void destruct_mashed_class() {}

        // sub_636AB0
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}
    };

    ASSERT_SIZEOF  (region_spawn_interface,                      0x10);
    ASSERT_OFFSETOF(region_spawn_interface, ped_def_react_red,   0x04);
    ASSERT_OFFSETOF(region_spawn_interface, ped_def_react_peter, 0x0C);
} // treyarch
