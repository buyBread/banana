#pragma once

#include <array>

#include "game/arch_base_vhandle.hh"
#include "game/container/fixed_vector.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class entity;

    namespace moved_entities {
        constexpr i32 max_moved = 0x640;

        void add_moved(arch_base_vhandle e);
        u32  get_moved_count();

        void remove_dead_registered_entities();
        void unregister_entity_to_update_every_frame(entity* ent);
        void add_registered_entities();

        namespace references {
            inline util::memory_reference<i32>                                            moved_count         { 0x01073E00 };
            inline util::memory_reference<std::array<arch_base_vhandle, max_moved>>       moved_list          { 0x01074078 };
            inline util::memory_reference<container::fixed_vector<arch_base_vhandle, 40>> registered_entities { 0x010768D8 };
        } // references
    } // moved_entities
} // treyarch
