#include <cassert>

#include "game/hash/algo.hh"
#include "treyarch/mash/virtual_base.hh"

using namespace treyarch;

mash::virtual_types_key mash::mash_virtual_base::type_key = 0x0668FEE9; // "mash_virtual_base"

// sub_A6B730
mash::virtual_types_key mash::mash_virtual_base::get_virtual_type_key() const {
    return type_key;
}

void mash::mash_virtual_base::copy_values(const mash_virtual_base* other) {
    assert(other != nullptr);
    assert(get_virtual_type_key() == other->get_virtual_type_key());
}

mash::virtual_types_key mash::mash_virtual_base::generate_virtual_types_key_from_string(const char* string) {
    return hash::djb2(string);
}

// sub_A6B740
bool mash::mash_virtual_base::is_or_is_subclass_of(virtual_types_key parent_class) const {
    return get_virtual_type_key() == parent_class || is_subclass_of(parent_class);
}

// sub_A6B9D0
void mash::mash_virtual_base::fixup_vtable(void* what_class_to_fixup) {
    // the first image word is a type key until this replaces it with an executable vtable
    vtable_key_t   key   = *(vtable_key_t*)what_class_to_fixup;
    vtable_value_t value = lookup_vtable_entry(key);

    if (value != invalid_vtable_value)
        *(vtable_value_t*)what_class_to_fixup = value;
}

// sub_A6B710
mash::mash_virtual_base* mash::mash_virtual_base::construct_class_helper(void* class_ptr) {
    mash_virtual_base* mashed_class = (mash_virtual_base*)class_ptr;
    mashed_class->construct_mashed_class();

    return mashed_class;
}

// sub_A6C760
bool mash::mash_virtual_base::register_vtable_entry(const vtable_key_t   &key,
                                                    const vtable_value_t &value) {

    auto result = references::vtable_map.get().insert({ key, value });

    references::vtable_set.get().insert(value);

    // a key registered twice keeps its first vtable
    return result.second || result.first->value.value == value;
}

mash::vtable_value_t mash::mash_virtual_base::lookup_vtable_entry(const vtable_key_t &key) {
    auto &vtable_map = references::vtable_map.get();
    auto* entry      = vtable_map.find(key);

    return entry == vtable_map.head() ? invalid_vtable_value : entry->value.value;
}
