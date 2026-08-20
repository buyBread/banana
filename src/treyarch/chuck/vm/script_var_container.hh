#pragma once

#include <map>

#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/mash/vector.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    struct script_var_debug_info {
        std::map<mash::string, i32>* var_to_offset;
    };

    struct script_var_address_entry {
        u32 name_hash; // mash::vector stores by ascending order of `name_hash`
        u8* address;
    };

    struct script_variable_block {
        void* buffer;
        u32   size;
        u32   ownership_flags;
        void* fixed_block_owner;
    };

    enum script_var_container_flags : u32 {
        script_var_from_mash = 1u << 0,
        script_var_is_game   = 1u << 1, // selects the game container
        script_var_unk_04    = 1u << 2, // is set after copying caller data into game-variable storage
    };

    struct script_var_container {
        u32                            unk_00; // overwritten during resource root construction, then... nothing?
        script_variable_block          script_var_block;
        mash::vector
            <script_var_address_entry> script_var_to_address;
        script_var_debug_info*         debug_info;
        script_var_container_flags     flags;

        void* find(u32 hash) const {
            i32 low  = 0;
            i32 high = script_var_to_address.size - 1;

            while (low <= high) {
                i32 middle = (low + high) / 2;
                
                auto* entry = script_var_to_address.data[middle];

                if (entry->name_hash == hash)
                    return entry->address;

                if (entry->name_hash < hash)
                    low  = middle + 1;
                else
                    high = middle - 1;
            }

            return nullptr;
        }
    };
}}} // treyarch::chuck::vm