#pragma once

#include "game/arch_base_vhandle.hh"
#include "vc8/hash_set.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class entity;

    namespace limbo {
        void add_limbo_entity   (entity* ent); // raises in_limbo
        void remove_limbo_entity(entity* ent); // raises out_limbo
        void update_limbo_entity(entity* ent); // a member is judged by its owner

        namespace references {
            inline util::memory_reference<vc8::hash_set<arch_base_vhandle>> g_limbo_entities { 0x01075E30 };
        } // references
    } // limbo
} // treyarch
