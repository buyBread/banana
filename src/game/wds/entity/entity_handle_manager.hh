#pragma once

#include "game/wds/entity/entity_base.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/string_hash.hh"
#include "vc8/map.hh"
#include "vc8/vector.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class entity_handle_manager {

    public:
        static entity_base* find_entity(const string_hash &target_entity, e_entity_flavor flavor, bool unknown_ok = false);

    private:
        static void register_entity(entity_base* e);
        static void deregister_entity(entity_base* e);

        friend class entity_base;
    };

    // a generator of unique instance names
    struct name_pool {
        mash::string             prefix;
        u32                      unk_00c;
        u32                      unk_010;
        vc8::vector<string_hash> recycled_names; // handed back for reuse
    };

    ASSERT_SIZEOF  (name_pool,                 0x24);
    ASSERT_OFFSETOF(name_pool, recycled_names, 0x14);

    void generated_name_map_erase(const string_hash &id); // hands the name back to its pool

    namespace references {
        inline util::memory_reference<vc8::map<string_hash, entity_base*>> the_map { 0x01003600 };

        // every generated name that's out, with the pool it came from
        inline util::memory_reference<vc8::map<string_hash, name_pool*>> generated_name_map { 0x01003A30 };

        // nothing in the image writes it; find_entity formats a line per entity while it's set
        inline util::memory_reference<u8> unk_00ffeb19 { 0x00FFEB19 };
    } // references
} // treyarch
