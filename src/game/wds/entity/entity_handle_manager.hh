#pragma once

#include "game/wds/entity/entity_base.hh"
#include "treyarch/mash/string_hash.hh"
#include "vc8/map.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class entity_handle_manager {

    public:
        static entity_base* find_entity(const string_hash &target_entity, e_entity_flavor flavor, bool unknown_ok = false);

    private:
        static void register_entity(entity_base* e);

        friend class entity_base;
    };

    namespace references {
        inline util::memory_reference<vc8::map<string_hash, entity_base*>> the_map { 0x01003600 };

        // nothing in the image writes it; find_entity formats a line per entity while it's set
        inline util::memory_reference<u8> unk_00ffeb19 { 0x00FFEB19 };
    } // references
} // treyarch
