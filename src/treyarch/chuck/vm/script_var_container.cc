#include "treyarch/chuck/vm/script_var_container.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;
using namespace treyarch::chuck::vm;

// sub_A20780
u8* script_var_container::get_address(const char* name) const {
    string_hash name_hash;
    name_hash.initialize(mash::ALLOCATED, name);

    i32 count = (i32)script_var_to_address.size;

    if (!count)
        return nullptr;

    i32 low  = 0;
    i32 high = count - 1;
    i32 mid  = count >> 1;

    const script_var_address_entry* entry = script_var_to_address.data[mid];

    while (entry->name_hash != name_hash.source_hash_code) {
        i32 previous = mid;

        if (entry->name_hash >= name_hash.source_hash_code) {
            high = mid - 1;

            if (high < 0)
                return nullptr;
        } else {
            low = mid + 1;

            if (low >= count)
                return nullptr;
        }

        if (low > high)
            return nullptr;

        mid = (high + low) >> 1;

        if (previous == mid)
            return nullptr;

        entry = script_var_to_address.data[mid];
    }

    return entry->address;
}

// sub_A20C10
void script_var_container::construct_mashed_class() {
    self = nullptr;

    script_var_block     .construct_mashed_class();
    script_var_to_address.construct_mashed_class();

    initialize(mash::FROM_MASH);
}

// sub_A20DE0
void script_var_container::destruct_mashed_class() {
    script_var_to_address.destruct_mashed_class();
}

// sub_A20C60
void script_var_container::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    script_var_block     .unmash(mash_info, this, buffer);
    script_var_to_address.unmash(mash_info, this, buffer);
}

// sub_A20810
void script_var_container::initialize(mash::allocation_scope scope) {
    if (scope == mash::ALLOCATED) {
        flags      = (e_script_var_container_flags)0;
        debug_info = nullptr;

        return;
    }

    flags = (e_script_var_container_flags)(flags | script_var_container_flag_from_mash);

    for (script_var_address_entry* entry : script_var_to_address) {
        i32 offset = (i32)entry->address;

        if (offset < 0)
            offset = -1 - offset;

        entry->address = script_var_block.buffer + offset;
    }

    flags = (e_script_var_container_flags)(flags & ~script_var_container_flag_unk_04);
}

// sub_A20D60
void script_var_container::destroy() {
    if (flags & script_var_container_flag_from_mash)
        return;

    script_var_to_address.clear();

    if (debug_info) {
        if (debug_info->var_to_offset) {
            debug_info->var_to_offset->~map();

            memory::heap::free(debug_info->var_to_offset);
        }

        memory::heap::free(debug_info);

        debug_info = nullptr;
    }

    flags = (e_script_var_container_flags)0;
}

// sub_A20DD0
void script_var_container::finalize(mash::allocation_scope scope) {
    if (scope == mash::ALLOCATED)
        destroy();
}
